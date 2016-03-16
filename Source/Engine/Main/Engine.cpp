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
   m_bQuitRequested = false;
   m_bIsRunning = false;

   m_screenSize = Point(0,0);
   m_window = NULL;
   
   }

EngineApp::~EngineApp()
   {

   }

HWND EngineApp::GetHwnd( SDL_Window* window )
   { 
   SDL_SysWMinfo info;
   // initialize info structure with SDL version info
   SDL_VERSION( &info.version ); 

   // the call returns true on success
   if( SDL_GetWindowWMInfo( window, &info ) ) 
      { 
      return info.info.win.window;
      }
   else
      {
      return NULL;
      }
   }

bool EngineApp::InitInstance( SDL_Window* window, int screenWidth, int screenHeight )
   {

   // Check for existing instance of the same window
	// Not sure if this feature is working normally.... 
   #ifndef _DEBUG
	// Note - it can be really useful to debug network code to have
	// more than one instance of the game up at one time - so
	// feel free to comment these lines in or out as you wish!
   if (!IsOnlyInstance( VGetGameTitle() ) )
      {
      return false;
      }
   #endif

   SetCursor( NULL );
   
   //--------------------------------- 
   // Check system requirements
   //--------------------------------- 
   bool resourceCheck = false;
	while (!resourceCheck)
	   {
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;
		if (!CheckStorage(diskSpace))
         {
			return false;
         }
		const DWORD minCpuSpeed = 1300;			// 1.3Ghz
		DWORD thisCPU = ReadCPUSpeed();
		if (thisCPU < minCpuSpeed)
		   {
			ENG_ERROR("GetCPUSpeed reports CPU is too slow for this game.");
			return false;
		   }

		resourceCheck = true;
	   }
   //--------------------------------- 
   // Check system requirements
   //--------------------------------- 

   //--------------------------------- 
   // Initiate window
   //--------------------------------- 
   if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
      {
      ENG_ERROR( SDL_GetError() );
      return false;
      }

   CHAR charTitle[100];
   if(  GenericToAnsiCch( charTitle, VGetGameTitle(),  strlen( charTitle ) ) != S_OK )
      {
      ENG_ERROR( "Game title translation failed" );
      }
   if( !window )
      {
      m_window = SDL_CreateWindow( charTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth,screenHeight, SDL_WINDOW_OPENGL );
      }
   else
      {
      m_window = window;
      }
   
   if ( !m_window ) 
      {
      ENG_ERROR( SDL_GetError() );
      return false;
      }

	m_screenSize = Point( screenWidth, screenHeight );
   //--------------------------------- 
   // Create window
   //--------------------------------- 

   // initialize the directory location you can store save game files
	_tcscpy_s( m_saveGameDirectory, GetSaveGameDirectory( GetHwnd( m_window ), VGetGameAppDirectory() ) );

   //--------------------------------- 
   // Create game & view
   //--------------------------------- 
   m_pGame = VCreateGameAndView();
   if (!m_pGame)
      {
      return false;
      }
   //--------------------------------- 
   // Create game & view
   //--------------------------------- 
   m_bIsRunning = true;

   return true;
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

BaseGameLogic *EngineApp::VCreateGameAndView()
   {
   m_pGame = ENG_NEW BaseGameLogic();
   m_pGame->Init();

	shared_ptr<IGameView> menuView(ENG_NEW HumanView());
	m_pGame->VAddView(menuView);

	return m_pGame;
   }

void EngineApp::MainLoop()
   {
   while( !g_pApp->m_bQuitRequested  )
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
