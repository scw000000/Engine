#pragma once
/*!
 * \file Controller.h
 * \date 2016/06/05 10:04
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

#include "..\Graphics\SceneNode\SceneNodes.h"

//class MovementController : public Controller
//   {
//   public:
//
//      /**
//       * @brief  This controller will copy transform of its owner instead of using pointer
//       *
//       * @param  object shared_ptr<SceneNode> object
//       * @param  initialYaw float initialYaw
//       * @param  initialPitch float initialPitch
//       * @param  rotateWhenLButtonDown bool rotateWhenLButtonDown
//       * @param  smoothness float smoothness
//       * @return 
//       */
//      MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, float smoothness = 0.7f);
//	   
//	   void SetObject(shared_ptr<SceneNode> newObject);
//
//	   virtual void VOnTick( float deltaSeconds ) override;
//
//    protected:
//      shared_ptr<SceneNode> m_object;
//      Vec3     m_TargetRotShift;
//      TransformPtr m_pTransform;
//
//	   bool					m_KeyButton[ SDL_NUM_SCANCODES ];			// Which keys are up and down
//      bool              m_MouseButton[ 256 ];
//      bool              m_isMouseLocked;
//	   // Orientation Controls
//	   float		m_MaxSpeed;
//	   float		m_CurrentSpeed;
//      float    m_MsToMaxSpeed;
//      float    m_Smoothness;
//	   // Added for Ch19/20 refactor
//	   bool		m_isMouseLButtonDown;
//      bool		m_isRotateWhenLButtonDown;
//
//	   
//   };