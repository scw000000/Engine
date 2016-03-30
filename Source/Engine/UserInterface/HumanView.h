#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.h
////////////////////////////////////////////////////////////////////////////////


typedef std::list< shared_ptr< IScreenElement > > ScreenElementList;

class HumanView : public IGameView
   {
	friend class EngineApp;

   public:
      HumanView();
      virtual ~HumanView();
      virtual void VOnRender( double fTime, float fElapsedTime );
	   virtual GameViewType VGetType() { return GameView_Human; }
	   virtual GameViewId VGetId() const { return m_ViewId; }

	   virtual void VOnAttach( GameViewId vid, ActorId aid )
         {
   		m_ViewId = vid; 
	   	m_ActorId = aid;
	      }
      virtual int VOnRestore();
      virtual int VOnLostDevice();
      virtual int VOnMsgProc( SDL_Event event ); // process the OS event
	   virtual void VOnUpdate(const int deltaMilliseconds );
   
	   // Virtual methods to control the layering of interface elements
	   virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	   virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);

      ProcessManager* GetProcessManager() { return m_pProcessManager; }
	   //void TogglePause(bool active);

	   //HumanView(shared_ptr<IRenderer> renderer);

   public:
      ScreenElementList m_ScreenElements;

   protected:
	   GameViewId m_ViewId;
	   ActorId m_ActorId;
      Uint64 m_currTick; 
      Uint64 m_lastDraw; // last time the game rended
      bool m_runFullSpeed; // if the view should ignore max rendering frequency setting
      ProcessManager *m_pProcessManager; // this Processmanager is for things like button animations
      BaseGameState m_BaseGameState;					// Added post-press - what is the current game state
   private:

   };


