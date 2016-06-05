#pragma once
#include "Controller.h"
/*!
 * \file EditorController.h
 * \date 2016/06/05 11:43
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


class EditorController : public Controller
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
   EditorController( shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, bool isMouseLocked = false, float smoothness = 0.7f );

      void SetObject( shared_ptr<SceneNode> newObject );

   protected:
      virtual void VOnUpdate( unsigned long elapsedMs ) override;

   protected:
      shared_ptr<SceneNode> m_object;
      Vec3     m_TargetRotShift;
      TransformPtr m_pTransform;

      // Orientation Controls
      float		m_MaxSpeed;
      float		m_CurrentSpeed;
      float    m_MsToMaxSpeed;
      float    m_Smoothness;
      // Added for Ch19/20 refactor
      bool		m_isRotateWhenLButtonDown;

   };