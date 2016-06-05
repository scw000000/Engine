/*!
 * \file EditorController.cpp
 * \date 2016/06/05 11:44
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
#include "EditorController.h"



EditorController::EditorController( shared_ptr<SceneNode> object,
                                        float initialYaw,
                                        float initialPitch,
                                        bool rotateWhenLButtonDown,
                                        bool isMouseLocked,
                                        float smoothness ) :
                                        Controller( isMouseLocked ),
                                        m_object( object ),
                                        m_pTransform( ENG_NEW Transform( object->VGetProperties().GetTransform() ) )
   {
   m_MaxSpeed = 40.0f / 1000.f;			// 40 meters per Ms
   m_CurrentSpeed = 0.0f;
   m_MsToMaxSpeed = 1500.f;
   m_Smoothness = std::max( 0.0f, std::min( 0.99f, smoothness ) );
   m_isRotateWhenLButtonDown = rotateWhenLButtonDown;
   }

//  class MovementController::OnUpdate			- Chapter 10, page 283
//  Update object movement in this function
//  This function is updated in human view
void EditorController::VOnUpdate( unsigned long deltaMilliseconds )
   {
   if( !m_isRotateWhenLButtonDown || ( m_isRotateWhenLButtonDown && ( m_MouseButton[ SDL_BUTTON_LEFT ]  )) )
      {
      m_TargetRotShift.x += 0.001f * ( m_MouseShift.y );
      m_TargetRotShift.y += 0.001f * ( -m_MouseShift.x );
      SDL_WarpMouseInWindow( g_pApp->GetWindow(), m_LastMousePos.x, m_LastMousePos.y );
      m_CurMousePos = m_LastMousePos;
      }

   bool bTranslating = false;
   Vec3 direction( 0.0f, 0.0f, 0.0f );
   if( m_KeyButton[ SDL_SCANCODE_W ] || m_KeyButton[ SDL_SCANCODE_S ] )
      {
      direction += ( m_KeyButton[ SDL_SCANCODE_W ] ) ? g_Forward : -1.0f * g_Forward;
      bTranslating = true;
      }

   if( m_KeyButton[ SDL_SCANCODE_A ] || m_KeyButton[ SDL_SCANCODE_D ] )
      {
      direction += ( m_KeyButton[ SDL_SCANCODE_D ] ) ? g_Right : -1.0f * g_Right;
      bTranslating = true;
      }

   if( m_KeyButton[ SDL_SCANCODE_SPACE ] || m_KeyButton[ SDL_SCANCODE_C ] || m_KeyButton[ SDL_SCANCODE_X ] )
      {
      direction += ( m_KeyButton[ SDL_SCANCODE_C ] ) ? -1.0f * g_Up : g_Up;
      bTranslating = true;
      }

   if( m_KeyButton[ SDL_SCANCODE_UP ] || m_KeyButton[ SDL_SCANCODE_DOWN ] )
      {
      if( m_KeyButton[ SDL_SCANCODE_UP ] )
         {
         m_TargetRotShift.x += 0.05f * ( 1.0f );
         }
      else
         {
         m_TargetRotShift.x += 0.05f * ( -1.0f );
         }

      }

   if( m_KeyButton[ SDL_SCANCODE_LEFT ] || m_KeyButton[ SDL_SCANCODE_RIGHT ] )
      {
      if( m_KeyButton[ SDL_SCANCODE_LEFT ] )
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

   if( bTranslating )
      {
      float elapsedTime = ( float ) deltaMilliseconds / 1000.0f;
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