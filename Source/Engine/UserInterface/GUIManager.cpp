////////////////////////////////////////////////////////////////////////////////
// Filename: GUIManager.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "GUIManager.h"

CEGUI::OpenGL3Renderer* GUIManager::s_pRenderer = NULL;
CEGUI::DefaultResourceProvider* GUIManager::s_pResProvider = NULL;

GUIManager::GUIManager( void )
   {
   m_pContext = NULL;
   m_pRoot = NULL;
   m_pPromptRoot = NULL;
   m_pUIRoot = NULL;
   m_ModalEventType = 0;
   m_HasModalDialog = 0;
   }

void GUIManager::Init(  const std::string& resourceDirectory  )
   {
   if( !s_pRenderer || !s_pResProvider )
      {
      s_pRenderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
      s_pResProvider = static_cast< CEGUI::DefaultResourceProvider* >( CEGUI::System::getSingleton().getResourceProvider() );
      std::string resourceDir( resourceDirectory );
      s_pResProvider->setResourceGroupDirectory( "imagesets", resourceDir + "imagesets/" );
      s_pResProvider->setResourceGroupDirectory( "schemes", resourceDir + "schemes/" );
      s_pResProvider->setResourceGroupDirectory( "fonts", resourceDir + "fonts/" );
      s_pResProvider->setResourceGroupDirectory( "layouts", resourceDir + "layouts/" );
      s_pResProvider->setResourceGroupDirectory( "looknfeel", resourceDir + "looknfeel/" );
      s_pResProvider->setResourceGroupDirectory( "lua_scripts", resourceDir + "lua_scripts/" );

      CEGUI::ImageManager::setImagesetDefaultResourceGroup( "imagesets" );
      CEGUI::Scheme::setDefaultResourceGroup( "schemes" );
      CEGUI::Font::setDefaultResourceGroup( "fonts" );
      CEGUI::WindowManager::setDefaultResourceGroup( "layouts" );
      CEGUI::WidgetLookManager::setDefaultResourceGroup( "looknfeel" );
      CEGUI::ScriptModule::setDefaultResourceGroup( "lua_scripts" );
      }
   

   m_pContext = &CEGUI::System::getSingleton().createGUIContext( s_pRenderer->getDefaultRenderTarget() );
   LoadScheme( "WindowsLook.scheme" );
   SetFont( "UTF8.ttf" );

   m_pRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "root" );
   m_pContext->setRootWindow( m_pRoot );
   m_pRoot->setMousePassThroughEnabled( true );

   m_pPromptRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "prompt_root" );
   m_pPromptRoot->setMousePassThroughEnabled( true );
   m_pRoot->addChild( m_pPromptRoot );
  

   m_pUIRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "ui_root" );
   m_pUIRoot->setMousePassThroughEnabled( true );
   m_pRoot->addChild( m_pUIRoot );

   m_ModalEventType = SDL_RegisterEvents( 1 );
   ENG_ASSERT( m_ModalEventType != ((Uint32)-1) );
   }

void GUIManager::Destory( void )
   {
   CEGUI::System::destroy(); 
   CEGUI::OpenGL3Renderer::destroy( static_cast<CEGUI::OpenGL3Renderer&>( *s_pRenderer ) ); 
   }

void GUIManager::LoadScheme( const std::string& schemeFile )
   {
   CEGUI::SchemeManager::getSingleton().createFromFile( schemeFile );
   }

void GUIManager::SetFont( const std::string& fontFile )
   {
   ENG_ASSERT( m_pContext );
   // in order to display different language ex: Chinese
   CEGUI::FontManager::getSingleton().createFreeTypeFont( fontFile, 10/*pt*/, true, fontFile );
   m_pContext->setDefaultFont( fontFile );
   
  // CEGUI::FontManager::getSingleton().createFromFile( fontFile + ".font" );
  // m_pContext->setDefaultFont( fontFile );
   }

void GUIManager::OnUpdate( const unsigned long deltaMs )
   {
   // This fuction takes seconds as input
   m_pContext->injectTimePulse( (float)deltaMs / 1000.f );
   }

void GUIManager::OnRender( double fTime, float fElapsedTime )
   {
   //CEGUI::System::getSingleton().renderAllGUIContexts();
    CEGUI::System::getSingleton().renderAllGUIContexts();
   /* s_pRenderer->beginRendering( );
    m_pContext->draw( );
    s_pRenderer->endRendering( );*/
   }

int GUIManager::OnMsgProc( SDL_Event event ) // process the OS event
   {
   switch( event.type )
      {
      case SDL_MOUSEBUTTONDOWN:
         if( m_pContext->injectMouseButtonDown( SDLButtonTOCEGUIButton( event.button.button ) ) || m_HasModalDialog )
            {
            return 1;
            }
         
         break;
      case SDL_MOUSEBUTTONUP:
         if( m_pContext->injectMouseButtonUp( SDLButtonTOCEGUIButton( event.button.button ) ) || m_HasModalDialog )
            {
            return 1;
            }
         break;
      case SDL_MOUSEMOTION:
         if( m_pContext->injectMousePosition( (float)event.motion.x, (float)event.motion.y ) || m_HasModalDialog )
            {
            return 1;
            }
         break;
      case SDL_KEYDOWN:
         if( m_pContext->injectKeyDown( SDLKeyToCEGUIKey( event.key.keysym.sym ) ) || m_HasModalDialog )
            {
            
            return 1;
            }
         break;
      case SDL_KEYUP:
         if( m_pContext->injectKeyUp( SDLKeyToCEGUIKey( event.key.keysym.sym ) ) || m_HasModalDialog )
            {
            return 1;
            }
         break;
      case SDL_TEXTINPUT:
         // LATER: this convertion is wrong, we need to convert event.text.text (UTF8) to codePoint (UTF32)
         // for m_pContext->injectChar( UTF32DATA );
         CEGUI::utf32 codePoint = 0;
         for( int i = 0; event.text.text[i] != '\0'; ++i )
            {
            codePoint |= event.text.text[i] << ( i * 8 );
            }
         if( m_pContext->injectChar( codePoint ) || m_HasModalDialog )
            {
            return 1;
            }
         break;
               
      }
   return 0;
   }


int GUIManager::Ask( MessageBox_Questions question )
   {
   std::wstring msg;
	std::wstring title;
	int buttonFlags;
	int defaultAnswer = IDOK;

	switch(question)
	   {
		case QUESTION_WHERES_THE_CD:
		   {
			msg = (_T("IDS_QUESTION_WHERES_THE_CD"));
			title = (_T("IDS_ALERT"));
			buttonFlags = MB_RETRYCANCEL;
			defaultAnswer = IDCANCEL;
			break;
		   }
		case QUESTION_QUIT_GAME:
		   {
			msg = (_T("IDS_QUESTION_QUIT_GAME"));
			title = (_T("IDS_QUESTION"));
			buttonFlags = MB_YESNO;
			defaultAnswer = IDNO;
			break;
		   }
		default:
			ENG_ASSERT(0 && _T("Undefined question in GUIManager::Ask"));
			return IDCANCEL;
	   }

	if ( m_pContext )
	   {
      if ( m_HasModalDialog & 0x10000000 )
	      {
		   ENG_ASSERT( 0 && "Too Many nested dialogs!" );
		   return defaultAnswer;
	      }
      m_HasModalDialog <<= 1;
	   m_HasModalDialog |= 1;

      SDL_ShowCursor( SDL_ENABLE );
		shared_ptr<Dialog> pDialog( ENG_NEW Dialog( m_pPromptRoot, m_ModalEventType, msg, title, buttonFlags ) );
      pDialog->m_pWindow->setModalState( true );
      int result = g_pApp->Modal( pDialog, defaultAnswer );

      m_HasModalDialog >>= 1;
      if( !m_HasModalDialog )
         {
         SDL_ShowCursor( SDL_DISABLE );
         }
		return result;
	   }
	// If the engine is not exist, still pop a message box
	return 0;
   //return ::MessageBox(g_pApp ? g_pApp->GetHwnd() : NULL, msg.c_str(), title.c_str(), buttonFlags);
   }

CEGUI::Key::Scan GUIManager::SDLKeyToCEGUIKey( SDL_Keycode key )
   {
   using namespace CEGUI;
   switch (key)
      {
      case SDLK_BACKSPACE:    return Key::Backspace;
      case SDLK_TAB:          return Key::Tab;
      case SDLK_RETURN:       return Key::Return;
      case SDLK_PAUSE:        return Key::Pause;
      case SDLK_ESCAPE:       return Key::Escape;
      case SDLK_SPACE:        return Key::Space;
      case SDLK_COMMA:        return Key::Comma;
      case SDLK_MINUS:        return Key::Minus;
      case SDLK_PERIOD:       return Key::Period;
      case SDLK_SLASH:        return Key::Slash;
      case SDLK_0:            return Key::Zero;
      case SDLK_1:            return Key::One;
      case SDLK_2:            return Key::Two;
      case SDLK_3:            return Key::Three;
      case SDLK_4:            return Key::Four;
      case SDLK_5:            return Key::Five;
      case SDLK_6:            return Key::Six;
      case SDLK_7:            return Key::Seven;
      case SDLK_8:            return Key::Eight;
      case SDLK_9:            return Key::Nine;
      case SDLK_COLON:        return Key::Colon;
      case SDLK_SEMICOLON:    return Key::Semicolon;
      case SDLK_EQUALS:       return Key::Equals;
      case SDLK_LEFTBRACKET:  return Key::LeftBracket;
      case SDLK_BACKSLASH:    return Key::Backslash;
      case SDLK_RIGHTBRACKET: return Key::RightBracket;
      case SDLK_a:            return Key::A;
      case SDLK_b:            return Key::B;
      case SDLK_c:            return Key::C;
      case SDLK_d:            return Key::D;
      case SDLK_e:            return Key::E;
      case SDLK_f:            return Key::F;
      case SDLK_g:            return Key::G;
      case SDLK_h:            return Key::H;
      case SDLK_i:            return Key::I;
      case SDLK_j:            return Key::J;
      case SDLK_k:            return Key::K;
      case SDLK_l:            return Key::L;
      case SDLK_m:            return Key::M;
      case SDLK_n:            return Key::N;
      case SDLK_o:            return Key::O;
      case SDLK_p:            return Key::P;
      case SDLK_q:            return Key::Q;
      case SDLK_r:            return Key::R;
      case SDLK_s:            return Key::S;
      case SDLK_t:            return Key::T;
      case SDLK_u:            return Key::U;
      case SDLK_v:            return Key::V;
      case SDLK_w:            return Key::W;
      case SDLK_x:            return Key::X;
      case SDLK_y:            return Key::Y;
      case SDLK_z:            return Key::Z;
      case SDLK_DELETE:       return Key::Delete;
      case SDLK_KP_0:          return Key::Numpad0;
      case SDLK_KP_1:          return Key::Numpad1;
      case SDLK_KP_2:          return Key::Numpad2;
      case SDLK_KP_3:          return Key::Numpad3;
      case SDLK_KP_4:          return Key::Numpad4;
      case SDLK_KP_5:          return Key::Numpad5;
      case SDLK_KP_6:          return Key::Numpad6;
      case SDLK_KP_7:          return Key::Numpad7;
      case SDLK_KP_8:          return Key::Numpad8;
      case SDLK_KP_9:          return Key::Numpad9;
      case SDLK_KP_PERIOD:    return Key::Decimal;
      case SDLK_KP_DIVIDE:    return Key::Divide;
      case SDLK_KP_MULTIPLY:  return Key::Multiply;
      case SDLK_KP_MINUS:     return Key::Subtract;
      case SDLK_KP_PLUS:      return Key::Add;
      case SDLK_KP_ENTER:     return Key::NumpadEnter;
      case SDLK_KP_EQUALS:    return Key::NumpadEquals;
      case SDLK_UP:           return Key::ArrowUp;
      case SDLK_DOWN:         return Key::ArrowDown;
      case SDLK_RIGHT:        return Key::ArrowRight;
      case SDLK_LEFT:         return Key::ArrowLeft;
      case SDLK_INSERT:       return Key::Insert;
      case SDLK_HOME:         return Key::Home;
      case SDLK_END:          return Key::End;
      case SDLK_PAGEUP:       return Key::PageUp;
      case SDLK_PAGEDOWN:     return Key::PageDown;
      case SDLK_F1:           return Key::F1;
      case SDLK_F2:           return Key::F2;
      case SDLK_F3:           return Key::F3;
      case SDLK_F4:           return Key::F4;
      case SDLK_F5:           return Key::F5;
      case SDLK_F6:           return Key::F6;
      case SDLK_F7:           return Key::F7;
      case SDLK_F8:           return Key::F8;
      case SDLK_F9:           return Key::F9;
      case SDLK_F10:          return Key::F10;
      case SDLK_F11:          return Key::F11;
      case SDLK_F12:          return Key::F12;
      case SDLK_F13:          return Key::F13;
      case SDLK_F14:          return Key::F14;
      case SDLK_F15:          return Key::F15;
   //   case SDLK_NUMLOCKCLEAR: return Key::NumLock; // is it correct?
      case SDLK_SCROLLLOCK:    return Key::ScrollLock;
      case SDLK_RSHIFT:       return Key::RightShift;
      case SDLK_LSHIFT:       return Key::LeftShift;
      case SDLK_RCTRL:        return Key::RightControl;
      case SDLK_LCTRL:        return Key::LeftControl;
      case SDLK_RALT:         return Key::RightAlt;
      case SDLK_LALT:         return Key::LeftAlt;
  //    case SDLK_LSUPER:       return Key::LeftWindows;
   //   case SDLK_RSUPER:       return Key::RightWindows;
      case SDLK_SYSREQ:       return Key::SysRq;
      case SDLK_MENU:         return Key::AppMenu;
      case SDLK_POWER:        return Key::Power;
      default:                return Key::Unknown;
      }
      std::cout << "unkown key" << std::endl;
      return Key::Unknown;
   }

CEGUI::MouseButton GUIManager::SDLButtonTOCEGUIButton( Uint8 button )
   {
   using namespace CEGUI;
   switch( button )
      {
      case SDL_BUTTON_LEFT:   return MouseButton::LeftButton;
      case SDL_BUTTON_MIDDLE: return MouseButton::MiddleButton;
      case SDL_BUTTON_RIGHT:  return MouseButton::RightButton;
      case SDL_BUTTON_X1:     return MouseButton::X1Button;
      case SDL_BUTTON_X2:     return MouseButton::X2Button;
      default:                return MouseButton::NoButton;
      }
   return MouseButton::NoButton;
   }