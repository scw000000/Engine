/*!
 * \file BaseEngineLogic.cpp
 * \date 2016/05/14 21:30
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */
#include "EngineStd.h"
#include "BaseEngineLogic.h"
#include "..\Mainloop\Initialization.h"			// only for GameOptions
#include "..\MainLoop\Process.h"
#include "..\Actors\Actor.h"
#include "..\Actors\ActorFactory.h"
#include "..\Utilities\String.h"
#include "..\UserInterface\HumanView.h"
#include "..\UserInterface\GUIManager.h"
#include "..\ResourceCache\XmlResource.h"
#include "..\Event\EventManager.h"
#include "..\Physics\Physics.h"
#include "..\Physics\Raycast.h"

void LevelManager::Init(void)
   {
   auto currentLevel = g_pApp->m_pResCache->Match( g_pApp->m_EngineOptions.GetFullLevelDirectory() + "level.xml" );
   ENG_ASSERT( currentLevel.size() );
   pCurrentLevelRes.reset( ENG_NEW Resource( currentLevel[0] ) );
   }

BaseEngineLogic::BaseEngineLogic( shared_ptr<IRenderer> pRenderer ) : m_pGUIManager( ENG_NEW GUIManager )
   {
   m_Lifetime = 0.f;
   m_LastActorId = 0;
   m_HasStarted = false;
	m_pProcessManager = ENG_NEW ProcessManager;
   if( pRenderer )
      {
      m_pWrold.reset( ENG_NEW Scene( pRenderer ) );
      m_pRenderer = pRenderer;
      }
   m_pActorFactory = ENG_NEW ActorFactory;
   m_pLevelManager = ENG_NEW LevelManager;
   m_EnableActorUpdate = true;
   m_EnableWorldUpdate = true;
   m_RenderDiagnostics = true;
   m_EngineState = BES_Invalid;
   }

BaseEngineLogic::~BaseEngineLogic()
   {
   while ( !m_ViewList.empty() )
      {
		m_ViewList.pop_front();
      }
	SAFE_DELETE( m_pProcessManager );
   SAFE_DELETE( m_pLevelManager );
   SAFE_DELETE( m_pActorFactory );
   m_pGUIManager->Destory();
    // destroy all actors
   for ( auto it = m_Actors.begin(); it != m_Actors.end(); ++it )
      {
      it->second->Destroy();
      }
   m_Actors.clear();
   IGamePhysics::Shutdown();
   }

bool BaseEngineLogic::Init()
   {
   IGamePhysics::RegisterImplementation< BulletPhysics >();
   IGamePhysics::GetSingleton().VInitialize();
   m_pLevelManager->Init();
   m_pWrold->OnRestore();
   if( !VLoadLevel() )
      {
      ENG_ERROR( "The game failed to load." );
      g_pApp->AbortGame( );
      return false;
      }
   m_pGUIManager->Init( g_pApp->m_EngineOptions.GetGUIDirectory() );

   shared_ptr<IView> pView( ENG_NEW HumanView( ) );
   VAddView( pView );
   SetNextEngineState( BES_Ready );
   return true;  
   }

void BaseEngineLogic::VAddView( shared_ptr<IView> pView )
   {
   // This makes sure that all views have a non-zero view id.
	int viewId = static_cast<int>( m_ViewList.size() );
	m_ViewList.push_back( pView );
	pView->VOnAttach( viewId );
	pView->VOnRestore(); 
   }

StrongActorPtr BaseEngineLogic::VCreateActor( const Resource& actorRes, TransformPtr pTransform, ActorId serversActorId )
   {
   ENG_ASSERT( m_pActorFactory );
   Resource actorClassRes = actorRes;
   const Resource* pActorOverridesRes = NULL;
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( actorRes );
   std::string rootName = pRoot->Value();

   if( !std::strcmp( rootName.c_str(), "ActorInstance" ) ) // If this file is overrides file, find the actor class res
      {
      TiXmlHandle actorClassResHandle = XmlResourceLoader::LoadAndReturnRootXmlElement( actorRes );
      TiXmlElement* pActorClassReshNode = actorClassResHandle.FirstChild( "Data" ).FirstChild( "ActorClassResource" ).ToElement();
      ENG_ASSERT( pActorClassReshNode );
      actorClassRes = Resource( pActorClassReshNode->Attribute( "path" ) );
      pActorOverridesRes = &actorRes;
      }

   StrongActorPtr pActor = m_pActorFactory->CreateActor( actorClassRes, pActorOverridesRes, pTransform );
   if( pActor )
       {
       // Insert into actor map
       m_Actors.insert( std::make_pair( pActor->GetId(), pActor ) );
       return pActor;
       }
    else
       {
       // FUTURE WORK: Log error: couldn't create actor
       return StrongActorPtr();
       }
   }

void BaseEngineLogic::VDestroyActor( ActorId actorId )
   {
   // We need to trigger a synchronous event to ensure that any systems responding to this event can still access a 
   // valid actor if need be.  The actor will be destroyed after this.
   shared_ptr<Event_Destroy_Actor> pEvent( ENG_NEW Event_Destroy_Actor( actorId ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   auto findIt = m_Actors.find(actorId);
   if (findIt != m_Actors.end())
      {
      findIt->second->Destroy();
      m_Actors.erase(findIt);
      }
   }

WeakActorPtr BaseEngineLogic::VGetActor( ActorId actorId )
   {
   ActorMap::iterator findIt = m_Actors.find(actorId);
    if (findIt != m_Actors.end())
       {
       return findIt->second;
       }
   return WeakActorPtr();
   }

void BaseEngineLogic::VModifyActor( ActorId actorId, TiXmlElement *overrides )
   {
   ENG_ASSERT( m_pActorFactory );
	// if (!m_pActorFactory)
	//	return;

	auto findIt = m_Actors.find(actorId);
   if (findIt != m_Actors.end())
      {
		m_pActorFactory->ModifyActor( findIt->second, overrides );
      }
   }

int BaseEngineLogic::VOnRestore( void )
   {
   m_pWrold->OnRestore();
   for( auto pView : m_ViewList )
      {
      pView->VOnRestore();
      }
   return S_OK;
   }

void BaseEngineLogic::VOnMsgProc( SDL_Event event )
   {
   if( m_pGUIManager->OnMsgProc( event ) )
      {
      return;
      }
   // send event to all of game views
   for( auto i = m_ViewList.rbegin( ); i != m_ViewList.rend( ); ++i )
      {
      if( ( *i )->VOnMsgProc( event ) )
         {
         return;
         }
      }
   }

void BaseEngineLogic::VSetActorUpdate( bool isUpdatable )
   {
   m_EnableActorUpdate = isUpdatable;
   }

void BaseEngineLogic::VSetWorldUpdate( bool isUpdatable )
   {
   m_EnableWorldUpdate = isUpdatable;
   }

void BaseEngineLogic::VOnUpdate( float time, float elapsedTime )
   {
   unsigned long deltaMs = unsigned long( elapsedTime * 1000.0f );
	m_Lifetime += elapsedTime;

   IGamePhysics::GetSingleton().VOnUpdate( elapsedTime );
   IGamePhysics::GetSingleton().VSyncVisibleScene();

   if( m_EnableWorldUpdate )
      {
      m_pWrold->OnUpdate( deltaMs );
      }
   m_pGUIManager->OnUpdate( deltaMs );
   // update all game views
   for ( ViewList::iterator it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
	   {
		(*it)->VOnUpdate( deltaMs );
	   }

   if( m_EnableActorUpdate )
      {
      // update game actors
      for( ActorMap::const_iterator it = m_Actors.begin( ); it != m_Actors.end( ); ++it )
         {
         it->second->Update( deltaMs );
         }
      }
   }

void BaseEngineLogic::VOnRender( double fTime, float fElapsedTime )
   {
   m_pRenderer->VPreRender( );
   for( auto pView : m_ViewList )
      {
      pView->VOnRender( fTime, fElapsedTime );
      }
   //Render GUI last
   m_pGUIManager->OnRender( fTime, fElapsedTime );
   VRenderDiagnostics();
   m_pRenderer->VPostRender( );
   }

void BaseEngineLogic::VRenderDiagnostics( void ) const
   {
   if( m_RenderDiagnostics )
      {
      IGamePhysics::GetSingleton().VRenderDiagnostics();
      }
   }

// this function is called by EngineApp::VLoadGame
// LATER: finish implementation
bool BaseEngineLogic::VLoadLevel()
   {
   m_pLevelManager->Init();

   Resource levelRes( m_pLevelManager->GetCurrentLevel() );
    // Grab the root XML node
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( levelRes );
   if (!pRoot)
      { 
      ENG_ERROR( "Failed to find level resource file: " + levelRes.m_Name );
      return false;
      }

    // pre and post load scripts
   const char* preLoadScript = NULL;
   const char* postLoadScript = NULL;

    // parse the pre & post script attributes
   TiXmlElement* pScriptElement = pRoot->FirstChildElement("Script");
   if (pScriptElement)
      {
      preLoadScript = pScriptElement->Attribute("preLoad");
      postLoadScript = pScriptElement->Attribute("postLoad");
      }

    // load the pre-load script if there is one
   if (preLoadScript)
      {
      Resource resource( preLoadScript );
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( resource );  // this actually loads the XML file from the zip file
      }

    // load all initial actors
    TiXmlElement* pActorsNode = pRoot->FirstChildElement( "StaticActors" );
   if ( pActorsNode )
      {
      for ( TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
         {
         std::string actorFileName = pNode->Attribute( "actorinstance" );
         Resource overridesResource( g_pApp->m_EngineOptions.GetFullActorInstanceDirectory() + actorFileName );
         StrongActorPtr pActor = VCreateActor( overridesResource );
       //  StrongActorPtr pActor = VCreateActorFromOverrides( overridesResource );
			if ( pActor )
			   {
            //std::cout << XMLHelper::WriteXMLToString( pActor->GenerateXML() ).c_str() << std::endl;
				// fire an event letting everyone else know that we created a new actor
			//	shared_ptr<EvtData_New_Actor> pNewActorEvent( ENG_NEW EvtData_New_Actor(pActor->GetId()));
			//	IEventManager::Get()->VQueueEvent(pNewActorEvent);
			   }
         }
      }

    // initialize all human views
   for ( auto it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
      {
      shared_ptr<IView> pView = *it;
      if ( pView->VGetType() == View_Human )
         {
         shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IView>(pView);
         pHumanView->LoadLevel( pRoot );
         }
      }

    // register script events from the engine
	//   [mrmike] this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
    // RegisterEngineScriptEventImps();

    // load the post-load script if there is one
   if ( postLoadScript )
      {
      Resource resource( postLoadScript );
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( resource );  // this actually loads the XML file from the zip file
      }
   
   return true;
   }

void BaseEngineLogic::VClearWorld( void )
   {
   VSetIsSimulating( false );
   m_HasStarted = false;
   for( auto it = m_Actors.begin(); it != m_Actors.end(); )
      {
      ActorId id = it->first;
      ++it;
      VDestroyActor( id );
      }
   m_Actors.clear();
   m_pActorFactory->ClearActorId();
   }

void BaseEngineLogic::VStartAndPause( void ) 
   {
   switch( m_EngineState )
      {
      case BES_Invalid:
         break;
      case BES_Ready:
         SetNextEngineState( BES_Running );
         VGameStart();
         VSetIsSimulating( true );
         break;
      case BES_Running:
         SetNextEngineState( BES_Pause );
         VSetIsSimulating( false );
         break;
      case BES_Pause:
         SetNextEngineState( BES_Running );
         VSetIsSimulating( true );
         break;
      };
   }

void BaseEngineLogic::VStop( void ) 
   {
   switch( m_EngineState )
      {
      case BES_Invalid:
         break;
      case BES_Ready:
         break;
      case BES_Running:
      case BES_Pause:
         SetNextEngineState( BES_Ready );
         VSetIsSimulating( false );
         ReInitWorld();
         break;
      };
   }

void BaseEngineLogic::VSetIsSimulating( bool isOn )
   {

   IGamePhysics::GetSingleton().VSetSimulation( isOn );
   }

void BaseEngineLogic::VGameStart( void )
   {
   //TODO: send game start event
   
   }

void BaseEngineLogic::ReInitWorld( void )
   {
   Resource levelRes( ( m_pLevelManager->GetCurrentLevel() ).c_str() );
   // Grab the root XML node
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( levelRes );
   if( !pRoot )
      {
      ENG_ERROR( "Failed to find level resource file: " + levelRes.m_Name );
      //return false;
      }

   // pre and post load scripts
   const char* preLoadScript = NULL;
   const char* postLoadScript = NULL;

   // parse the pre & post script attributes
   TiXmlElement* pScriptElement = pRoot->FirstChildElement( "Script" );
   if( pScriptElement )
      {
      preLoadScript = pScriptElement->Attribute( "preLoad" );
      postLoadScript = pScriptElement->Attribute( "postLoad" );
      }

   // load the pre-load script if there is one
   if( preLoadScript )
      {
      Resource resource( preLoadScript );
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( resource );  // this actually loads the XML file from the zip file
      }

   for( auto actorIt : m_Actors )
      {
      TiXmlElement* pActorData = XmlResourceLoader::LoadAndReturnRootXmlElement( actorIt.second->m_pActorClassResource->m_Name );
      m_pActorFactory->ModifyActor( actorIt.second, pActorData );
      if( actorIt.second->m_pActorInstanceResource )
         {
         pActorData = XmlResourceLoader::LoadAndReturnRootXmlElement( actorIt.second->m_pActorInstanceResource->m_Name );
         m_pActorFactory->ModifyActor( actorIt.second, pActorData );
         }
      }

   // initialize all human views
   for( auto it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
      {
      shared_ptr<IView> pView = *it;
      if( pView->VGetType() == View_Human )
         {
         shared_ptr<HumanView> pHumanView = static_pointer_cast< HumanView, IView >( pView );
         //pHumanView->LoadLevel( pRoot );
         }
      }

   // register script events from the engine
   //   [mrmike] this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
   // RegisterEngineScriptEventImps();

   // load the post-load script if there is one
   if( postLoadScript )
      {
      Resource resource( postLoadScript );
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( resource );  // this actually loads the XML file from the zip file
      }

   //return true;
   }

void BaseEngineLogic::SetNextEngineState( BaseEngineState nxtState )
   {
   m_EngineState = nxtState;
   }
