////////////////////////////////////////////////////////////////////////////////
// Filename: BaseGameLogic.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "EngineStd.h"
#include "BaseGameLogic.h"
#include "../Mainloop/Initialization.h"			// only for GameOptions
#include "../MainLoop/Process.h"
#include "../Actors/Actor.h"
#include "../Actors/ActorFactory.h"
#include "../Utilities/String.h"

BaseGameLogic::BaseGameLogic()
   {
   m_Lifetime = 0.f;
   m_LastActorId = 0;
	m_pProcessManager = ENG_NEW ProcessManager;
	m_State = BGS_Initializing;
	m_ExpectedPlayers = 0;
	m_ExpectedRemotePlayers = 0;
	m_ExpectedAI = 0;
	m_HumanPlayersAttached = 0;
	m_AIPlayersAttached = 0;
	m_HumanGamesLoaded = 0;
   m_pActorFactory = NULL;
   }

BaseGameLogic::~BaseGameLogic()
   {
   while (!m_gameViews.empty())
		m_gameViews.pop_front();

	SAFE_DELETE(m_pProcessManager);
    SAFE_DELETE(m_pActorFactory);

    // destroy all actors
    for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
        it->second->Destroy();
    m_actors.clear();

   }

bool BaseGameLogic::Init()
   {
   m_pActorFactory = VCreateActorFactory();
   return true;
   }

void BaseGameLogic::VAddView( shared_ptr<IGameView> pView, ActorId actorId )
   {
   // This makes sure that all views have a non-zero view id.
	int viewId = static_cast<int>( m_gameViews.size() );
	m_gameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
   }

StrongActorPtr BaseGameLogic::VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4* initialTransform, const ActorId serversActorId )
   {
   ENG_ASSERT( m_pActorFactory );

    StrongActorPtr pActor = m_pActorFactory->CreateActor(actorResource.c_str() );
    if (pActor)
      {
      // Insert into actor map
      m_actors.insert( std::make_pair(pActor->GetId(), pActor) );
      return pActor;
      }
    else
      {
      // FUTURE WORK: Log error: couldn't create actor
      return StrongActorPtr();
      }
   }

void BaseGameLogic::VDestroyActor( const ActorId actorId )
   {
   // We need to trigger a synchronous event to ensure that any systems responding to this event can still access a 
   // valid actor if need be.  The actor will be destroyed after this.

   auto findIt = m_actors.find(actorId);
   if (findIt != m_actors.end())
      {
      findIt->second->Destroy();
      m_actors.erase(findIt);
      }
   }

WeakActorPtr BaseGameLogic::VGetActor( const ActorId actorId )
   {
   ActorMap::iterator findIt = m_actors.find(actorId);
    if (findIt != m_actors.end())
       {
       return findIt->second;
       }
   return WeakActorPtr();
   }

void BaseGameLogic::VModifyActor( const ActorId actorId, TiXmlElement *overrides )
   {
   ENG_ASSERT(m_pActorFactory);
	// if (!m_pActorFactory)
	//	return;

	auto findIt = m_actors.find(actorId);
   if (findIt != m_actors.end())
      {
		m_pActorFactory->ModifyActor( findIt->second, overrides );
      }
   }

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
   {
   int deltaMilliseconds = int( elapsedTime * 1000.0f );
	m_Lifetime += elapsedTime;

	switch(m_State)
	{
		case BGS_Initializing:
			// If we get to here we're ready to attach players
			VChangeState( BGS_MainMenu );
			break;

		case BGS_MainMenu:
			break;

		case BGS_LoadingGameEnvironment:
/***
			// [mrmike] This was modified a little from what you see in the book - VLoadGame() is now called from 
			//          BaseGameLogic::VChangeState()
			//
			if (!g_pApp->VLoadGame())
			{
				GCC_ERROR("The game failed to load.");
				g_pApp->AbortGame();
			}
***/
			break;

		case BGS_WaitingForPlayersToLoadEnvironment:
			if ( m_ExpectedPlayers + m_ExpectedRemotePlayers <= m_HumanGamesLoaded )
			   {
				VChangeState( BGS_SpawningPlayersActors );
			   }
			break;

		case BGS_SpawningPlayersActors:
			VChangeState( BGS_Running );
			break;

		case BGS_WaitingForPlayers:
			if (m_ExpectedPlayers + m_ExpectedRemotePlayers == m_HumanPlayersAttached ) 
			{
				// The server sends us the level name as a part of the login message. 
				// We have to wait until it arrives before loading the level
			//	if (!g_pApp->m_Options.m_Level.empty())
			//	{
			//		VChangeState(BGS_LoadingGameEnvironment);
			//	}
			}
			break;

		case BGS_Running:
			m_pProcessManager->UpdateProcesses( deltaMilliseconds );
			break;

		default:
			ENG_ERROR("Unrecognized state.");
	   }

    // update all game views
    for ( GameViewList::iterator it = m_gameViews.begin(); it != m_gameViews.end(); ++it )
	   {
		(*it)->VOnUpdate( deltaMilliseconds );
	   }

    // update game actors
    for (ActorMap::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it)
      {
      it->second->Update(deltaMilliseconds);
      }

   }

void BaseGameLogic::VChangeState( BaseGameState newState )
   {
   if ( newState == BGS_WaitingForPlayers )
	   {
		// Get rid of the Main Menu...
		m_gameViews.pop_front();

		// Note: Split screen support would require this to change!
		m_ExpectedPlayers = 1;
      m_ExpectedAI = g_pApp->m_EngineOptions.m_numAIs;

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
		if ( !g_pApp->VLoadGame() )
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
bool BaseGameLogic::VLoadGame( const char* levelResource )
{
   /*
    // Grab the root XML node
    TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(levelResource);
    if (!pRoot)
    {
        GCC_ERROR("Failed to find level resource file: " + std::string(levelResource));
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
        Resource resource(preLoadScript);
        shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);  // this actually loads the XML file from the zip file
    }

    // load all initial actors
    TiXmlElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
    if (pActorsNode)
    {
        for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
        {
            const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode);
			if (pActor)
			{
				// fire an event letting everyone else know that we created a new actor
				shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId()));
				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
        }
    }

    // initialize all human views
    for (auto it = m_gameViews.begin(); it != m_gameViews.end(); ++it)
    {
        shared_ptr<IGameView> pView = *it;
        if (pView->VGetType() == GameView_Human)
        {
            shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(pView);
            pHumanView->LoadGame(pRoot);
        }
    }

    // register script events from the engine
	//   [mrmike] this was moved to the constructor post-press, since this function can be called when new levels are loaded by the game or editor
    // RegisterEngineScriptEvents();

    // call the delegate load function
    if (!VLoadGameDelegate(pRoot))
        return false;  // no error message here because it's assumed VLoadGameDelegate() kicked out the error

    // load the post-load script if there is one
    if (postLoadScript)
    {
        Resource resource(postLoadScript);
        shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);  // this actually loads the XML file from the zip file
    }

	// trigger the Environment Loaded Game event - only then can player actors and AI be spawned!
	if (m_bProxy)
	{
		shared_ptr<EvtData_Remote_Environment_Loaded> pNewGameEvent(GCC_NEW EvtData_Remote_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
	else
	{
		shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(GCC_NEW EvtData_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
   */
    return true;
}


ActorFactory* BaseGameLogic::VCreateActorFactory( void )
   {
   return ENG_NEW ActorFactory;
   }