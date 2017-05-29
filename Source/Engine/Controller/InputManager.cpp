/*!
 * \file InputManager.cpp
 * \date 2017/05/28 15:17
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "InputManager.h"

#define MAX_ALLOWABLE_CONTROLLERS 16

std::vector< SDL_Scancode > InputManager::s_ValidScanCodes(104);

int InputManager::Init( GUIManager* pManager )
   {
   m_pGUIManager = pManager;
   if( !pManager )
      {
      return S_FALSE;
      }
   m_CurMousePos = m_LastMousePos = g_pApp->GetMousePosition();
   m_MouseShift = Point( 0, 0 );
   ENG_ZERO_MEM( m_KeyState );
   ENG_ZERO_MEM( m_MouseButtonState );
   return S_OK;
   /*s_ValidScanCodes[ 0 ] = SDL_SCANCODE_BACKSPACE;
   s_ValidScanCodes[ 1 ] = SDL_SCANCODE_TAB;
   s_ValidScanCodes[ 2 ] = SDL_SCANCODE_RETURN;
   s_ValidScanCodes[ 3 ] = SDL_SCANCODE_PAUSE;
   s_ValidScanCodes[ 4 ] = SDL_SCANCODE_ESCAPE;
   s_ValidScanCodes[ 5 ] = SDL_SCANCODE_SPACE;
   s_ValidScanCodes[ 6 ] = SDL_SCANCODE_COMMA;
   s_ValidScanCodes[ 7 ] = SDL_SCANCODE_MINUS;
   s_ValidScanCodes[ 8 ] = SDL_SCANCODE_PERIOD;
   s_ValidScanCodes[ 9 ] = SDL_SCANCODE_SLASH;
   s_ValidScanCodes[ 10 ] = SDL_SCANCODE_0;
   s_ValidScanCodes[ 11 ] = SDL_SCANCODE_1;
   s_ValidScanCodes[ 12 ] = SDL_SCANCODE_2;
   s_ValidScanCodes[ 13 ] = SDL_SCANCODE_3;
   s_ValidScanCodes[ 14 ] = SDL_SCANCODE_4;
   s_ValidScanCodes[ 15 ] = SDL_SCANCODE_5;
   s_ValidScanCodes[ 16 ] = SDL_SCANCODE_6;
   s_ValidScanCodes[ 17 ] = SDL_SCANCODE_7;
   s_ValidScanCodes[ 18 ] = SDL_SCANCODE_8;
   s_ValidScanCodes[ 19 ] = SDL_SCANCODE_9;
   s_ValidScanCodes[ 20 ] = SDL_SCANCODE_KP_COLON;
   s_ValidScanCodes[ 21 ] = SDL_SCANCODE_SEMICOLON;
   s_ValidScanCodes[ 22 ] = SDL_SCANCODE_EQUALS;
   s_ValidScanCodes[ 23 ] = SDL_SCANCODE_LEFTBRACKET;
   s_ValidScanCodes[ 24 ] = SDL_SCANCODE_BACKSLASH;
   s_ValidScanCodes[ 25 ] = SDL_SCANCODE_RIGHTBRACKET;
   s_ValidScanCodes[ 26 ] = SDL_SCANCODE_A;
   s_ValidScanCodes[ 27 ] = SDL_SCANCODE_B;
   s_ValidScanCodes[ 28 ] = SDL_SCANCODE_C;
   s_ValidScanCodes[ 29 ] = SDL_SCANCODE_D;
   s_ValidScanCodes[ 30 ] = SDL_SCANCODE_E;
   s_ValidScanCodes[ 31 ] = SDL_SCANCODE_F;
   s_ValidScanCodes[ 32 ] = SDL_SCANCODE_G;
   s_ValidScanCodes[ 33 ] = SDL_SCANCODE_H;
   s_ValidScanCodes[ 34 ] = SDL_SCANCODE_I;
   s_ValidScanCodes[ 35 ] = SDL_SCANCODE_J;
   s_ValidScanCodes[ 36 ] = SDL_SCANCODE_K;
   s_ValidScanCodes[ 37 ] = SDL_SCANCODE_L;
   s_ValidScanCodes[ 38 ] = SDL_SCANCODE_M;
   s_ValidScanCodes[ 39 ] = SDL_SCANCODE_N;
   s_ValidScanCodes[ 40 ] = SDL_SCANCODE_O;
   s_ValidScanCodes[ 41 ] = SDL_SCANCODE_P;
   s_ValidScanCodes[ 42 ] = SDL_SCANCODE_Q;
   s_ValidScanCodes[ 43 ] = SDL_SCANCODE_R;
   s_ValidScanCodes[ 44 ] = SDL_SCANCODE_S;
   s_ValidScanCodes[ 45 ] = SDL_SCANCODE_T;
   s_ValidScanCodes[ 46 ] = SDL_SCANCODE_U;
   s_ValidScanCodes[ 47 ] = SDL_SCANCODE_V;
   s_ValidScanCodes[ 48 ] = SDL_SCANCODE_W;
   s_ValidScanCodes[ 49 ] = SDL_SCANCODE_X;
   s_ValidScanCodes[ 50 ] = SDL_SCANCODE_Y;
   s_ValidScanCodes[ 51 ] = SDL_SCANCODE_Z;
   s_ValidScanCodes[ 52 ] = SDL_SCANCODE_DELETE;
   s_ValidScanCodes[ 53 ] = SDL_SCANCODE_KP_0;
   s_ValidScanCodes[ 54 ] = SDL_SCANCODE_KP_1;
   s_ValidScanCodes[ 55 ] = SDL_SCANCODE_KP_2;
   s_ValidScanCodes[ 56 ] = SDL_SCANCODE_KP_3;
   s_ValidScanCodes[ 57 ] = SDL_SCANCODE_KP_4;
   s_ValidScanCodes[ 58 ] = SDL_SCANCODE_KP_5;
   s_ValidScanCodes[ 59 ] = SDL_SCANCODE_KP_6;
   s_ValidScanCodes[ 60 ] = SDL_SCANCODE_KP_7;
   s_ValidScanCodes[ 61 ] = SDL_SCANCODE_KP_8;
   s_ValidScanCodes[ 62 ] = SDL_SCANCODE_KP_9;
   s_ValidScanCodes[ 63 ] = SDL_SCANCODE_KP_PERIOD;
   s_ValidScanCodes[ 64 ] = SDL_SCANCODE_KP_DIVIDE;
   s_ValidScanCodes[ 65 ] = SDL_SCANCODE_KP_MULTIPLY;
   s_ValidScanCodes[ 66 ] = SDL_SCANCODE_KP_MINUS;
   s_ValidScanCodes[ 67 ] = SDL_SCANCODE_KP_PLUS;
   s_ValidScanCodes[ 68 ] = SDL_SCANCODE_KP_ENTER;
   s_ValidScanCodes[ 69 ] = SDL_SCANCODE_KP_EQUALS;
   s_ValidScanCodes[ 70 ] = SDL_SCANCODE_UP;
   s_ValidScanCodes[ 71 ] = SDL_SCANCODE_DOWN;
   s_ValidScanCodes[ 72 ] = SDL_SCANCODE_RIGHT;
   s_ValidScanCodes[ 73 ] = SDL_SCANCODE_LEFT;
   s_ValidScanCodes[ 74 ] = SDL_SCANCODE_INSERT;
   s_ValidScanCodes[ 75 ] = SDL_SCANCODE_HOME;
   s_ValidScanCodes[ 76 ] = SDL_SCANCODE_END;
   s_ValidScanCodes[ 77 ] = SDL_SCANCODE_PAGEUP;
   s_ValidScanCodes[ 78 ] = SDL_SCANCODE_PAGEDOWN;
   s_ValidScanCodes[ 79 ] = SDL_SCANCODE_F1;
   s_ValidScanCodes[ 80 ] = SDL_SCANCODE_F2;
   s_ValidScanCodes[ 81 ] = SDL_SCANCODE_F3;
   s_ValidScanCodes[ 82 ] = SDL_SCANCODE_F4;
   s_ValidScanCodes[ 83 ] = SDL_SCANCODE_F5;
   s_ValidScanCodes[ 84 ] = SDL_SCANCODE_F6;
   s_ValidScanCodes[ 85 ] = SDL_SCANCODE_F7;
   s_ValidScanCodes[ 86 ] = SDL_SCANCODE_F8;
   s_ValidScanCodes[ 87 ] = SDL_SCANCODE_F9;
   s_ValidScanCodes[ 88 ] = SDL_SCANCODE_F10;
   s_ValidScanCodes[ 89 ] = SDL_SCANCODE_F11;
   s_ValidScanCodes[ 90 ] = SDL_SCANCODE_F12;
   s_ValidScanCodes[ 91 ] = SDL_SCANCODE_F13;
   s_ValidScanCodes[ 92 ] = SDL_SCANCODE_F14;
   s_ValidScanCodes[ 93 ] = SDL_SCANCODE_F15;
   s_ValidScanCodes[ 94 ] = SDL_SCANCODE_SCROLLLOCK;
   s_ValidScanCodes[ 95 ] = SDL_SCANCODE_RSHIFT;
   s_ValidScanCodes[ 96 ] = SDL_SCANCODE_LSHIFT;
   s_ValidScanCodes[ 97 ] = SDL_SCANCODE_RCTRL;
   s_ValidScanCodes[ 98 ] = SDL_SCANCODE_LCTRL;
   s_ValidScanCodes[ 99 ] = SDL_SCANCODE_RALT;
   s_ValidScanCodes[ 100 ] = SDL_SCANCODE_LALT;
   s_ValidScanCodes[ 101 ] = SDL_SCANCODE_SYSREQ;
   s_ValidScanCodes[ 102 ] = SDL_SCANCODE_MENU;
   s_ValidScanCodes[ 103 ] = SDL_SCANCODE_POWER;*/
   }

void InputManager::VOnMouseMove( Point motion )
   {
   m_CurMousePos = motion;
   }

bool InputManager::VOnMsgProc( const SDL_Event& event )
   {
   switch( event.type )
      {
      case SDL_KEYDOWN:
            {
            VOnKeyDown( event.key.keysym.scancode );
            m_KeyState[ event.key.keysym.scancode ].m_bIsMasked = m_pGUIManager->OnKeyDown( event.key.keysym.scancode );
            return true;
            }
      case SDL_KEYUP:
            {
            VOnKeyUp( event.key.keysym.scancode );
            m_KeyState[ event.key.keysym.scancode ].m_bIsMasked = m_pGUIManager->OnKeyUp( event.key.keysym.scancode );
            return true;
            }
      case SDL_MOUSEMOTION:
            {
            VOnMouseMove( Point( event.motion.x, event.motion.y ) );
            m_pGUIManager->OnMouseMove( Point( event.motion.x, event.motion.y ) );
            return true;
            }
      case SDL_MOUSEBUTTONDOWN:
            {
            VOnMouseButtonDown( event.button.button );
            m_KeyState[ event.button.button ].m_bIsMasked = m_pGUIManager->OnMouseButtonDown( event.button.button );
            return true;
            }
      case SDL_MOUSEBUTTONUP:
            {
            VOnMouseButtonUp( event.button.button );
            m_KeyState[ event.button.button ].m_bIsMasked = m_pGUIManager->OnMouseButtonUp( event.button.button );
            return true;
            }
      case SDL_MOUSEWHEEL:
            return true;
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
         return false;
      }// Switch
   return false;
   }

void InputManager::VOnTick( float deltaSeconds )
   {
   // also need to update CEGUI to repeat the key
   /*for( auto validScanCode : s_ValidScanCodes )
      {
      if( m_KeyState[ validScanCode ].m_bIsDown )
         {
         m_pGUIManager->OnKeyDown( validScanCode );
         }
      }*/
   m_CurMousePos = g_pApp->GetMousePosition();
   m_MouseShift = m_CurMousePos - m_LastMousePos;

   /*for( auto pController : m_Controllers )
      {
      pController->VOnTick( deltaSeconds );
      }*/
   if( m_IsMouseLocked )
      {
      g_pApp->ResetMousePosition();
      m_LastMousePos = Point( g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
      }
   else
      {
      m_LastMousePos = m_CurMousePos;
      }
   }

int InputManager::VRegisterController( shared_ptr< IController > pController )
   {
   if( m_Controllers.find( pController ) != m_Controllers.end() )
      {
      ENG_ASSERT( false && "Controller has been registed" );
      return S_FALSE;
      }
   if( m_Controllers.size() >= MAX_ALLOWABLE_CONTROLLERS )
      {
      ENG_ASSERT( false && "Too many controllers have been registered" );
      return S_FALSE;
      }
      
   m_Controllers.insert( pController );
   return S_OK;
   }

int InputManager::VDeregisterController( shared_ptr< IController > pController )
   {
   if( m_Controllers.find( pController ) == m_Controllers.end() )
      {
      ENG_ASSERT( false && "Controller has not been registed" );
      return S_FALSE;
      }
   m_Controllers.erase( pController );
   return S_OK;
   }

bool InputManager::VIsControllerRegistered( shared_ptr< IController > pController ) const
   {
   return m_Controllers.find( pController ) != m_Controllers.end();
   }