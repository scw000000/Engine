#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: BasicGameLogic.h
////////////////////////////////////////////////////////////////////////////////


#include "../Mainloop/ProcessManager.h"
#include "../Actors/Actor.h"

class ActorFactory;

enum BaseGameState
   {
   BGS_Invalid,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_WaitingForPlayers,
	BGS_LoadingGameEnvironment,
	BGS_WaitingForPlayersToLoadEnvironment,
	BGS_SpawningPlayersActors,
	BGS_Running
   };

typedef std::map<ActorId, StrongActorPtr> ActorMap;

class BaseGameLogic : public IGameLogic
   {
	friend class EngineApp;						// This is only to gain access to the view list

   public:
   
	   BaseGameLogic();
	   virtual ~BaseGameLogic();
      
      bool Init(void);
      virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId=INVALID_ACTOR_ID);

      ActorId GetNewActorID( void ) { return ++m_LastActorId; }

      std::string BaseGameLogic::GetActorXml( const ActorId id );
      virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);  // [rez] note: don't store this strong pointer outside of this class
      virtual void VDestroyActor(const ActorId actorId);
      virtual WeakActorPtr VGetActor(const ActorId actorId);
	   virtual void VModifyActor(const ActorId actorId, TiXmlElement *overrides);
	   virtual void VMoveActor(const ActorId id, Mat4x4 const &mat) {}

      virtual void VOnUpdate( float time, float elapsedTime );

      virtual void VChangeState(BaseGameState newState);
	   const BaseGameState GetState() const { return m_State; }

      virtual bool VLoadGame( const char* levelResource ) override;  

      void AttachProcess(StrongProcessPtr pProcess) { if (m_pProcessManager) {m_pProcessManager->AttachProcess(pProcess);} }

   protected: 
      virtual ActorFactory* VCreateActorFactory(void);
	   
   protected:
      float m_Lifetime;
      BaseGameState m_State;	// game state: loading, running, etc.

      ProcessManager* m_pProcessManager;				// a game logic entity
	   //GCCRandom m_random;								// our RNG
	   ActorMap m_actors;
	   ActorId m_LastActorId;
      ActorFactory* m_pActorFactory;
   							
	   int m_ExpectedPlayers;							// how many local human players
	   int m_ExpectedRemotePlayers;					// expected remote human players
	   int m_ExpectedAI;								// how many AI players
	   int m_HumanPlayersAttached;
	   int m_AIPlayersAttached;
	   int m_HumanGamesLoaded;
	   GameViewList m_gameViews;						// views that are attached to our game
     

   };


