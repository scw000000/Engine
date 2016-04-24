#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Controller.h
////////////////////////////////////////////////////////////////////////////////

#include "..\Graphics\SceneNodes.h"

class MovementController : public IController
   {
   public:
      MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown);
      virtual bool VOnMsgProc( const SDL_Event& event ) override ;
	   virtual bool VOnPointerMove( Point motion ) override;
	   virtual bool VOnPointerButtonDown( Uint8 button  ) override{ m_MouseButton[button] = true; return true; }
      virtual bool VOnPointerButtonUp( Uint8 button ) override{ m_MouseButton[button] = false; return true; }

	   virtual bool VOnKeyDown( const SDL_Scancode& keyCode ) override { m_KeyButton[keyCode] = true; return true; }
	   virtual bool VOnKeyUp( const SDL_Scancode& keyCode ) override { m_KeyButton[keyCode] = false; return true; }

	   const Mat4x4 *GetToWorld() { return &m_FromWorld; }
	   const Mat4x4 *GetFromWorld() { return &m_ToWorld; }	

	   
	   void SetObject(shared_ptr<SceneNode> newObject);

	   void OnUpdate( const unsigned long elapsedMs);

    protected:
      Vec3     m_Rotation;
      Vec3     m_TargetRotation;
	   Mat4x4  m_FromWorld;
	   Mat4x4	m_ToWorld;
      Mat4x4  m_Position;

	   Point					m_LastMousePos;
	   bool					m_KeyButton[ SDL_NUM_SCANCODES ];			// Which keys are up and down
      bool              m_MouseButton[ 256 ];
	   // Orientation Controls
	   float		m_TargetYaw;
	   float		m_TargetPitch;
	   float		m_Yaw;
	   float		m_Pitch;
	   float		m_PitchOnDown;
	   float		m_YawOnDown;
	   float		m_MaxSpeed;
	   float		m_CurrentSpeed;

	   // Added for Ch19/20 refactor
	   bool		m_isMouseLButtonDown;
      bool		m_isRotateWhenLButtonDown;

	   shared_ptr<SceneNode> m_object;
   };