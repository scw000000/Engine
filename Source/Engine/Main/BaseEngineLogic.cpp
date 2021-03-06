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
#include "..\Physics\BulletPhysics.h"
#include "..\Physics\PEPhysics.h"
#include "..\Physics\Raycast.h"
#include "..\Graphics\BasicGeometry.h"
#include "..\Animation\AnimationManager.h"

void LevelManager::Init(void)
   {
   auto currentLevel = g_pApp->m_pResCache->Match( g_pApp->m_EngineOptions.GetFullLevelDirectory() + "level.xml" );
   ENG_ASSERT( currentLevel.size() );
   pCurrentLevelRes.reset( ENG_NEW Resource( currentLevel[0] ) );
   }

BaseEngineLogic::BaseEngineLogic( shared_ptr< IRenderManager > pRenderManager ) : m_pGUIManager( ENG_NEW GUIManager )
   {
   m_Lifetime = 0.f;
   m_LastActorId = 0;
   m_HasStarted = false;
	m_pProcessManager = ENG_NEW ProcessManager;
   if( pRenderManager )
      {
      m_pWrold.reset( ENG_NEW Scene( pRenderManager ) );
      m_pRenderManager = pRenderManager;
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
   SBasicGeometry::GetSingleton().Init();
   // IGamePhysics::RegisterImplementation< BulletPhysics >();
   IGamePhysics::RegisterImplementation< PEPhysics >();
   IGamePhysics::GetSingleton().VInitialize();
   m_pLevelManager->Init();
   m_pWrold->OnRestore();
   m_pGUIManager->Init( g_pApp->m_EngineOptions.GetGUIDirectory() );
   shared_ptr<IView> pView( ENG_NEW HumanView() );
   VAddView( pView );
   if( !VLoadLevel() )
      {
      ENG_ERROR( "The game failed to load." );
      g_pApp->AbortGame( );
      return false;
      }
   // render manager need the information of camera to restore, which is in the scene
   m_pRenderManager->VOnRestore( m_pWrold.get() );
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

StrongActorPtr BaseEngineLogic::VCreateActor( shared_ptr< Resource > pActorRes, TransformPtr pTransform, ActorId serversActorId )
   {
   ENG_ASSERT( m_pActorFactory );
   shared_ptr< Resource > actorClassRes = pActorRes;
   shared_ptr< Resource > pActorOverridesRes;
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( pActorRes );
   std::string rootName = pRoot->Value();

   if( !std::strcmp( rootName.c_str(), "ActorInstance" ) ) // If this file is overrides file, find the actor class res
      {
      TiXmlHandle actorClassResHandle = XmlResourceLoader::LoadAndReturnRootXmlElement( pActorRes );
      TiXmlElement* pActorClassReshNode = actorClassResHandle.FirstChild( "Data" ).FirstChild( "ActorClassResource" ).ToElement();
      ENG_ASSERT( pActorClassReshNode );
      actorClassRes = shared_ptr< Resource >( ENG_NEW Resource( pActorClassReshNode->Attribute( "path" ) ) );
      pActorOverridesRes = pActorRes;
      }

   StrongActorPtr pActor = m_pActorFactory->CreateActor( actorClassRes, pActorOverridesRes, pTransform );
   if( pActor )
       {
       // Insert into actor map
       m_Actors.insert( std::make_pair( pActor->GetId(), pActor ) );
       ENG_LOG( "Actor", std::string( "Actor " ) + std::to_string( pActor->GetId() ) + " added" );
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

// I don't think it will be called for now
int BaseEngineLogic::VOnRestore( void )
   {
   //m_pWrold->OnRestore();
   //for( auto pView : m_ViewList )
   //   {
   //   pView->VOnRestore();
   //   }
   //// render manager need the information of camera to restore, which is in the scene
   //m_pRenderManager->VOnRestore( m_pWrold.get() );
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

void BaseEngineLogic::VOnUpdate( float time, float deltaSeconds )
   {
   unsigned long deltaMs = unsigned long( deltaSeconds * 1000.0f );
	m_Lifetime += deltaSeconds;

   m_pGUIManager->OnUpdate( deltaSeconds );
   // update all game views
   for ( ViewList::iterator it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
	   {
      ( *it )->VOnUpdate( deltaSeconds );
	   }

   if( m_EnableActorUpdate )
      {
      // update game actors
      for( ActorMap::const_iterator it = m_Actors.begin(); it != m_Actors.end(); ++it )
         {
         it->second->Update( deltaSeconds );
         }
      }

   if( m_EnableWorldUpdate )
      {
      m_pWrold->OnUpdate( deltaSeconds );
      }

   AnimationManager::GetSingleton().VUpdate( deltaSeconds );

   IGamePhysics::GetSingleton().VOnUpdate( deltaSeconds );
   IGamePhysics::GetSingleton().VSyncVisibleScene();
   }

void BaseEngineLogic::VOnRender( double fTime, float fElapsedTime )
   {
   m_pRenderManager->VPreRender( );
   for( auto pView : m_ViewList )
      {
      pView->VOnRender( fTime, fElapsedTime );
      }
   m_pRenderManager->VSSAOPass();
   m_pRenderManager->VLightingPass( m_pWrold.get() );
   m_pRenderManager->VBloomPass();
   m_pRenderManager->VMotionBlurPass();
   m_pRenderManager->VToneMappingPass();
   VRenderDiagnostics();
   //Render GUI last
   m_pGUIManager->OnRender( fTime, fElapsedTime );
   m_pRenderManager->VPostRender();
   }

void BaseEngineLogic::VRenderDiagnostics( void ) const
   {
   if( m_RenderDiagnostics )
      {
      IGamePhysics::GetSingleton().VRenderDiagnostics();
      }
   }

void BaseEngineLogic::VOnFileDrop( const char* filePath, const Point& dropLocation ) 
   {
   shared_ptr< Resource > pFileRes( ENG_NEW Resource( filePath ) );
   // std::string extension = fileRes.GetExtension();
   // this means it's drag-dropped from folder directly
   if( WildcardMatch( "*game*", pFileRes->m_Name.c_str() ) )
      {
      // int startIdx = std::string::find_last_of( pFileRes->m_Name.c_str(), "assets" );
      auto startIdx = pFileRes->m_Name.find_first_of( "game" );
      if( startIdx != std::string::npos )
         {
         std::cout << pFileRes->m_Name.substr( startIdx ) << " ddddd";
         //pFileRes->m_Name = pFileRes->m_Name.substr( startIdx + std::string( "assets\\" ).length() );
         }
      
      }
   if( WildcardMatch( "*.xml", pFileRes->m_Name.c_str() ) )
      {
      TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( pFileRes );
      std::string rootName = pRoot->Value();
      TransformPtr pTransform( m_pWrold->GetCamera()->VGetProperties().GetTransformPtr() );
      Vec3 projStart;
      Vec3 projEnd;
      m_pWrold->GetCamera()->GetScreenProjectPoint( projStart, projEnd, dropLocation, 17.f );
      pTransform->SetPosition( projEnd );
      if( !std::strcmp( rootName.c_str(), "ActorClass" ) )
         {
         VCreateActor( pFileRes, pTransform );
         }
      else if( !std::strcmp( rootName.c_str(), "World" ) )
         {
         VClearWorld();
         std::string newLevelInstanceDir = pFileRes->GetFileName();
         auto rBound = pFileRes->m_Name.find_last_of( "\\" );
         auto firstIdx = pFileRes->m_Name.find_last_of( "\\", rBound - 1 );
         ++firstIdx;
         std::string levelFolderName = pFileRes->m_Name.substr( firstIdx, rBound + 1 - firstIdx );
         g_pApp->m_EngineOptions.SetLevelInstanceDirectory( levelFolderName );
         VLoadLevel();
         }
      }
   }


// this function is called by EngineApp::VLoadGame
// LATER: finish implementation
bool BaseEngineLogic::VLoadLevel()
   {
   m_pLevelManager->Init();
   shared_ptr< Resource > pLevelRes( ENG_NEW Resource( m_pLevelManager->GetCurrentLevel() ) );
    // Grab the root XML node
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( pLevelRes );
   if (!pRoot)
      { 
      ENG_ERROR( "Failed to find level resource file: " + pLevelRes->m_Name );
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
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( shared_ptr< Resource >( ENG_NEW Resource( preLoadScript ) ) );  // this actually loads the XML file from the zip file
      }

    // load all initial actors
    TiXmlElement* pActorsNode = pRoot->FirstChildElement( "StaticActors" );
   if ( pActorsNode )
      {
      for ( TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
         {
         std::string actorFileName = pNode->Attribute( "actorinstance" );
         StrongActorPtr pActor = VCreateActor( shared_ptr< Resource >( ENG_NEW Resource( g_pApp->m_EngineOptions.GetFullActorInstanceDirectory() + actorFileName ) ) );
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
	 // this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
    // RegisterEngineScriptEventImps();

    // load the post-load script if there is one
   if ( postLoadScript )
      {
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( shared_ptr< Resource >( ENG_NEW Resource( postLoadScript ) ) );  // this actually loads the XML file from the zip file
      }
   ENGRandom random;
   for( int i = 0; i < 100; ++i )
      {
      float x = random.Random() * ( 1421.f + 1290.f ) - 1290.f;
      float z = random.Random() * ( 634 + 565 ) - 565.f;
      TransformPtr pTransform( ENG_NEW Transform( Vec3( x, 20.f, z ) ) );
      VCreateActor( shared_ptr< Resource >( ENG_NEW Resource( "actors\\light.xml" ) ), pTransform );
      }

   VSetIsSimulating( true );

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
   VSetWorldUpdate( isOn );
   AnimationManager::GetSingleton().VSetIsRunning( isOn );
   }

void BaseEngineLogic::VGameStart( void )
   {
   //TODO: send game start event
   VSetIsSimulating( true );
   }

void BaseEngineLogic::ReInitWorld( void )
   {
   shared_ptr< Resource > levelRes( ENG_NEW Resource( ( m_pLevelManager->GetCurrentLevel() ).c_str() ) );
   // Grab the root XML node
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( levelRes );
   if( !pRoot )
      {
      ENG_ERROR( "Failed to find level resource file: " + levelRes->m_Name );
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
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( shared_ptr< Resource >( ENG_NEW Resource( preLoadScript ) ) );  // this actually loads the XML file from the zip file
      }

   for( auto actorIt : m_Actors )
      {
      TiXmlElement* pActorData = XmlResourceLoader::LoadAndReturnRootXmlElement( 
         shared_ptr< Resource >( ENG_NEW Resource( actorIt.second->m_pActorClassResource->m_Name ) ) );
      m_pActorFactory->ModifyActor( actorIt.second, pActorData );
      if( actorIt.second->m_pActorInstanceResource )
         {
         pActorData = XmlResourceLoader::LoadAndReturnRootXmlElement( 
            shared_ptr< Resource >( ENG_NEW Resource( actorIt.second->m_pActorInstanceResource->m_Name ) ) );
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

  // VOnRestore();

   // register script events from the engine
   //   [mrmike] this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
   // RegisterEngineScriptEventImps();

   // load the post-load script if there is one
   if( postLoadScript )
      {
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( shared_ptr< Resource >( ENG_NEW Resource( postLoadScript ) ) );
      }

   //return true;
   }

void BaseEngineLogic::SetNextEngineState( BaseEngineState nxtState )
   {
   m_EngineState = nxtState;
   }
