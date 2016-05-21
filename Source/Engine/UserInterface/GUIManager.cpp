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
      s_pRenderer->enableExtraStateSettings( true );
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

void GUIManager::OnUpdate( const unsigned long deltaMs )
   {
   // This fuction takes seconds as input
   m_pContext->injectTimePulse( (float)deltaMs / 1000.f );
   }

void GUIManager::OnRender( double fTime, float fElapsedTime )
   {
   CEGUI::System::getSingleton().renderAllGUIContexts();
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
         if( m_pContext->injectKeyDown( SDLKeyToCEGUIKey( event.key.keysym.sym ) ) || hasModalWindow )
            {
            
            return 1;
            }
         break;
      case SDL_KEYUP:
         if( m_pContext->injectKeyUp( SDLKeyToCEGUIKey( event.key.keysym.sym ) ) || hasModalWindow )
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