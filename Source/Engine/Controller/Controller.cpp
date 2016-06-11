/*!
 * \file Controller.cpp
 * \date 2016/06/05 14:10
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
#include "Controller.h"



Controller::Controller( bool isMouseLocked = false ) : m_IsMouseLocked( isMouseLocked )
   {
   m_CurMousePos = m_LastMousePos = g_pApp->GetMousePosition();
   m_MouseShift = Point( 0, 0 );
   memset( &m_KeyButton[ 0 ], 0x00, sizeof( bool ) * SDL_NUM_SCANCODES );
   memset( &m_MouseButton[ 0 ], 0x00, sizeof( bool ) * 256 );
   }

void Controller::VOnTickUpdate( unsigned long deltaMilliseconds )
   {
   m_CurMousePos = g_pApp->GetMousePosition();
   m_MouseShift = m_CurMousePos - m_LastMousePos;
   VOnUpdate( deltaMilliseconds );
   if( m_IsMouseLocked )
      {
      g_pApp->ResetMousePosition();
      m_LastMousePos = Point( g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
      }
   else
      {
      m_LastMousePos = m_CurMousePos;
      }
   m_MouseShift = Point( 0, 0 );
   }

bool Controller::VOnMsgProc( const SDL_Event& event )
   {
   switch( event.type )
      {
      case SDL_KEYDOWN:
            {
            return VOnKeyDown( event.key.keysym.scancode );
            }
      case SDL_KEYUP:
            {
            return VOnKeyUp( event.key.keysym.scancode );
            }
      case SDL_MOUSEMOTION:
            {
            return VOnPointerMove( Point( event.motion.x, event.motion.y ) );
            }
      case SDL_MOUSEBUTTONDOWN:
            {
            return VOnPointerButtonDown( event.button.button );
            }
      case SDL_MOUSEBUTTONUP:
            {
            return VOnPointerButtonUp( event.button.button );
            }
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
         return false;
      }// Switch
   return false;
   }

bool Controller::VOnPointerMove( Point motion )
   {
   m_CurMousePos = motion;
   return true;
   }