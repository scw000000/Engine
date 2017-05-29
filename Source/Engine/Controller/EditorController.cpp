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
#include "..\UserInterface\HumanView.h"
#include "InputManager.h"

EditorController::EditorController( InputManager& manager,
                                    shared_ptr<SceneNode> object,
                                    float initialYaw,
                                    float initialPitch,
                                    bool rotateWhenLButtonDown,
                                    bool isMouseLocked, 
                                    float smoothness) :
                                    Controller( manager ),
                                    m_pControllingObject( object ),
                                    m_pTransform( ENG_NEW Transform( object->VGetProperties().GetLocalTransform() ) )
   {
   m_MaxSpeed = 40.0f / 1000.f;			// 40 meters per Ms
   m_CurrentSpeed = 0.0f;
   m_MsToMaxSpeed = 1.5f;
   m_Smoothness = std::max( 0.0f, std::min( 0.99f, smoothness ) );
   m_isRotateWhenLButtonDown = rotateWhenLButtonDown;
   }

//  class MovementController::OnUpdate			- Chapter 10, page 283
//  Update object movement in this function
//  This function is updated in human view
void EditorController::VOnTick( float deltaSeconds )
   {
   auto& mouseShift = m_InputManager.VGetMouseShift();
   if( !m_isRotateWhenLButtonDown || ( m_isRotateWhenLButtonDown && ( m_InputManager.VIsMouseButtonDown( SDL_BUTTON_LEFT ) ) ) )
      {
      m_TargetRotShift.x += 0.001f * ( ( float ) mouseShift.y );
      m_TargetRotShift.y += -0.001f * ( ( float ) mouseShift.x );
      }
   if( m_isRotateWhenLButtonDown )
      {
      if( m_InputManager.VIsMouseButtonDown( SDL_BUTTON_LEFT ) && ( mouseShift.x || mouseShift.y || m_InputManager.VGetIsCursorLocked() ) )
         {
         m_InputManager.VSetCursorLocked( true );
         g_pApp->SetIsMouseCursorEnable( false );
         }
      else
         {
         m_InputManager.VSetCursorLocked( false );
         g_pApp->SetIsMouseCursorEnable( true );
         }
      }

   bool bTranslating = false;
   Vec3 direction( 0.0f, 0.0f, 0.0f );
   if( m_InputManager.VIsKeyDown( SDL_SCANCODE_W ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_S ) )
      {
      direction += ( m_InputManager.VIsKeyDown( SDL_SCANCODE_W ) ) ? g_Forward : -1.0f * g_Forward;
      bTranslating = true;
      }

   if( m_InputManager.VIsKeyDown( SDL_SCANCODE_A ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_D ) )
      {
      direction += m_InputManager.VIsKeyDown( SDL_SCANCODE_D ) ? g_Right : -1.0f * g_Right;
      bTranslating = true;
      }

   if( m_InputManager.VIsKeyDown( SDL_SCANCODE_SPACE ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_C ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_X ) )
      {
      direction += m_InputManager.VIsKeyDown( SDL_SCANCODE_C ) ? -1.0f * g_Up : g_Up;
      bTranslating = true;
      }

   if( m_InputManager.VIsKeyDown( SDL_SCANCODE_UP ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_DOWN ) )
      {
      if( m_InputManager.VIsKeyDown( SDL_SCANCODE_UP ) )
         {
         m_TargetRotShift.x += 0.05f;
         }
      else
         {
         m_TargetRotShift.x += 0.05f * ( -1.0f );
         }

      }

   if( m_InputManager.VIsKeyDown( SDL_SCANCODE_LEFT ) || m_InputManager.VIsKeyDown( SDL_SCANCODE_RIGHT ) )
      {
      if( m_InputManager.VIsKeyDown( SDL_SCANCODE_LEFT ) )
         {
         m_TargetRotShift.y += 0.05f;
         }
      else
         {
         m_TargetRotShift.y += -0.05f;
         }
      }
      
      m_pTransform->AddFromWorldPitchYawRollRad( m_TargetRotShift.x, 0.0f, m_TargetRotShift.z );
      m_pTransform->AddToWorldPitchYawRollRad( 0.0f, m_TargetRotShift.y, 0.0f );
      m_TargetRotShift = Vec3::g_Zero;

      //Vec3 rotVal = m_TargetRotShift * ( 1.f - m_Smoothness );
      //if( abs( rotVal.x ) >= 0.01f || abs( rotVal.y ) >= 0.01f )
      //   {
      //   
      //   ENG_LOG( "Test", ToStr( rotVal ) );
      //   m_TargetRotShift *= m_Smoothness;
      //   }
      //else
      //   {
      // //  m_TargetRotShift = Vec3::g_Zero;
      //   }

   if( bTranslating )
      {
      direction.Normalize();
      // Ramp the acceleration by the elapsed time.
      float numberOfSeconds = 2.f;
      // a = maxV * numOfSeconds -> after numOfSecconds the scene node will reach max speed
      m_CurrentSpeed += m_MaxSpeed * ( deltaSeconds / m_MsToMaxSpeed );
      //	m_CurrentSpeed += m_MaxSpeed * ( (deltaSeconds * deltaSeconds) / numberOfSeconds);
      if( m_CurrentSpeed > m_MaxSpeed )
         {
         m_CurrentSpeed = m_MaxSpeed;
         }

      direction *= m_CurrentSpeed * deltaSeconds;
      m_pTransform->AddFromWorldPosition( direction );
      }
   else
      {
      m_CurrentSpeed = 0.0f;
      }

   m_pControllingObject->VSetTransform( *m_pTransform );
   }