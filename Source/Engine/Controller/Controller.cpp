/*!
 * \file Controller.cpp
 * \date 2016/05/05 15:40
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

MovementController::MovementController( shared_ptr<SceneNode> object, 
                                        float initialYaw, 
                                        float initialPitch, 
                                        bool rotateWhenLButtonDown, 
                                        float smoothness ) : 
                                        m_object( object ),
                                        m_pTransform( ENG_NEW Transform( object->VGetProperties().GetTransform() ) )
   {
	m_MaxSpeed = 40.0f / 1000.f;			// 40 meters per Ms
	m_CurrentSpeed = 0.0f;
   m_MsToMaxSpeed = 1500.f;
   m_Smoothness = std::max( 0.0f, std::min( 0.99f, smoothness ) );
   m_isMouseLocked = false;
   memset( &m_KeyButton[0], 0x00, sizeof( bool ) * SDL_NUM_SCANCODES );

	m_isMouseLButtonDown = false;
	m_isRotateWhenLButtonDown = rotateWhenLButtonDown;  
   }

bool MovementController::VOnMsgProc( const SDL_Event& event )
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

//  class MovementController::VOnMouseMove		- Chapter 10, page 282

bool MovementController::VOnPointerMove( Point motion )
   {
	// There are two modes supported by this controller.
   
	if ( m_isRotateWhenLButtonDown )
	   {
		// Mode 1 - rotate the view only when the left mouse button is down
		// Only look around if the left button is down
		if( m_isMouseLButtonDown )
		   {
         m_TargetRotShift.x += 0.001f * ( g_pApp->GetScreenSize().GetY() / 2 - motion.y );
         m_TargetRotShift.y += 0.001f * ( motion.x - g_pApp->GetScreenSize().GetX() / 2 ) ;
         SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
		   }
	   }
	else
	   {
      m_TargetRotShift.x += 0.001f * ( motion.y - g_pApp->GetScreenSize().GetY() / 2 );
      m_TargetRotShift.y += 0.001f * ( g_pApp->GetScreenSize().GetX() / 2 - motion.x ) ;    
	   }
   if( m_isMouseLocked )
      {
      SDL_WarpMouseInWindow( g_pApp->GetWindow( ), g_pApp->GetScreenSize( ).GetX( ) / 2, g_pApp->GetScreenSize( ).GetY( ) / 2 );
      }
   return true;
   }

//  class MovementController::OnUpdate			- Chapter 10, page 283
//  Update object movement in this function
//  This function is updated in human view
void MovementController::OnUpdate( const unsigned long deltaMilliseconds )
   {
	bool bTranslating = false;
   Vec3 direction( 0.0f, 0.0f, 0.0f );
   if ( m_KeyButton[ SDL_SCANCODE_W ] || m_KeyButton[ SDL_SCANCODE_S ] )
	   {
      direction += ( m_KeyButton[SDL_SCANCODE_W] )? g_Forward: -1.0f * g_Forward;
      bTranslating = true;
	   }

	if ( m_KeyButton[ SDL_SCANCODE_A ] || m_KeyButton[ SDL_SCANCODE_D ])
	   {
      direction += ( m_KeyButton[SDL_SCANCODE_D] )? g_Right: -1.0f * g_Right;
	   bTranslating = true;      
      }

	if ( m_KeyButton[ SDL_SCANCODE_SPACE ] || m_KeyButton[ SDL_SCANCODE_C ] || m_KeyButton[ SDL_SCANCODE_X ])
	   {
      direction += ( m_KeyButton[SDL_SCANCODE_C] )? -1.0f * g_Up: g_Up;
	   bTranslating = true;
      }

   if ( m_KeyButton[ SDL_SCANCODE_UP ] || m_KeyButton[ SDL_SCANCODE_DOWN ] )
	   {
      if( m_KeyButton[SDL_SCANCODE_UP] )
         {
         m_TargetRotShift.x += 0.05f * ( 1.0f );
         }
      else
         {
         m_TargetRotShift.x += 0.05f * ( -1.0f );
         }

      }

   if ( m_KeyButton[ SDL_SCANCODE_LEFT ] || m_KeyButton[ SDL_SCANCODE_RIGHT ] )
	   {
      if( m_KeyButton[SDL_SCANCODE_LEFT] )
         {
         m_TargetRotShift.y += 0.05f * ( 1.0f );
         }
      else
         {
         m_TargetRotShift.y += 0.05f * ( -1.0f );
         }

      }
   Vec3 rotVal = m_TargetRotShift * ( 1 - m_Smoothness );
   m_pTransform->AddFromWorldPitchYawRollRad( rotVal.x, 0.0f, rotVal.z );
   m_pTransform->AddToWorldPitchYawRollRad( 0.0f, rotVal.y, 0.0f );

   m_TargetRotShift *= m_Smoothness;

   if ( bTranslating )
	   {
		float elapsedTime = (float)deltaMilliseconds / 1000.0f;
		direction.Normalize(); 
		// Ramp the acceleration by the elapsed time.
		float numberOfSeconds = 2.f;
      // a = maxV * numOfSeconds -> after numOfSecconds the scene node will reach max speed
      m_CurrentSpeed += m_MaxSpeed * ( deltaMilliseconds / m_MsToMaxSpeed );
	   //	m_CurrentSpeed += m_MaxSpeed * ( (elapsedTime * elapsedTime) / numberOfSeconds);
		if( m_CurrentSpeed > m_MaxSpeed )
         {
			m_CurrentSpeed = m_MaxSpeed;
         }
     
		direction *= m_CurrentSpeed * deltaMilliseconds;
      m_pTransform->AddFromWorldPosition( direction );
	   }
	else
	   {
		m_CurrentSpeed = 0.0f;
	   }

   m_object->VSetTransform( *m_pTransform );
   }