////////////////////////////////////////////////////////////////////////////////
// Filename: Engine.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "..\UserInterface\HumanView.h"
#include "..\UserInterface/GUIManager.h"
#include "..\ResourceCache\XmlResource.h"
#include "..\Event\EventManager.h"
#include "..\Graphics\OpenGLRenderer.h"


EngineApp *g_pApp = NULL;

EngineApp::EngineApp( void )
   {
   g_pApp = this;
   m_pGame = NULL;

   m_bQuitting = false;
   m_bQuitRequested = false;
   m_bIsRunning = false;

   m_screenSize = Point(0,0);
   m_pWindow = NULL;
   m_ShutDownEventType = 0;
   m_pEventManager = NULL;
   
   }

EngineApp::~EngineApp( void )
   {

   }

Renderer EngineApp::GetRendererImpl( void )
   {
   GLint majVer;
   GLint minVer;
   glGetIntegerv( GL_MINOR_VERSION, &majVer );
   glGetIntegerv( GL_MAJOR_VERSION, &minVer );
   return Renderer_OpenGL;
   }

HWND EngineApp::GetHwnd( void )
   { 
   SDL_SysWMinfo info;
   // initialize info structure with SDL version info
   SDL_VERSION( &info.version ); 

   // the call returns true on success
   if( SDL_GetWindowWMInfo( m_pWindow, &info ) ) 
      { 
      return info.info.win.window;
      }
   else
      {
      ENG_ERROR( "Error: Target Platform is not Windows" );
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
   
   //--------------------------------- 
   // Check system requirements
   //--------------------------------- 
   bool resourceCheck = false;
	while (!resourceCheck)
	   {
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;
		if ( !CheckStorage(diskSpace) )
         {
			return false;
         }
		const DWORD minCpuSpeed = 1300;			// 1.3Ghz
		DWORD thisCPU = ReadCPUSpeed();
		if ( thisCPU < minCpuSpeed )
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
   //  Initialize ResCache, all asseets are within a zip file
   //--------------------------------- 
	IResourceFile *zipFile = ENG_NEW ResourceZipFile( L"Assets.zip" );

	m_pResCache = ENG_NEW ResCache( 50, zipFile );

	if ( !m_pResCache->Init() )
	   {
      ENG_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	   }

 //  extern shared_ptr<IResourceLoader> CreateWAVResourceLoader();
//	extern shared_ptr<IResourceLoader> CreateOGGResourceLoader();
	//extern shared_ptr<IResourceLoader> CreateDDSResourceLoader();
	//extern shared_ptr<IResourceLoader> CreateJPGResourceLoader();
   extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();
   //extern shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();
   //extern shared_ptr<IResourceLoader> CreateScriptResourceLoader();

	// Note - register these in order from least specific to most specific! They get pushed onto a list.
	// RegisterLoader is discussed in Chapter 5, page 142

   m_pResCache->RegisterLoader(CreateXmlResourceLoader());

   if( !LoadStrings("English") )
	   {
      ENG_ERROR("Failed to load strings");
		return false;
	   }
   //--------------------------------- 
   //  Initialize ResCache
   //--------------------------------- 

   //--------------------------------- 
   //  Initialize EventManager
   //--------------------------------- 

   // Set as global EventManager
   m_pEventManager = ENG_NEW EventManager( "Engine Event Manager", true );
	if ( !m_pEventManager )
	   {
		ENG_ERROR( "Failed to create EventManager." );
		return false;
	   }
   //--------------------------------- 
   //  Initialize EventManager
   //--------------------------------- 


   //--------------------------------- 
   // Initiate window & SDL, glew
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
      m_pWindow = SDL_CreateWindow( charTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth,screenHeight, SDL_WINDOW_OPENGL );
      if ( !m_pWindow ) 
         {
         ENG_ERROR( SDL_GetError() );
         return false;
         }
      }
   else
      {
      m_pWindow = window;
      }

	m_screenSize = Point( screenWidth, screenHeight );

   SDL_ShowCursor( SDL_DISABLE );

   // setup opengl rendering context
   SDL_GLContext glContext = SDL_GL_CreateContext( m_pWindow );
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

   m_ShutDownEventType = SDL_RegisterEvents( 1 );
   ENG_ASSERT( m_ShutDownEventType != ((Uint32)-1) );
   //--------------------------------- 
   // Initiate window & SDL, glew
   //--------------------------------- 

   //--------------------------------- 
   // Set Renderer
   //--------------------------------- 
    if( GetRendererImpl() == Renderer_OpenGL )
      {
      m_pRenderer = shared_ptr<IRenderer>( ENG_NEW OpenGLRenderer() );
      }
   else
      {
      ENG_ERROR( "Not supported renderer type" );
      }
   m_pRenderer->VSetBackgroundColor( g_Black );
   m_pRenderer->VOnRestore();
   //--------------------------------- 
   // Set Renderer
   //--------------------------------- 


   // Start global timer
   GetGlobalTimer()->Reset();

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

bool EngineApp::VLoadGame(void)
   {
   // Read the game options and see what the current game
   // needs to be - all of the game graphics are initialized by now, too...
	return m_pGame->VLoadGame( m_EngineOptions.m_Level.c_str() );
   }

Uint32 EngineApp::GetWindowState( void )
   {
   return SDL_GetWindowFlags( m_pWindow );
   }

void EngineApp::MsgProc( void )
   {

   SDL_Event event;
   HumanView*  pHumanView;
   if( SDL_PollEvent( &event ) )
      {
      if( event.type == m_ShutDownEventType )
         {
         pHumanView = GetHumanView();
         if( pHumanView && pHumanView->HasModalDialog() )
            {
            // force the dialog to exit
            ENG_ASSERT( PushUserEvent( pHumanView->GetModalEventType(), g_QuitNoPrompt ) >= 0 );
            // force next dialog( if it exists ) to exit
            ENG_ASSERT( PushUserEvent( m_ShutDownEventType, 0 ) >= 0 );
            }
         else // this is triggered by last dialog event
            {
            OnClose();
            }
         return;
         }
      switch( event.type )
         {
         case SDL_WINDOWEVENT:
            
         break;
         case SDL_QUIT: // this case is triggered by hitting 'x' at the window
            if( m_bQuitRequested )
               {
               return;
               }

            m_bQuitRequested = true;
            pHumanView = GetHumanView();
            // remenber to process additional messagebox
            if ( pHumanView )
				   {
               if( pHumanView->Ask( QUESTION_QUIT_GAME ) == IDNO )
                  {
                  // We've cleared the dialog, Reset quit requested flag
					   m_bQuitRequested = false;
					   return; 
                  }
               else
                  {
                  PushUserEvent( m_ShutDownEventType, 0 );
                  }
			      }
            m_bQuitting = true;
            break;
         case SDL_KEYDOWN:
         case SDL_KEYUP:
         case SDL_TEXTEDITING:
         case SDL_TEXTINPUT:
         case SDL_KEYMAPCHANGED:
         case SDL_MOUSEMOTION:
         case SDL_MOUSEBUTTONDOWN:
         case SDL_MOUSEBUTTONUP:
         case SDL_MOUSEWHEEL:
         case SDL_JOYAXISMOTION:
         case SDL_JOYBALLMOTION:
         case SDL_JOYHATMOTION:
         case SDL_JOYBUTTONDOWN:
         case SDL_JOYBUTTONUP:
         case SDL_JOYDEVICEADDED:
         case SDL_JOYDEVICEREMOVED:
         case SDL_CONTROLLERAXISMOTION:
         case SDL_CONTROLLERBUTTONDOWN:
         case SDL_CONTROLLERBUTTONUP:
         case SDL_CONTROLLERDEVICEADDED:
         case SDL_CONTROLLERDEVICEREMOVED:
         case SDL_CONTROLLERDEVICEREMAPPED:
         // send event to all of game views
         for( auto i = m_pGame->m_gameViews.rbegin(); i != m_pGame->m_gameViews.rend(); ++i) 
		      {
			   if ( (*i)->VOnMsgProc( event ) )
				   {
				   return;
			      }
			   }
         }// Switch
      }// If poll event exist
   }// Function MsgProc

BaseGameLogic *EngineApp::VCreateGameAndView( void )
   {
   m_pGame = ENG_NEW BaseGameLogic();
   m_pGame->Init();

   // shared_ptr<IGameView> menuView;
	shared_ptr<IGameView> menuView( ENG_NEW HumanView( g_pApp->m_pRenderer ) );
	m_pGame->VAddView( menuView );
	return m_pGame;
   }

int EngineApp::Modal( shared_ptr<Dialog> pModalScreen, int defaultAnswer )
   {
   ENG_ASSERT( GetWindow() != NULL && _T("Main Window is NULL!") );
   // if the dialog poped out while the window is minimized, flash it to notify
	if ( IsIconic(GetHwnd()) )
	   {
		FlashWhileMinimized();
	   }
   HumanView*  pHumanView = GetHumanView();
   
   if( pHumanView )
      {
      Uint32 eventEndType = pHumanView->GetModalEventType();
      Sint32 code = 0;
      int result = PumpUntilMessage( eventEndType, code );
      if( code == g_QuitNoPrompt )
         {
         return defaultAnswer;
         }
      return (int) code;
      }

   return defaultAnswer;

   }

HumanView* EngineApp::GetHumanView( void )
   {
   for( auto it = m_pGame->m_gameViews.begin(); it != m_pGame->m_gameViews.end(); ++it )
      {
      if( (*it)->VGetType() == GameViewType::GameView_Human )
         {
         return static_cast<HumanView*>( &**it );
         }
      }
   return NULL;
   }

bool EngineApp::LoadStrings( std::string language )
   {
	std::string languageFile = "Strings\\";
	languageFile += language;
	languageFile += ".xml";

	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( languageFile.c_str() );
	// load failed
   if ( !pRoot )
	   {
		ENG_ERROR("Strings are missing.");
		return false;
	   }

    // Loop through each child element and load the component
    for ( TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement() )
      {
		const char *pKey = pElem->Attribute( "id" );
		const char *pText = pElem->Attribute( "value" );
		if( pKey && pText ) 
		   {
			wchar_t wideKey[64];
			wchar_t wideText[1024];
			AnsiToWideCch(wideKey, pKey, 64);
			AnsiToWideCch(wideText, pText, 1024);
			m_textResource[ std::wstring(wideKey) ] = std::wstring(wideText);
		   }
	   }
	return true;
   }

std::wstring EngineApp::GetString( std::wstring sID )
   {
	auto localizedString = m_textResource.find( sID );
	if(localizedString == m_textResource.end() )
	   {
		ENG_ASSERT(0 && "String not found!");
		return L"";
	   }
	return localizedString->second;
   }  

//
// class GameCodeApp::PumpUntilMessage			- Chapter 10, page 295
//
int EngineApp::PumpUntilMessage( Uint32& eventEnd, Sint32& code )
{
   SDL_Event event;
	int currentTime = timeGetTime();
	for ( ;; )
	   {
      SDL_PumpEvents();
		if ( SDL_PeepEvents( &event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT ) > 0 )
		   {
         // it should be the event we want
			if ( event.type == eventEnd )
			   {
				SDL_PeepEvents( &event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT );
            eventEnd = event.user.type;
            code = event.user.code;
				break;
			   }
			else
			   {
            // Default processing
				MsgProc();
			   }
		   }
		else
		   {
			// Update the game views, but nothing else! ( actor & game logic state skipped ) 
			// Remember this is a modal screen.
			if ( m_pGame )
			   {
				double fAppTime = 0.0;
            double fAbsoluteTime = 0.0;
            float  fElapasedTime = 0.0f;
            GetGlobalTimer()->GetTimeValues( &fAppTime, &fAbsoluteTime, &fElapasedTime );
				for( auto i = m_pGame->m_gameViews.begin(); i!=m_pGame->m_gameViews.end(); ++i )
				   {
					(*i)->VOnUpdate( ( unsigned long ) ( fElapasedTime * 1000.f ) );
				   }
				OnFrameRender( fAppTime, fElapasedTime );
			   }
		   }
	}
	

	return 0;
}

void EngineApp::MainLoop( void )
   {
   double fAppTime = 0.0;
   double fAbsoluteTime = 0.0;
   float  fElapasedTime = 0.0f;
   while( true  )
      {
      MsgProc();
      if( !m_bIsRunning )
         {
         break;
         }
      GetGlobalTimer()->GetTimeValues( &fAppTime, &fAbsoluteTime, &fElapasedTime );

      OnUpdateGame( fAppTime, fElapasedTime );
      
      OnFrameRender( fAppTime, fElapasedTime );

      }

   OnClose();
   }

void EngineApp::OnUpdateGame( double fTime, float fElapsedTime )
   {
	if ( m_bQuitting )
	   {
      ENG_ASSERT( PushUserEvent( m_ShutDownEventType, 0 ) );
      }

	   if (m_pGame)
	   {
      IEventManager::GetSingleton()->VUpdate(20); // allow event queue to process for up to 20 ms
		g_pApp->m_pGame->VOnUpdate( ( float ) fTime, fElapsedTime );
	   }
   }

void EngineApp::OnFrameRender( double fTime, float fElapsedTime )
   {
   // LATER: move these to renderer
   glClearDepth( 1.0 );
   // use previously setted clearColr to draw background
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   BaseGameLogic *pGame = g_pApp->m_pGame;

	for(GameViewList::iterator i=pGame->m_gameViews.begin(), end=pGame->m_gameViews.end(); i!=end; ++i)
	   {
		(*i)->VOnRender( fTime, fElapsedTime );
	   }

   SDL_GL_SwapWindow( m_pWindow );

   }

// This is a WINDOWS dedicated function
// LATER: This function doesn't work as expected, it won't "blink" on and off, and I cann't find a solution yet
void EngineApp::FlashWhileMinimized( void )
   {
#ifndef _WINDOWS
   return;
#endif

   if( !m_pWindow )
      {
      return;
      }
   HWND hwnd = GetHwnd();
   ENG_ASSERT( hwnd );
   // If the window is minized
   if( GetWindowState() & SDL_WINDOW_MINIMIZED )
      {
      GetGlobalTimer()->GetElapsedTime();
      float totalTime = 0.f;
      SDL_Event event;
      FlashWindow( hwnd, true );

      while( true )
         {
         SDL_PumpEvents();
         if( SDL_PeepEvents( &event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT ) > 0 )
            {
            // wait for processing if close or other window event is called
            if( event.type != SDL_WINDOWEVENT ||  event.type != SDL_QUIT )
               {
               MsgProc();
               }
            // Not minimized anymore, flash for once and break
            if( !( GetWindowState() & SDL_WINDOW_MINIMIZED ) )
               {
               FlashWindow( GetHwnd(), false );
               break;
               }
            }
         else // The window has no upcoming message, keep flashing 
            {
            totalTime += GetGlobalTimer()->GetElapsedTime();
            
            if( totalTime > 1.0f )
               {
               totalTime -= 1.0f;
               FlashWindow( GetHwnd(), true );
               }
            }
         }
      }

   }

int  EngineApp::PushUserEvent( Uint32 eventType, Sint32 code, void* d1, void* d2 )
   {
   SDL_Event event;
   SDL_zero( event );
   event.type = eventType;
   event.user.code = code;
   event.user.data1 = d1;
   event.user.data2 = d2;
   return SDL_PushEvent( &event );
   }

void EngineApp::OnClose()
{
   m_bIsRunning = false;
	// release all the game systems in reverse order from which they were created
   ENG_LOG( "Test", "On close" );
	SAFE_DELETE( m_pGame );
   SDL_DestroyWindow( m_pWindow );
   SDL_Quit();
   SAFE_DELETE( m_pEventManager );
   SAFE_DELETE( m_pResCache );
   /*
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
