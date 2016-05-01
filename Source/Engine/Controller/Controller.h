#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Controller.h
////////////////////////////////////////////////////////////////////////////////

#include "..\Graphics\SceneNodes.h"

class MovementController : public IController
   {
   public:

      /**
       * @brief  This controller will copy transform of its owner instead of using pointer
       *
       * @param  object shared_ptr<SceneNode> object
       * @param  initialYaw float initialYaw
       * @param  initialPitch float initialPitch
       * @param  rotateWhenLButtonDown bool rotateWhenLButtonDown
       * @param  smoothness float smoothness
       * @return 
       */
      MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, float smoothness = 0.7f);
      virtual bool VOnMsgProc( const SDL_Event& event ) override ;
	   virtual bool VOnPointerMove( Point motion ) override;
	   virtual bool VOnPointerButtonDown( Uint8 button  ) override{ m_MouseButton[button] = true; return true; }
      virtual bool VOnPointerButtonUp( Uint8 button ) override{ m_MouseButton[button] = false; return true; }

	   virtual bool VOnKeyDown( const SDL_Scancode& keyCode ) override { m_KeyButton[keyCode] = true; return true; }
	   virtual bool VOnKeyUp( const SDL_Scancode& keyCode ) override { m_KeyButton[keyCode] = false; return true; }
	   
	   void SetObject(shared_ptr<SceneNode> newObject);

	   void OnUpdate( const unsigned long elapsedMs);

    protected:
      shared_ptr<SceneNode> m_object;
      Vec3     m_TargetRotShift;
      TransformPtr m_pTransform;

	   Point					m_LastMousePos;
	   bool					m_KeyButton[ SDL_NUM_SCANCODES ];			// Which keys are up and down
      bool              m_MouseButton[ 256 ];
	   // Orientation Controls
	   float		m_MaxSpeed;
	   float		m_CurrentSpeed;
      float    m_MsToMaxSpeed;
      float    m_Smoothness;
	   // Added for Ch19/20 refactor
	   bool		m_isMouseLButtonDown;
      bool		m_isRotateWhenLButtonDown;

	   
   };