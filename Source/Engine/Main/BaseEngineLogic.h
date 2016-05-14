#pragma once
/*!
 * \file BaseEngineLogic.h
 * \date 2016/05/14 21:31
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

class BaseEngineLogic : public IGameLogic
   {
	friend class EngineApp;						// This is only to gain access to the view list

   public:
   
	   BaseEngineLogic();
	   virtual ~BaseEngineLogic();
      
      bool Init(void);
      virtual void VAddView( shared_ptr<IGameView> pView );

      ActorId GetNewActorID( void ) { return ++m_LastActorId; }

      std::string BaseEngineLogic::GetActorXml( const ActorId id );
      virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);  // [rez] note: don't store this strong pointer outside of this class
      virtual void VDestroyActor(const ActorId actorId);
      virtual WeakActorPtr VGetActor(const ActorId actorId);
	   virtual void VModifyActor(const ActorId actorId, TiXmlElement *overrides);
	   virtual void VMoveActor(const ActorId id, Mat4x4 const &mat) {}

      /**
       * @brief this function is called by EngineApp::OnUpdateGame after EventManger is updated
       *     state order->BGS_Initializing (constructor)->BGS_MainMenu (VOnUpdate)->BGS_WaitingForPlayers (RequestStartGameDelegate by GUI event)
       *     ->BGS_LoadingGameEnvironment (VOnUpdate)->BGS_WaitingForPlayersToLoadEnvironment (VChangeState)->BGS_SpawningPlayersActors (VOnUpdate)
       *     ->BGS_Running (VOnUpdate)
       * @param  time float time
       * @param  elapsedTime float elapsedTime
       * @return void
       */
      virtual void VOnUpdate( float time, float elapsedTime );

      virtual void VChangeState( BaseGameState newState );
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
	   ActorMap m_Actors;
	   ActorId m_LastActorId;
      ActorFactory* m_pActorFactory;

	   ViewList m_ViewList;						// views that are attached to our game
     

   };


