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

void EngineApp::Init( void )
   {
   if( !g_pApp )
      {
      ENG_NEW EngineApp();
      }
   }

void EngineApp::InitInstance( void )
   {
   SDL_Init(SDL_INIT_EVERYTHING);
   m_pGame = VCreateGameAndView();
   m_window = SDL_CreateWindow( "Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_EngineOptions.m_ScreenSize.GetX(), m_EngineOptions.m_ScreenSize.GetY(), SDL_WINDOW_OPENGL );
   
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

            case SDL_KEYDOWN:
            g_pApp->m_bQuitting = true;
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
   
   }

void EngineApp::EngineLoop()
   {
   while( !g_pApp->m_bReadyToQuit  )
      {
      MsgProc();
      OnUpdateGame( 0.0f, 0.0f, NULL );
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
