////////////////////////////////////////////////////////////////////////////////
// Filename: Engine.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "..\UserInterface\HumanView.h"

EngineApp *g_pApp = NULL;

EngineApp::EngineApp()
   {
   g_pApp = this;
   m_pGame = NULL;
   m_bQuitting = false;
   m_bReadyToQuit = false;
   m_window = NULL;
   
   }

EngineApp::~EngineApp()
   {

   }

void EngineApp::MsgProc()
   {
   SDL_Event event;
   if( SDL_PollEvent( &event ) )
      {
      switch( event.type )
         {
            case SDL_QUIT:
            g_pApp->m_bQuitting = true;
            g_pApp->OnClose();
            break;

         }
      }
   }

void EngineApp::Run()
   {
   InitSystems();
   EngineLoop();
   }

BaseGameLogic *EngineApp::VCreateGameAndView()
   {
   m_pGame = ENG_NEW BaseGameLogic();
   m_pGame->Init();

	shared_ptr<IGameView> menuView(ENG_NEW HumanView());
	m_pGame->VAddView(menuView);

	return m_pGame;
   }

void EngineApp::InitSystems()
   {
   SDL_Init(SDL_INIT_EVERYTHING);
   m_pGame = VCreateGameAndView();
   m_window = SDL_CreateWindow( "Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_EngineOptions.m_ScreenWidth, m_EngineOptions.m_ScreenHeight, SDL_WINDOW_OPENGL );
   
   }

void EngineApp::EngineLoop()
   {
   while( !g_pApp->m_bReadyToQuit  )
      {
      MsgProc();
      }
   }

void EngineApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  )
   {
	if ( g_pApp->m_bQuitting )
	   {
      SDL_Event event;
      event.type = SDL_QUIT;
      SDL_PushEvent(&event);
	   }

	   if (g_pApp->m_pGame)
	   {
     //   IEventManager::Get()->VUpdate(20); // allow event queue to process for up to 20 ms
		//g_pApp->m_pGame->VOnUpdate(float(fTime), fElapsedTime);
	   }
   }

void EngineApp::OnClose()
{
	// release all the game systems in reverse order from which they were created

	SAFE_DELETE(m_pGame);
   m_bReadyToQuit = true;
   /*
	DestroyWindow(GetHwnd());

	VDestroyNetworkEventForwarder();

	SAFE_DELETE(m_pBaseSocketManager);

	SAFE_DELETE(m_pEventManager);

    BaseScriptComponent::UnregisterScriptFunctions();
    ScriptExports::Unregister();
    LuaStateManager::Destroy();

	SAFE_DELETE(m_ResCache);
   */
	return;
}