////////////////////////////////////////////////////////////////////////////////
// Filename: Controller.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Controller.h"

MovementController::MovementController( shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown ) : m_object( object )
   {
	m_object->VGetProperties()->GetTransform( &m_ToWorld, &m_FromWorld );

  // m_Position = m_ToWorld.GetPosition();
   m_Rotation = m_ToWorld.GetPitchYawRoll();
   m_TargetRotation = m_Rotation;
	m_TargetYaw = m_Yaw = RADIANS_TO_DEGREES( -initialYaw );
	m_TargetPitch = m_Pitch = RADIANS_TO_DEGREES( initialPitch );

	m_MaxSpeed = 20.0f;			// 30 meters per second
	m_CurrentSpeed = 0.0f;

	Vec3 pos = m_ToWorld.GetPosition();

	m_Position.BuildTranslation( pos );

   memset( &m_KeyButton[0], 0x00, sizeof( bool ) * SDL_NUM_SCANCODES );
   int x, y;
   SDL_GetMouseState( &x, &y );
   m_LastMousePos = Point( x, y );
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
			m_TargetYaw += motion.x * 0.001f;
			m_TargetPitch += motion.y * 0.001f;
		   }
	   }
	else
	   {
      int x, y;
      SDL_GetMouseState( &x, &y );
      m_TargetRotation.y += 0.001f * ( motion.x - g_pApp->GetScreenSize().x / 2.0f ) ;
	   m_TargetRotation.x += 0.001f * ( g_pApp->GetScreenSize().y / 2.0f - motion.y );
      m_LastMousePos = motion;
      SDL_WarpMouseInWindow( g_pApp->GetWindow(), g_pApp->GetScreenSize().GetX() / 2, g_pApp->GetScreenSize().GetY() / 2 );
	   }
	return true;
   }

//  class MovementController::OnUpdate			- Chapter 10, page 283
//  Update object movement in this function
//  This function is updated in human view
void MovementController::OnUpdate( const unsigned long deltaMilliseconds )
   {
	bool bTranslating = false;
	Vec4 atWorld( 0.f, 0.f, 0.f,0.f );
	Vec4 rightWorld( 0.f, 0.f, 0.f, 0.f );
	Vec4 upWorld( 0.f, 0.f, 0.f, 0.f );
	if ( m_KeyButton[ SDL_SCANCODE_W ] || m_KeyButton[ SDL_SCANCODE_S ] )
	   {
		// In D3D, the "look at" default is always
		// the positive Z axis.
      Vec4 at = g_Forward4; 
		if ( m_KeyButton[ SDL_SCANCODE_S ] )
			at *= -1;

		// This will give us the "look at" vector 
		// in world space - we'll use that to move
		// the camera.
		atWorld = m_ToWorld.Xform( at );
		bTranslating = true;
	   }

	if ( m_KeyButton[ SDL_SCANCODE_A ] || m_KeyButton[ SDL_SCANCODE_D ])
	   {
		// In D3D, the "right" default is always
		// the positive X axis.
		Vec4 right = g_Right4; 
		if ( m_KeyButton[ SDL_SCANCODE_A ] )
			right *= -1;

		// This will give us the "right" vector 
		// in world space - we'll use that to move
		// the camera
		rightWorld = m_ToWorld.Xform( right );
		bTranslating = true;
	   }

	if ( m_KeyButton[ SDL_SCANCODE_SPACE ] || m_KeyButton[ SDL_SCANCODE_C ] || m_KeyButton[ SDL_SCANCODE_X ])
	   {
		// In D3D, the "up" default is always
		// the positive Y axis.
		Vec4 up = g_Up4; 
		if ( !m_KeyButton[ SDL_SCANCODE_SPACE ] )
			up *= -1;

		//Unlike strafing, Up is always up no matter
		//which way you are looking
		upWorld = up;
		bTranslating = true;
	   }

	//Handling rotation as a result of mouse position
	   {
		// The secret formula!!! Don't give it away!
		//If you are seeing this now, then you must be some kind of elite hacker!
      m_Rotation.y += ( m_TargetRotation.y - m_Rotation.y ) * ( .5f );
      m_TargetRotation.x = std::max(-90.0f, std::min(90.0f, m_TargetRotation.x));
		m_Rotation.x += (m_TargetRotation.x - m_Rotation.x ) * ( .5f );

		// Calculate the new rotation matrix from the camera
		// yaw and pitch.
		Mat4x4 matRot;
		matRot.BuildPitchYawRollRad( m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// Create the new object-to-world matrix, and the
		// new world-to-object matrix.
		m_ToWorld = m_Position * matRot;
		m_FromWorld = m_ToWorld.Inverse(); 
		m_object->VSetTransform( &m_ToWorld, &m_FromWorld );
	   }

	if ( bTranslating )
	   {
		float elapsedTime = (float)deltaMilliseconds / 1000.0f;

		Vec3 direction = atWorld + rightWorld + upWorld;
		direction.Normalize(); 

		// Ramp the acceleration by the elapsed time.
		float numberOfSeconds = 5.f;
		m_CurrentSpeed += m_MaxSpeed * ( (elapsedTime * elapsedTime) / numberOfSeconds);
		if ( m_CurrentSpeed > m_MaxSpeed )
			m_CurrentSpeed = m_MaxSpeed;

		direction *= m_CurrentSpeed;

		Vec3 pos = m_Position.GetPosition() + direction;
		m_Position.SetPosition(pos);
		m_ToWorld.SetPosition(pos);

		m_FromWorld = m_ToWorld.Inverse();
		m_object->VSetTransform( &m_ToWorld, &m_FromWorld );

      
	   }
	else
	   {
		m_CurrentSpeed = 0.0f;
	   }
   }