/*!
 * \file Engine.cpp
 * \date 2016/05/14 21:58
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
#include "EngineStd.h"
#include "..\UserInterface\HumanView.h"
#include "..\UserInterface/GUIManager.h"
#include "..\ResourceCache\DevResourceFile.h"
#include "..\ResourceCache\XmlResource.h"
#include "..\Event\EventManager.h"
#include "..\Event\Events.h"
#include "..\Graphics\Renderer\MainRenderer.h"
#include "..\Graphics\Renderer\RenderManager.h"
#include "..\LuaScripting\LuaStateManager.h"
#include "..\LuaScripting\ScriptExports.h"
#include "..\Animation\AnimationClipNode.h"
#include "..\Animation\AnimationLerpNode.h"
#include "..\Animation\AnimationState.h"
#include "..\Controller\InputManager.h"
#include "SDL_image.h"



EngineApp *g_pApp = NULL;

EngineApp::EngineApp( void )
   {
   g_pApp = this;
   m_pResCache = NULL;
   m_pEngineLogic = NULL;

   m_bQuitting = false;
   m_bQuitRequested = false;
   m_bIsRunning = false;

   m_pWindow = NULL;
   m_pVideoResCache = NULL;
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
   std::stringstream  ss;
   ss << "Renderer version: OpenGL " << majVer << "." << minVer;
   ENG_LOG( "Renderer", ss.str() );
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
   //  Initialize ResCache, all assets are within a zip file or a folder
   //--------------------------------- 
   IResourceFile *pFile = NULL;
   if( m_EngineOptions.GetIsUsingDevDirectory() )
      {
      pFile = ENG_NEW DevResourceFile( DevResourceFile::Editor );
      }
   else
      {
      pFile = ENG_NEW ResourceZipFile( L"Assets.zip" );
      }
   m_pResCache = ENG_NEW ResourceCache( 50, pFile );

	if ( m_pResCache->Init() != S_OK )
	   {
      ENG_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	   }

   if( !LoadStrings("English") )
	   {
      ENG_ERROR("Failed to load strings");
		return false;
	   }
   //--------------------------------- 
   //  Initialize ResCache
   //--------------------------------- 

   //--------------------------------- 
   //  Initialize Lua scripting
   //---------------------------------

   // Rez up the Lua State manager now, and run the initial script - discussed in Chapter 5, page 144.
   if( !LuaStateManager::GetSingleton().VInit() )
      {
      ENG_ERROR( "Failed to initialize Lua" );
      return false;
      }

   ScriptExports::Register();
   
   Resource resource( m_EngineOptions.GetPreInitScriptFile() );
   // shared_ptr< ResHandle > pReshandle( ENG_NEW ResHandle( resource, 0, 0, m_pResCache ) );
   
   shared_ptr<ResHandle> pResourceHandle = m_pResCache->GetHandle( resource );
   ENG_ASSERT( pResourceHandle );

   // RegisterScriptClass< AnimationClipNode, IAnimationNode >();
   // RegisterScriptClass< AnimationLerpNode, IAnimationNode >();
   // RegisterScriptClass< AnimationState >();

   //--------------------------------- 
   //  Initialize Lua scripting
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
   
   if( !window )
      {
      if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
         {
         ENG_ERROR( SDL_GetError() );
         return false;
         }

      CHAR charTitle[ 100 ];
      if( GenericToAnsiCch( charTitle, VGetGameTitle(), strlen( charTitle ) ) != S_OK )
         {
       //  ENG_ERROR( "Game title translation failed" );
         }

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

   // Only if execute under editor can engine allow file drop 
   if( m_EngineOptions.GetEngineEnvironment() == Environment_Editor )
      {
      SDL_EventState( SDL_DROPFILE, SDL_ENABLE );
      }

   if( m_EngineOptions.GetShowMouseCursor() )
      {
      SDL_ShowCursor( SDL_ENABLE );
      }
   else
      {
      SDL_ShowCursor( SDL_DISABLE );
      }

   SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
   // setup opengl rendering context
   SDL_GLContext glContext = SDL_GL_CreateContext( m_pWindow );
   if( !glContext )
      {
      ENG_ERROR( SDL_GetError() );
      }
   
   // Needed for core profile
   glewExperimental = GL_TRUE; 

   GLenum error = glewInit();

   ENG_ASSERT( error == GLEW_OK );

   // set two buffer for rendering
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

   m_ShutDownEventType = RegisterEvent( 1 );
   //--------------------------------- 
   // Initiate window & SDL, glew
   //--------------------------------- 

   //--------------------------------- 
   // SDL_Image
   //--------------------------------- 
   int initFlags= IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
   int initted = IMG_Init( initFlags );

   if( ( initted & initFlags ) != initFlags ) 
      {
      ENG_ERROR( IMG_GetError() );
      }
   //--------------------------------- 
   // SDL_Image
   //--------------------------------- 

   //--------------------------------- 
   // Set Renderer
   //--------------------------------- 
   if( GetRendererImpl() == Renderer_OpenGL )
      {
     // m_pRenderer = shared_ptr< IMainRenderer >( ENG_NEW MainRenderer() );
      m_pRenderManager = shared_ptr< IRenderManager >( ENG_NEW OpenGLRenderManager() );
      }
   else
      {
      ENG_ERROR( "Not supported renderer type" );
      }
   glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc( GL_LESS ); 
   // Cull triangles which normal is not towards the camera
   glEnable( GL_CULL_FACE );
   m_pRenderManager->VInit();
   m_pRenderManager->VGetMainRenderer().VSetBackgroundColor( g_Black );
  // m_pRenderer->VOnRestore();
   //--------------------------------- 
   // Set Renderer
   //--------------------------------- 

   //--------------------------------- 
   // Init video resource cache
   //--------------------------------- 
   m_pVideoResCache = ENG_NEW VideoResourceCache( 0, m_pResCache );
   if( m_pVideoResCache->Init() != S_OK )
      {
      ENG_ERROR( "Failed to initialize video resource cache! " );
      return false;
      }

   //--------------------------------- 
   // Init video resource cache
   //--------------------------------- 
   // Start global timer
   GetGlobalTimer()->Reset();

   // initialize the directory location you can store save game files
	_tcscpy_s( m_saveGameDirectory, GetSaveGameDirectory( GetHwnd(), VGetGameAppDirectory() ) );
   
   //--------------------------------- 
   // Create game & view
   //--------------------------------- 
   m_pEngineLogic = VCreateLogic();
   if (!m_pEngineLogic)
      {
      return false;
      }
   //--------------------------------- 
   // Create game & view
   //--------------------------------- 
   //--------------------------------- 
   // Input Manager
   //--------------------------------- 
   int result= SInputManager::GetSingleton().Init( m_pEngineLogic->m_pGUIManager.get() );
   ENG_ASSERT( result == S_OK );

   SDL_SetEventFilter( EventFilter, nullptr );
   //--------------------------------- 
   // Input Manager
   //--------------------------------- 

   m_bIsRunning = true;

   return true;
   }

Uint32 EngineApp::GetWindowState( void )
   {
   return SDL_GetWindowFlags( m_pWindow );
   }

Uint32 EngineApp::RegisterEvent( int eventNum )
   {
   Uint32 eventID = SDL_RegisterEvents( eventNum );
   ENG_ASSERT( eventID != ( ( Uint32 ) -1 ) );
   return eventID;
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
         if( pHumanView && pHumanView->HasModalWindow() )
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
            // remember to process additional messagebox
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
         case SDL_DROPFILE:
            SetWindowFocus();
            m_pEngineLogic->VOnFileDrop( event.drop.file, GetMousePosition() );
            SDL_free( event.drop.file );    // Free dropped_file memory
            break;
         case SDL_MOUSEBUTTONDOWN:
         case SDL_KEYDOWN:
         case SDL_KEYUP:
         case SDL_TEXTEDITING:
         case SDL_TEXTINPUT:
         case SDL_KEYMAPCHANGED:
         case SDL_MOUSEMOTION:
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
            if( !SInputManager::GetSingleton().VOnMsgProc( event ) )
               {
               m_pEngineLogic->VOnMsgProc( event );
               }
         }
      
      }// If poll event exist
   }// Function MsgProc

BaseEngineLogic *EngineApp::VCreateLogic( void )
   {
   m_pEngineLogic = ENG_NEW BaseEngineLogic( g_pApp->m_pRenderManager );
   m_pEngineLogic->Init();

	return m_pEngineLogic;
   }

int EngineApp::Modal( shared_ptr<PromptBox> pModalScreen, int defaultAnswer )
   {
   ENG_ASSERT( GetWindow() != NULL && _T("Main Window is NULL!") );
   // if the dialog popped out while the window is minimized, flash it to notify
	if ( IsIconic(GetHwnd()) )
	   {
		FlashWhileMinimized();
	   }
   HumanView*  pHumanView = GetHumanView();
   
   if( pHumanView )
      {
      Uint32 eventEndType = pHumanView->GetModalEventType();
      Sint32 code = 0;
      m_pEngineLogic->VSetActorUpdate( false );
      m_pEngineLogic->VSetWorldUpdate( false );
    //  int result = PumpUntilMessage( eventEndType, code );
      m_pEngineLogic->VSetActorUpdate( true );
      m_pEngineLogic->VSetWorldUpdate( true );
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
   for( auto it = m_pEngineLogic->m_ViewList.begin(); it != m_pEngineLogic->m_ViewList.end(); ++it )
      {
      if( (*it)->VGetType() == ViewType::View_Human )
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
   Resource languageRes( languageFile, g_pApp->m_EngineOptions.GetIsUsingDevDirectory() );
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( languageRes );
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
		//ENG_ASSERT(0 && "String not found!");
		return L"";
	   }
	return localizedString->second;
   }  

void EngineApp::SetIsMouseCursorEnable( bool isDisplay )
   {
   if( isDisplay )
      {
      SDL_ShowCursor( SDL_ENABLE );
      }
   else
      {
      SDL_ShowCursor( SDL_DISABLE );
      }

   }

Point EngineApp::GetMousePosition( void )
   {
   int x, y;
   SDL_GetMouseState( &x, &y );
   return Point( x, y );
   }

void EngineApp::SetWindowFocus( void )
   {
#if defined( _WINDOWS ) || defined( WINDOWS )
   SetFocus( GetHwnd() );
#endif   
   }

void EngineApp::ResetMousePosition( void ) 
   {
   SetWindowFocus();
   SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
   }

int EngineApp::EventFilter( void* userdata, SDL_Event* event )
   {
   switch( event->type )
      {
      case SDL_WINDOWEVENT:
      case SDL_QUIT:
      case SDL_DROPFILE:
      case SDL_MOUSEBUTTONDOWN:
         if( SInputManager::GetSingleton().m_MouseButtonState[ event->button.button ].m_bIsDown )
            {
            return 0;
            }
         break;
      case SDL_KEYDOWN:
         if( SInputManager::GetSingleton().m_KeyState[ event->key.keysym.scancode ].m_bIsDown )
            {
            return 0;
            }
         break;
      case SDL_KEYUP:
      case SDL_TEXTEDITING:
      case SDL_TEXTINPUT:
      case SDL_KEYMAPCHANGED:
      case SDL_MOUSEMOTION:
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
      default:
         break;
      };
   return 1;
   }

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
			if ( m_pEngineLogic )
			   {
				double appTime = 0.0;
            double absoluteTime = 0.0;
            float  fElapasedTime = 0.0f;
            GetGlobalTimer()->GetTimeValues( &appTime, &absoluteTime, &fElapasedTime );
            SInputManager::GetSingleton().VOnTick( fElapasedTime );
            m_pEngineLogic->VOnUpdate( ( float ) appTime, fElapasedTime );
            OnFrameRender( appTime, fElapasedTime );
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
      SInputManager::GetSingleton().VOnTick( fElapasedTime );

      OnUpdateGame( fAppTime, fElapasedTime );
      
      OnFrameRender( fAppTime, fElapasedTime );

      }

   OnClose();
   }

void EngineApp::SingleLoop( void )
   {
   static double fAppTime = 0.0;
   static double fAbsoluteTime = 0.0;
   static float  fElapasedTime = 0.0f;
   MsgProc();
   GetGlobalTimer()->GetTimeValues( &fAppTime, &fAbsoluteTime, &fElapasedTime );

   OnUpdateGame( fAppTime, fElapasedTime );

   OnFrameRender( fAppTime, fElapasedTime );
   }

void EngineApp::OnUpdateGame( double fTime, float fElapsedTime )
   {
	if ( m_bQuitting )
	   {
      ENG_ASSERT( PushUserEvent( m_ShutDownEventType, 0 ) );
      }

	if ( m_pEngineLogic )
	   {
      IEventManager::GetSingleton()->VUpdate( 20 ); // allow event queue to process for up to 20 ms
		g_pApp->m_pEngineLogic->VOnUpdate( ( float ) fTime, fElapsedTime );
	   }
   }

void EngineApp::OnFrameRender( double fTime, float fElapsedTime )
   {
   g_pApp->m_pEngineLogic->VOnRender( fTime, fElapsedTime );
   }

// This is a WINDOWS dedicated function
// LATER: This function doesn't work as expected, it won't "blink" on and off, and I cann't find a solution yet
void EngineApp::FlashWhileMinimized( void )
   {
#if !defined( _WINDOWS ) && !defined( WINDOWS )
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

// TODO: recheck close order
void EngineApp::OnClose()
{
   m_bIsRunning = false;
	// release all the game systems in reverse order from which they were created
   ENG_LOG( "Test", "On close" );
   SInputManager::DeleteSingleton();
	SAFE_DELETE( m_pEngineLogic );
   SAFE_DELETE( m_pVideoResCache );
   IMG_Quit();
   SDL_DestroyWindow( m_pWindow );
   SDL_Quit();
   SAFE_DELETE( m_pEventManager );
   SAFE_DELETE( m_pResCache );

   LuaStateManager::GetSingleton().ClearLuaState();
   InternalScriptExports::Destroy();

	return;
}
