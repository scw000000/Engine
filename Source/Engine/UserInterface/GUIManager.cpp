/*!
 * \file GUIManager.cpp
 * \date 2016/05/19 15:43
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
#include "GUIManager.h"

CEGUI::OpenGL3Renderer* GUIManager::s_pRenderer = NULL;
CEGUI::DefaultResourceProvider* GUIManager::s_pResProvider = NULL;

GUIManager::GUIManager( void )
   {
   m_pContext = NULL;
   m_pRoot = NULL;
   m_pPromptRoot = NULL;
   m_pUIRoot = NULL;
   }

void GUIManager::Init(  const std::string& resourceDirectory  )
   {
   if( !s_pRenderer || !s_pResProvider )
      {
      s_pRenderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
      s_pResProvider = static_cast< CEGUI::DefaultResourceProvider* >( CEGUI::System::getSingleton().getResourceProvider() );
      m_ResourceDir = resourceDirectory;
      s_pResProvider->setResourceGroupDirectory( "imagesets", m_ResourceDir + "imagesets/" );
      s_pResProvider->setResourceGroupDirectory( "schemes", m_ResourceDir + "schemes/" );
      s_pResProvider->setResourceGroupDirectory( "fonts", m_ResourceDir + "fonts/" );
      s_pResProvider->setResourceGroupDirectory( "layouts", m_ResourceDir + "layouts/" );
      s_pResProvider->setResourceGroupDirectory( "looknfeel", m_ResourceDir + "looknfeel/" );
      s_pResProvider->setResourceGroupDirectory( "lua_scripts", m_ResourceDir + "lua_scripts/" );

      CEGUI::ImageManager::setImagesetDefaultResourceGroup( "imagesets" );
      CEGUI::Scheme::setDefaultResourceGroup( "schemes" );
      CEGUI::Font::setDefaultResourceGroup( "fonts" );
      CEGUI::WindowManager::setDefaultResourceGroup( "layouts" );
      CEGUI::WidgetLookManager::setDefaultResourceGroup( "looknfeel" );
      CEGUI::ScriptModule::setDefaultResourceGroup( "lua_scripts" );
      }
   

   m_pContext = &CEGUI::System::getSingleton().createGUIContext( s_pRenderer->getDefaultRenderTarget() );
   LoadScheme( "GlossySerpentFHD.scheme" );
   LoadScheme( "WindowsLook.scheme" );
   CEGUI::ImageManager::getSingleton( ).loadImageset( "GlossySerpentFHDCursors.imageset" );
   CEGUI::ImageManager::getSingleton( ).loadImageset( "WindowsLook.imageset" );

   m_pContext->getMouseCursor().setDefaultImage( "WindowsLook/MouseArrow" );
   m_pContext->getMouseCursor().hide();

   SetFont( "UTF8.ttf" );

   m_pRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "root" );
   m_pContext->setRootWindow( m_pRoot );
   m_pRoot->setMousePassThroughEnabled( true );

  /* m_pUIRoot = CEGUI::WindowManager::getSingleton( ).loadLayoutFromFile( g_pApp->m_EngineOptions.m_Layout, "ui_" );
   m_pRoot->addChild( m_pUIRoot );

   m_pPromptRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "prompt_root" );
   m_pPromptRoot->setMousePassThroughEnabled( true );
   m_pPromptRoot->setAlwaysOnTop( true );

   m_pRoot->addChild( m_pPromptRoot );*/
   //m_pRoot->addChild( m_pUIRoot );
   //m_pUIRoot->addChild( m_pPromptRoot );

   //m_ModalEventType = SDL_RegisterEvents( 1 );
   //ENG_ASSERT( m_ModalEventType != ((Uint32)-1) );
   }

void GUIManager::Destory( void )
   {
   m_Layouts.clear();
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

void GUIManager::OnUpdate( unsigned long deltaMs )
   {
   // This fuction takes seconds as input
   m_pContext->injectTimePulse( (float)deltaMs / 1000.f );
   }

void GUIManager::OnRender( double fTime, float fElapsedTime )
   {
   s_pRenderer->enableExtraStateSettings( false );
   CEGUI::System::getSingleton().renderAllGUIContexts();
   s_pRenderer->enableExtraStateSettings( true );
   /*s_pRenderer->beginRendering( );
   m_pContext->draw( );
   s_pRenderer->endRendering( );*/
   }

void GUIManager::AttachLayout( shared_ptr<UserInterface> pUI )
   {
   auto pRootWindow = pUI->GetRootWindow();
   m_Layouts[ pRootWindow ] = pUI;
   m_pRoot->addChild( pRootWindow );
   pRootWindow->subscribeEvent( CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber( &GUIManager::OnMouseEntersArea, this ) );
   
   }

void GUIManager::DetachLayout( shared_ptr<UserInterface> pUI )
   {
   auto findResult = m_Layouts.find( pUI->GetRootWindow() );
   if( findResult != m_Layouts.end() )
      {
      m_pRoot->destroyChild( findResult->first );
      m_Layouts.erase( findResult );
      } 
   }

bool GUIManager::OnKeyDown( const SDL_Scancode& code )
   {
   return m_pContext->injectKeyDown( SDLScanToCEGUIScanCode( code ) ) || HasModelWindow();
   }

bool GUIManager::OnKeyUp( const SDL_Scancode& code )
   {
   return m_pContext->injectKeyUp( SDLScanToCEGUIScanCode( code ) ) || HasModelWindow();
   }

bool GUIManager::OnMouseMove( Point motion )
   {
   m_pContext->injectMousePosition( ( float ) motion.x, ( float ) motion.y );
   return true;
   }

bool GUIManager::OnMouseButtonDown( Uint8 button )
   {
   return m_pContext->injectMouseButtonDown( SDLButtonTOCEGUIButton( button ) ) || HasModelWindow();
   }

bool GUIManager::OnMouseButtonUp( Uint8 button )
   {
   return m_pContext->injectMouseButtonUp( SDLButtonTOCEGUIButton( button ) ) || HasModelWindow();
   }

int GUIManager::OnMsgProc( SDL_Event event ) // process the OS event
   {
   bool hasModalWindow = m_pContext->getModalWindow() != NULL;
   switch( event.type )
      {
      case SDL_MOUSEBUTTONDOWN:
         if( m_pContext->injectMouseButtonDown( SDLButtonTOCEGUIButton( event.button.button ) ) || hasModalWindow )
            {
            return 1;
            }
         
         break;
      case SDL_MOUSEBUTTONUP:
         if( m_pContext->injectMouseButtonUp( SDLButtonTOCEGUIButton( event.button.button ) ) || hasModalWindow )
            {
            return 1;
            }
         break;
      case SDL_MOUSEMOTION:
         m_pContext->injectMousePosition( (float)event.motion.x, (float)event.motion.y );
         if( hasModalWindow )
            {
            return 1;
            }
         break;
      case SDL_KEYDOWN:
         if( m_pContext->injectKeyDown( SDLScanToCEGUIScanCode( event.key.keysym.scancode ) ) || hasModalWindow )
            {
            
            return 1;
            }
         break;
      case SDL_KEYUP:
         if( m_pContext->injectKeyUp( SDLScanToCEGUIScanCode( event.key.keysym.scancode ) ) || hasModalWindow )
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
         if( m_pContext->injectChar( codePoint ) || hasModalWindow )
            {
            return 1;
            }
         break;
               
      }
   return 0;
   }

// CEGUI::UDim( scale, pos ) : scale : relative point on the screen between (0,1)
//                             pos   : additional absolute shift in pixels
// UDim( 0.5, 100 ) -> middle point + shift 100
CEGUI::Window* GUIManager::CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position, const Vec4& size )
   {
   CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton( ).createWindow( type, name );
   pWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( position.x, position.y ), CEGUI::UDim( position.z, position.w ) ) );
   pWindow->setSize( CEGUI::USize( CEGUI::UDim( size.x, size.y ), CEGUI::UDim( size.z, size.w ) ) );
   return pWindow;
   }

CEGUI::Key::Scan GUIManager::SDLScanToCEGUIScanCode( SDL_Scancode code )
   {
   using namespace CEGUI;
   switch( code )
      {
      case SDL_SCANCODE_BACKSPACE:    return Key::Backspace;
      case SDL_SCANCODE_TAB:          return Key::Tab;
      case SDL_SCANCODE_RETURN:       return Key::Return;
      case SDL_SCANCODE_PAUSE:        return Key::Pause;
      case SDL_SCANCODE_ESCAPE:       return Key::Escape;
      case SDL_SCANCODE_SPACE:        return Key::Space;
      case SDL_SCANCODE_COMMA:        return Key::Comma;
      case SDL_SCANCODE_MINUS:        return Key::Minus;
      case SDL_SCANCODE_PERIOD:       return Key::Period;
      case SDL_SCANCODE_SLASH:        return Key::Slash;
      case SDL_SCANCODE_0:            return Key::Zero;
      case SDL_SCANCODE_1:            return Key::One;
      case SDL_SCANCODE_2:            return Key::Two;
      case SDL_SCANCODE_3:            return Key::Three;
      case SDL_SCANCODE_4:            return Key::Four;
      case SDL_SCANCODE_5:            return Key::Five;
      case SDL_SCANCODE_6:            return Key::Six;
      case SDL_SCANCODE_7:            return Key::Seven;
      case SDL_SCANCODE_8:            return Key::Eight;
      case SDL_SCANCODE_9:            return Key::Nine;
      case SDL_SCANCODE_KP_COLON:        return Key::Colon;
      case SDL_SCANCODE_SEMICOLON:    return Key::Semicolon;
      case SDL_SCANCODE_EQUALS:       return Key::Equals;
      case SDL_SCANCODE_LEFTBRACKET:  return Key::LeftBracket;
      case SDL_SCANCODE_BACKSLASH:    return Key::Backslash;
      case SDL_SCANCODE_RIGHTBRACKET: return Key::RightBracket;
      case SDL_SCANCODE_A:            return Key::A;
      case SDL_SCANCODE_B:            return Key::B;
      case SDL_SCANCODE_C:            return Key::C;
      case SDL_SCANCODE_D:            return Key::D;
      case SDL_SCANCODE_E:            return Key::E;
      case SDL_SCANCODE_F:            return Key::F;
      case SDL_SCANCODE_G:            return Key::G;
      case SDL_SCANCODE_H:            return Key::H;
      case SDL_SCANCODE_I:            return Key::I;
      case SDL_SCANCODE_J:            return Key::J;
      case SDL_SCANCODE_K:            return Key::K;
      case SDL_SCANCODE_L:            return Key::L;
      case SDL_SCANCODE_M:            return Key::M;
      case SDL_SCANCODE_N:            return Key::N;
      case SDL_SCANCODE_O:            return Key::O;
      case SDL_SCANCODE_P:            return Key::P;
      case SDL_SCANCODE_Q:            return Key::Q;
      case SDL_SCANCODE_R:            return Key::R;
      case SDL_SCANCODE_S:            return Key::S;
      case SDL_SCANCODE_T:            return Key::T;
      case SDL_SCANCODE_U:            return Key::U;
      case SDL_SCANCODE_V:            return Key::V;
      case SDL_SCANCODE_W:            return Key::W;
      case SDL_SCANCODE_X:            return Key::X;
      case SDL_SCANCODE_Y:            return Key::Y;
      case SDL_SCANCODE_Z:            return Key::Z;
      case SDL_SCANCODE_DELETE:       return Key::Delete;
      case SDL_SCANCODE_KP_0:          return Key::Numpad0;
      case SDL_SCANCODE_KP_1:          return Key::Numpad1;
      case SDL_SCANCODE_KP_2:          return Key::Numpad2;
      case SDL_SCANCODE_KP_3:          return Key::Numpad3;
      case SDL_SCANCODE_KP_4:          return Key::Numpad4;
      case SDL_SCANCODE_KP_5:          return Key::Numpad5;
      case SDL_SCANCODE_KP_6:          return Key::Numpad6;
      case SDL_SCANCODE_KP_7:          return Key::Numpad7;
      case SDL_SCANCODE_KP_8:          return Key::Numpad8;
      case SDL_SCANCODE_KP_9:          return Key::Numpad9;
      case SDL_SCANCODE_KP_PERIOD:    return Key::Decimal;
      case SDL_SCANCODE_KP_DIVIDE:    return Key::Divide;
      case SDL_SCANCODE_KP_MULTIPLY:  return Key::Multiply;
      case SDL_SCANCODE_KP_MINUS:     return Key::Subtract;
      case SDL_SCANCODE_KP_PLUS:      return Key::Add;
      case SDL_SCANCODE_KP_ENTER:     return Key::NumpadEnter;
      case SDL_SCANCODE_KP_EQUALS:    return Key::NumpadEquals;
      case SDL_SCANCODE_UP:           return Key::ArrowUp;
      case SDL_SCANCODE_DOWN:         return Key::ArrowDown;
      case SDL_SCANCODE_RIGHT:        return Key::ArrowRight;
      case SDL_SCANCODE_LEFT:         return Key::ArrowLeft;
      case SDL_SCANCODE_INSERT:       return Key::Insert;
      case SDL_SCANCODE_HOME:         return Key::Home;
      case SDL_SCANCODE_END:          return Key::End;
      case SDL_SCANCODE_PAGEUP:       return Key::PageUp;
      case SDL_SCANCODE_PAGEDOWN:     return Key::PageDown;
      case SDL_SCANCODE_F1:           return Key::F1;
      case SDL_SCANCODE_F2:           return Key::F2;
      case SDL_SCANCODE_F3:           return Key::F3;
      case SDL_SCANCODE_F4:           return Key::F4;
      case SDL_SCANCODE_F5:           return Key::F5;
      case SDL_SCANCODE_F6:           return Key::F6;
      case SDL_SCANCODE_F7:           return Key::F7;
      case SDL_SCANCODE_F8:           return Key::F8;
      case SDL_SCANCODE_F9:           return Key::F9;
      case SDL_SCANCODE_F10:          return Key::F10;
      case SDL_SCANCODE_F11:          return Key::F11;
      case SDL_SCANCODE_F12:          return Key::F12;
      case SDL_SCANCODE_F13:          return Key::F13;
      case SDL_SCANCODE_F14:          return Key::F14;
      case SDL_SCANCODE_F15:          return Key::F15;
   //   case SDL_SCANCODE_NUMLOCKCLEAR: return Key::NumLock; // is it correct?
      case SDL_SCANCODE_SCROLLLOCK:    return Key::ScrollLock;
      case SDL_SCANCODE_RSHIFT:       return Key::RightShift;
      case SDL_SCANCODE_LSHIFT:       return Key::LeftShift;
      case SDL_SCANCODE_RCTRL:        return Key::RightControl;
      case SDL_SCANCODE_LCTRL:        return Key::LeftControl;
      case SDL_SCANCODE_RALT:         return Key::RightAlt;
      case SDL_SCANCODE_LALT:         return Key::LeftAlt;
  //    case SDL_SCANCODE_LSUPER:       return Key::LeftWindows;
   //   case SDL_SCANCODE_RSUPER:       return Key::RightWindows;
      case SDL_SCANCODE_SYSREQ:       return Key::SysRq;
      case SDL_SCANCODE_MENU:         return Key::AppMenu;
      case SDL_SCANCODE_POWER:        return Key::Power;
      default:                return Key::Unknown;
      }
      ENG_WARNING( "Unkown key input" );
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

void GUIManager::SetDisplayMouseCursor( bool isDisplay )
   {
   if( isDisplay )
      {
      m_pContext->getMouseCursor( ).show( );
      }
   else
      {
      m_pContext->getMouseCursor( ).hide( );
      }
   }

bool GUIManager::OnMouseEntersArea( const CEGUI::EventArgs& arg )
   {
   const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>( arg );
   auto findResult = m_Layouts.find( mouseArgs.window );
   if( findResult != m_Layouts.end() )
      {
      SetDisplayMouseCursor( findResult->second->VIsMouseCursorEnable() );
      }
   return false;
   }