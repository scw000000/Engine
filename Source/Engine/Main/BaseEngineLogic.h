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
#include "../UserInterface/GUIManager.h"
#include "../Actors/Actor.h"

class ActorFactory;

enum BaseEngineState
   {
   BES_Invalid,
   BES_Ready,
   BES_Pause,
   BES_Running
   };

typedef std::string Level;

class LevelManager 
   {
   public:
      Level GetCurrentLevel() const { return pCurrentLevelRes->m_Name; }
      void Init( void );

   protected:
      shared_ptr<Resource> pCurrentLevelRes;
   };

typedef std::map<ActorId, StrongActorPtr> ActorMap;

class BaseEngineLogic : public IEngineLogic
   {
	friend class EngineApp;						// This is only to gain access to the view list

   public:
   
	   BaseEngineLogic( shared_ptr<IRenderer> pRenderer );
	   virtual ~BaseEngineLogic();
      
      bool Init(void);
      virtual void VAddView( shared_ptr<IView> pView ) override;

      ActorId GetNewActorID( void ) { return ++m_LastActorId; }
      std::string BaseEngineLogic::GetActorXml( ActorId id );
      virtual StrongActorPtr VCreateActor( const Resource& actorRes,
                                           TransformPtr pTransform = NULL,
                                           ActorId serversActorId = INVALID_ACTOR_ID ) override;

      //virtual StrongActorPtr VCreateActorFromOverrides( const Resource& overrides,
      //                                     TransformPtr pTransform = NULL,
      //                                     ActorId serversActorId = INVALID_ACTOR_ID ) override;  // [rez] note: don't store this strong pointer outside of this class
      //virtual StrongActorPtr VCreateActorFromClass( const Resource& actorClassRes,
      //                                              TransformPtr pTransform = NULL,
      //                                              ActorId serversActorId = INVALID_ACTOR_ID ) override;
      virtual WeakActorPtr VGetActor( ActorId actorId ) override;
      virtual void VDestroyActor( ActorId actorId ) override;
      virtual void VModifyActor( ActorId actorId, TiXmlElement *overrides) override ;
      virtual void VMoveActor( ActorId id, Mat4x4 const &mat) override {}
      virtual void VSetActorUpdate( bool isUpdatable ) override;

      virtual bool VLoadLevel( void ) override;
      virtual int VOnRestore( void ) override;
      
      virtual void VOnMsgProc( SDL_Event event ) override;
      
      virtual void VSetWorldUpdate( bool isUpdatable ) override;
      /**
       * @brief this function is called by EngineApp::OnUpdateGame after EventManger is updated
       *     state order->BGS_Initializing (constructor)->BGS_MainMenu (VOnUpdate)->BGS_WaitingForPlayers (RequestStartGameDelegate by GUI event)
       *     ->BGS_LoadingGameEnvironment (VOnUpdate)->BGS_WaitingForPlayersToLoadEnvironment (VChangeState)->BGS_SpawningPlayersActors (VOnUpdate)
       *     ->BGS_Running (VOnUpdate)
       * @param  time float time
       * @param  elapsedTime float elapsedTime
       * @return void
       */
      virtual void VOnUpdate( float time, float elapsedTime ) override;
      virtual void VOnRender( double fTime, float fElapsedTime ) override;
      virtual void VRenderDiagnostics( void ) const override;

      virtual void VOnFileDrop( const char* filePath, const Point& dropLocation ) override { }

      void AttachProcess(StrongProcessPtr pProcess) { if (m_pProcessManager) {m_pProcessManager->AttachProcess(pProcess);} }
      virtual void VClearWorld( void ) override;
      virtual void VStartAndPause( void ) override;
      virtual void VStop( void ) override;
      virtual void VSetIsSimulating( bool isOn ) override;
      virtual void VGameStart( void ) override;
      virtual void VSaveAllActors() override {}
      virtual void VSaveActor( ActorId id ) override { }
      TiXmlElement* VGenerateWorldXML( void ) override { return NULL; };
      virtual void VSaveWorld( void ) override {  };

   public:
      shared_ptr<Scene> m_pWrold;
      shared_ptr<GUIManager> m_pGUIManager;
      LevelManager* m_pLevelManager;

   protected: 
      void ReInitWorld( void );
      void SetNextEngineState( BaseEngineState nxtState );

   protected:
	   ActorId m_LastActorId;
	   ActorMap m_Actors;
	   ViewList m_ViewList;						// views that are attached to our game

      //GCCRandom m_random;								// our RNG
      ActorFactory* m_pActorFactory;
      ProcessManager* m_pProcessManager;				// a game logic entity
      bool m_EnableActorUpdate;
      bool m_EnableWorldUpdate;
      bool m_RenderDiagnostics;
      bool m_HasStarted;
      float m_Lifetime;
      shared_ptr<IRenderer> m_pRenderer;
      BaseEngineState m_EngineState;

   };


