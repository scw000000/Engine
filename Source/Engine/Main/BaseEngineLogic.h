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

typedef std::string Level;

class LevelManager 
   {
   public:
      const std::vector<Level> &GetLevels( ) const { return m_Levels; }
      Level GetCurrentLevel( ) const { return m_Levels[m_CurrentLevel]; }
      void Init( const std::string& levelDir );
      void ResetCurrentLevel( void );

   protected:
      std::string m_LevelDirectory;
      std::vector<Level> m_Levels;
      int m_CurrentLevel;
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
      virtual StrongActorPtr VCreateActorFromOverrides( const char* overrides,
                                           TransformPtr pTransform = NULL,
                                           ActorId serversActorId = INVALID_ACTOR_ID ) override;  // [rez] note: don't store this strong pointer outside of this class
      virtual StrongActorPtr VCreateActorFromClass( const char* classFilePath,
                                                    TransformPtr pTransform = NULL,
                                                    ActorId serversActorId = INVALID_ACTOR_ID ) override;
      virtual WeakActorPtr VGetActor( ActorId actorId ) override;
      virtual void VDestroyActor( ActorId actorId ) override;
      virtual void VModifyActor( ActorId actorId, TiXmlElement *overrides) override ;
      virtual void VMoveActor( ActorId id, Mat4x4 const &mat) override {}
      virtual void VSetActorUpdate( bool isUpdatable ) override;

      virtual bool VLoadLevel( const std::string& levelResource ) override;
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
      virtual void VSetIsRunning( bool isOn ) override;
      virtual bool VGetIsRunning( void ) const override { return m_IsRunning; }
      virtual void VGameStart( void ) override;
      virtual bool VGetHasStarted( void ) const override { return m_HasStarted; };
      virtual void ReInitWorld( void ) override;

   public:
      shared_ptr<Scene> m_pWrold;
      shared_ptr<GUIManager> m_pGUIManager;
      LevelManager* m_pLevelManager;

   protected: 
	   
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
      bool m_IsRunning;
      float m_Lifetime;
      shared_ptr<IRenderer> m_pRenderer;
     

   };


