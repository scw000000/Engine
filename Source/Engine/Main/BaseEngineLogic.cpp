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

BaseEngineLogic::BaseEngineLogic()
   {
   m_Lifetime = 0.f;
   m_LastActorId = 0;
	m_pProcessManager = ENG_NEW ProcessManager;
	m_State = BGS_Initializing;
   m_pActorFactory = NULL;
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
   m_pActorFactory = VCreateActorFactory();
   return true;
   }

void BaseEngineLogic::VAddView( shared_ptr<IGameView> pView )
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

void BaseEngineLogic::VOnUpdate( float time, float elapsedTime )
   {
   unsigned long deltaMilliseconds = unsigned long( elapsedTime * 1000.0f );
	m_Lifetime += elapsedTime;

	switch(m_State)
	{
		case BGS_Initializing:
			// If we get to here we're ready to attach players
			VChangeState( BGS_MainMenu );
			break;

		case BGS_MainMenu:
      if ( !g_pApp->m_EngineOptions.m_Level.empty() )
				   {
					VChangeState(BGS_LoadingGameEnvironment);
				   }
			break;

		case BGS_LoadingGameEnvironment:

			break;

		case BGS_WaitingForPlayersToLoadEnvironment:
				VChangeState( BGS_SpawningPlayersActors );
			break;

		case BGS_SpawningPlayersActors:
			VChangeState( BGS_Running );
			break;

		case BGS_WaitingForPlayers:
				// The server sends us the level name as a part of the login message. 
				// We have to wait until it arrives before loading the level
				if ( !g_pApp->m_EngineOptions.m_Level.empty() )
				   {
					VChangeState(BGS_LoadingGameEnvironment);
				   }
			break;

		case BGS_Running:
			m_pProcessManager->UpdateProcesses( deltaMilliseconds );
			break;

		default:
			ENG_ERROR("Unrecognized state.");
	   }

    // update all game views
    for ( ViewList::iterator it = m_ViewList.begin(); it != m_ViewList.end(); ++it )
	   {
		(*it)->VOnUpdate( deltaMilliseconds );
	   }

    // update game actors
    for ( ActorMap::const_iterator it = m_Actors.begin(); it != m_Actors.end(); ++it )
      {
      it->second->Update( deltaMilliseconds );
      }

   }

void BaseEngineLogic::VChangeState( BaseGameState newState )
   {
   if ( newState == BGS_WaitingForPlayers )
	   {
		// Get rid of the Main Menu...
		m_ViewList.pop_front();

		// Note: Split screen support would require this to change!

      /* online gaming, skipped
		if ( !g_pApp->m_Options.m_gameHost.empty() )
		   {
		   }
		else if (m_ExpectedRemotePlayers > 0)
		   {
		   }*/
	}
	else if ( newState == BGS_LoadingGameEnvironment )
	{
		m_State = newState;
		if ( !g_pApp->VLoadLevel() )
		   {
			ENG_ERROR("The game failed to load.");
			g_pApp->AbortGame();
		   }
		else
		   {
			VChangeState( BGS_WaitingForPlayersToLoadEnvironment );			// we must wait for all human player to report their environments are loaded.
			return;
		   }
	}

	m_State = newState;
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
      shared_ptr<IGameView> pView = *it;
      if ( pView->VGetType() == GameView_Human )
         {
         shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(pView);
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


ActorFactory* BaseEngineLogic::VCreateActorFactory( void )
   {
   return ENG_NEW ActorFactory;
   }