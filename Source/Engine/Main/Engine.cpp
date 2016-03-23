////////////////////////////////////////////////////////////////////////////////
// Filename: Engine.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "..\UserInterface\HumanView.h"


EngineApp *g_pApp = NULL;

EngineApp::EngineApp( void )
   {
   g_pApp = this;
   m_pGame = NULL;

   m_bQuitting = false;
   m_bQuitRequested = false;
   m_bIsRunning = false;

   m_screenSize = Point(0,0);
   m_window = NULL;
   
   }

EngineApp::~EngineApp( void )
   {

   }

HWND EngineApp::GetHwnd( void )
   { 
   SDL_SysWMinfo info;
   // initialize info structure with SDL version info
   SDL_VERSION( &info.version ); 

   // the call returns true on success
   if( SDL_GetWindowWMInfo( m_window, &info ) ) 
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
   // Initiate window & SDL
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

   SDL_GLContext glContext = SDL_GL_CreateContext( m_window );
   if( !glContext )
      {
      ENG_ERROR( SDL_GetError() );
      }
   
   // Needed for core profile
   glewExperimental = true; 
   GLenum error = glewInit();

   if( error != GLEW_OK )
      {
      ENG_ERROR( reinterpret_cast<const char *>( gluErrorString( error ) ) );
      }
   // set two buffer for rendering
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

   //--------------------------------- 
   // Create window
   //--------------------------------- 

   // initialize the directory location you can store save game files
	_tcscpy_s( m_saveGameDirectory, GetSaveGameDirectory( GetHwnd(), VGetGameAppDirectory() ) );
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

Uint32 EngineApp::GetWindowState( void )
   {
   return SDL_GetWindowFlags( m_window );
   }

void EngineApp::MsgProc( void )
   {
   SDL_Event event;
   if( SDL_PollEvent( &event ) )
      {
      switch( event.type )
         {
         case SDL_WINDOWEVENT:
            
         break;

         // To do: I may have to filter it out and process it independtly to prevent SDL from
         // Close the window directly...
         case SDL_QUIT:
            // Eat it if already servicing a close
			   if ( m_bQuitRequested )
               {
               return;
               }
            OnClose();
            m_bQuitRequested = true;
            m_bQuitting = true;
			   return;
            /*
            // remenber to process additional messagebox
            if ( MessageBox::Ask(QUESTION_QUIT_GAME) == IDNO )
				   {
					   // Bail - quit aborted
					
					   // Reset quit requested flag
					   m_bQuitRequested = false;
					
					   return;
				      }
			      }*/
           
			   
         break;

         case SDL_KEYDOWN:
         break;

         }// Switch
      }// If poll event exist
   }// Function MsgProc

BaseGameLogic *EngineApp::VCreateGameAndView( void )
   {
   m_pGame = ENG_NEW BaseGameLogic();
   m_pGame->Init();

	shared_ptr<IGameView> menuView(ENG_NEW HumanView());
	m_pGame->VAddView(menuView);

	return m_pGame;
   }

void EngineApp::MainLoop( void )
   {
   while( !m_bQuitRequested  )
      {
      MsgProc();

      OnUpdateGame( 0.0f, 0.0f, NULL );
       
      }
   }

void EngineApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  )
   {
	if ( m_bQuitting )
	   {
      SDL_Event event;
      event.type = SDL_QUIT;
      SDL_PushEvent(&event);
	   }

	   if (m_pGame)
	   {
     //   IEventManager::Get()->VUpdate(20); // allow event queue to process for up to 20 ms
		//g_pApp->m_pGame->VOnUpdate(float(fTime), fElapsedTime);
	   }
   }

void EngineApp::FlashWhileMinized( void )
   {
 
   if( !GetHwnd() )
      {
      return;
      }
   // If the window is minized
   if( GetWindowState() & SDL_WINDOW_MINIMIZED )
      {
      DWORD now = timeGetTime();
      DWORD then = now;
      MSG msg;
      FlashWindow( GetHwnd(), true );

      while( true )
         {
         // PeekMessage, 0, 0 means read all of the messages
         // PeekMessage() gets a message from your application's message queue.   returned to you for processing. If there is no message, 
         // Any time the user moves the mouse, types on the keyboard, clicks on your window's menu, or does any number
         // of other things, messages are generated by the system and entered into your program's message queue.
         if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
            // To filter out WM_SYSCOMMAND or multiple alt + f4 
            if( msg.message != WM_SYSCOMMAND || msg.wParam != SC_CLOSE )
               {
               // Translates virtual-key messages into character messages. 
               // The character messages are posted to the calling thread's message queue,
               // to be read the next time the thread calls the GetMessage or PeekMessage function.
               TranslateMessage( &msg );
               // Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved by the GetMessage function.
               // DispatchMessage() sends the message out to the window that the message was sent to.
               // This could be our main window or it could be another one, or a control,
               // and in some cases a window that was created behind the scenes by the sytem or another program.
               DispatchMessage( &msg );
               }
            // Not minimized anymore, flash for once and break
            if( !( GetWindowState() & SDL_WINDOW_MINIMIZED ) )
               {
               FlashWindow( GetHwnd(), true );
               break;
               }
            
            }
         else // The window has no upcoming message, keep flashing 
            {
            now = timeGetTime();
            DWORD timeElapsed = now > then ? ( now - then ) : ( then - now );
            if( timeElapsed > 1000 )
               {
               then = now;
               FlashWindow( GetHwnd(), true );
               }
            }
         }
      }

   }

void EngineApp::OnClose()
{
	// release all the game systems in reverse order from which they were created
   ENG_LOG( "Test", "On close" );
	SAFE_DELETE( m_pGame );
   SDL_DestroyWindow( m_window );
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
