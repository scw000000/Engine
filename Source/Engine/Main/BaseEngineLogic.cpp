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
#include "..\ResourceCache\XmlResource.h"
#include "..\Event\EventManager.h"

BaseEngineLogic::BaseEngineLogic( shared_ptr<IRenderer> pRenderer )
   {
   m_Lifetime = 0.f;
   m_LastActorId = 0;
	m_pProcessManager = ENG_NEW ProcessManager;
   if( pRenderer )
      {
      m_pWrold.reset( ENG_NEW Scene( pRenderer ) );
      m_pRenderer = pRenderer;
      }
	m_State = BGS_Initializing;
   m_pActorFactory = ENG_NEW ActorFactory;;
   }

BaseEngineLogic::~BaseEngineLogic()
   {
   while (!m_ViewList.empty())
		m_ViewList.pop_front();

	SAFE_DELETE(m_pProcessManager);
   SAFE_DELETE(m_pActorFactory);

    // destroy all actors
    for ( auto it = m_Actors.begin(); it != m_Actors.end(); ++it )
        it->second->Destroy();
    m_Actors.clear();

   }

bool BaseEngineLogic::Init()
   {
   if( !VLoadGame( g_pApp->m_EngineOptions.m_Level.c_str() ))
      {
      ENG_ERROR( "The game failed to load." );
      g_pApp->AbortGame( );
      return false;
      }
   m_pWrold->OnRestore();
   
   shared_ptr<IView> pView( ENG_NEW HumanView( ) );
   VAddView( pView );
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

StrongActorPtr BaseEngineLogic::VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4* initialTransform, const ActorId serversActorId )
   {
   ENG_ASSERT( m_pActorFactory );

   StrongActorPtr pActor = m_pActorFactory->CreateActor(actorResource.c_str() );
   if (pActor)
      {
      // Insert into actor map
      m_Actors.insert( std::make_pair(pActor->GetId(), pActor) );
      return pActor;
      }
   else
      {
      // FUTURE WORK: Log error: couldn't create actor
      return StrongActorPtr();
      }
   }

void BaseEngineLogic::VDestroyActor( const ActorId actorId )
   {
   // We need to trigger a synchronous event to ensure that any systems responding to this event can still access a 
   // valid actor if need be.  The actor will be destroyed after this.
   shared_ptr<EvtData_Destroy_Actor> pEvent( ENG_NEW EvtData_Destroy_Actor( actorId ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   auto findIt = m_Actors.find(actorId);
   if (findIt != m_Actors.end())
      {
      findIt->second->Destroy();
      m_Actors.erase(findIt);
      }
   }

WeakActorPtr BaseEngineLogic::VGetActor( const ActorId actorId )
   {
   ActorMap::iterator findIt = m_Actors.find(actorId);
    if (findIt != m_Actors.end())
       {
       return findIt->second;
       }
   return WeakActorPtr();
   }

void BaseEngineLogic::VModifyActor( const ActorId actorId, TiXmlElement *overrides )
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

void BaseEngineLogic::VOnUpdate( float time, float elapsedTime )
   {
   unsigned long deltaMs = unsigned long( elapsedTime * 1000.0f );
	m_Lifetime += elapsedTime;

   m_pWrold->OnUpdate( deltaMs );

   // update all game views
   for ( ViewList::iterator it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
	   {
		(*it)->VOnUpdate( deltaMs );
	   }

   // update game actors
   for ( ActorMap::const_iterator it = m_Actors.begin(); it != m_Actors.end(); ++it )
      {
      it->second->Update( deltaMs );
      }

   }

void BaseEngineLogic::VOnRender( double fTime, float fElapsedTime )
   {
   m_pRenderer->VPreRender( );
   for( auto pView : m_ViewList )
      {
      pView->VOnRender( fTime, fElapsedTime );
      }
   m_pRenderer->VPostRender( );
   }

// this function is called by EngineApp::VLoadGame
// LATER: finish implementation
bool BaseEngineLogic::VLoadGame( const char* levelResource )
   {
   
    // Grab the root XML node
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( levelResource );
   if (!pRoot)
      { 
      ENG_ERROR( "Failed to find level resource file: " + std::string( levelResource ) );
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
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the XML file from the zip file
      }

    // load all initial actors
    TiXmlElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
   if ( pActorsNode )
      {
      for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
         {
         const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode);
			if ( pActor )
			   {
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
         pHumanView->LoadGame( pRoot );
         }
      }

    // register script events from the engine
	//   [mrmike] this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
    // RegisterEngineScriptEventImps();

    // load the post-load script if there is one
   if ( postLoadScript )
      {
      Resource resource( postLoadScript );
      shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the XML file from the zip file
      }
   
   return true;
   }
