////////////////////////////////////////////////////////////////////////////////
// Filename: Controller.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Controller.h"

MovementController::MovementController( shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, float smoothness ) : m_object( object )
   {
   Mat4x4 toWorld;
	m_object->VGetProperties()->GetTransform( &toWorld );
   m_Transform.SetTransform( &toWorld );
   m_Transform2.SetTransform( &toWorld );

	m_MaxSpeed = 30.0f;			// 30 meters per second
	m_CurrentSpeed = 0.0f;
   m_Smoothness = std::max( 0.0f, std::min( 0.99f, smoothness ) );

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
         m_TargetRotShift.x += 0.005f * ( g_pApp->GetScreenSize().GetY() / 2 - motion.y );
         m_TargetRotShift.y += 0.005f * ( motion.x - g_pApp->GetScreenSize().GetX() / 2 ) ;
         SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
		   }
	   }
	else
	   {
      m_TargetRotShift.x += 0.005f * ( g_pApp->GetScreenSize().GetY() / 2 - motion.y );
      m_TargetRotShift.y += 0.005f * ( motion.x - g_pApp->GetScreenSize().GetX() / 2 ) ;    
	   }
   SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
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
      direction += ( m_KeyButton[SDL_SCANCODE_W] )? m_Transform.GetToWorld().GetForward(): -1.0f * m_Transform.GetToWorld().GetForward();
		bTranslating = true;
	   }

	if ( m_KeyButton[ SDL_SCANCODE_A ] || m_KeyButton[ SDL_SCANCODE_D ])
	   {
      direction += ( m_KeyButton[SDL_SCANCODE_D] )? m_Transform.GetToWorld().GetRight(): -1.0f * m_Transform.GetToWorld().GetRight();
	   bTranslating = true;      
      }

	if ( m_KeyButton[ SDL_SCANCODE_SPACE ] || m_KeyButton[ SDL_SCANCODE_C ] || m_KeyButton[ SDL_SCANCODE_X ])
	   {
      direction += (! m_KeyButton[SDL_SCANCODE_C] )? m_Transform.GetToWorld().GetUp(): -1.0f * m_Transform.GetToWorld().GetUp();
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

   /*m_Transform.AddPitchYawRollRad( m_TargetRotShift * ( 1 - m_Smoothness ) );*/

   // m_Transform2.AddYawRad( Vec.y );
   // m_Transform2.AddPitchRad( Vec.x );
 // m_TargetRotShift *= m_Smoothness;
  //
  /* m_Transform2.AddPitchYawRollRad( Vec3( Vec.x, 0.0f, 0.0f ) );
   m_Transform2.AddPitchYawRollRad( Vec3( 0.0f, Vec.y, 0.0f ) );*/

   m_Transform2.AddPitchYawRollRad( m_TargetRotShift * ( 1 - m_Smoothness ) );
   m_TargetRotShift *= m_Smoothness;

  // Vec3 rotation = m_Transform2.GetPitchYawRollRad();
  /*std::cout <<  "y  in local : " <<ToStr( m_Transform2.GetQuaternion().Inverse().XForm( g_Up ) ) << std::endl;
  std::cout <<  "up in world : " <<ToStr( m_Transform2.GetQuaternion().XForm( g_Up ) ) << std::endl;*/
 // std::cout <<  "test        : " <<ToStr( m_Transform2.GetQuaternion().Inverse().XForm( m_Transform2.GetQuaternion().XForm( g_Forward ) ) ) << std::endl;
  // rotation += m_TargetRotShift * ( 1 - m_Smoothness );

  // m_TargetRotShift *= m_Smoothness;
  //// rotation.x = std::max( DEGREES_TO_RADIANS( -89.f ), std::min( DEGREES_TO_RADIANS( 89.f ), rotation.x ) );
  //// rotation.z = .0f;
  // m_Transform.SetPitchYawRollRad( rotation );

   if ( bTranslating )
	   {
		float elapsedTime = (float)deltaMilliseconds / 1000.0f;
		direction.Normalize(); 

		// Ramp the acceleration by the elapsed time.
		float numberOfSeconds = 5.f;
		m_CurrentSpeed += m_MaxSpeed * ( (elapsedTime * elapsedTime) / numberOfSeconds);
		if( m_CurrentSpeed > m_MaxSpeed )
         {
			m_CurrentSpeed = m_MaxSpeed;
         }
		   direction *= m_CurrentSpeed;
		   m_Transform.SetPosition( m_Transform.GetPosition() + direction );
	      }
	   else
	      {
		   m_CurrentSpeed = 0.0f;
	      }

   m_object->VSetTransform( &m_Transform2.GetToWorld() );
   }