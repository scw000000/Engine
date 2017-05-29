#pragma once
/*!
 * \file InputManager.h
 * \date 2017/05/28 15:17
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "CEGUI/CEGUI.h"
#include "../UserInterface/GUIManager.h"

struct ButtonState
   {
   public:
      ButtonState( void ) :
         m_bIsDown( false )
         , m_bIsMasked( false )
         { }

   public:
      bool m_bIsDown;
      bool m_bIsMasked;
   };

REGISTER_SINGLETON( InputManager );

class InputManager : public IInputManager
   {
   friend class EngineApp;
   EXPOSE_CONSTRUCTOR( InputManager );
   public:
      int Init( GUIManager* pManager );

      // Keyboard 
      virtual void VOnKeyDown( const SDL_Scancode& keyCode )  override { m_KeyState[ keyCode ].m_bIsDown = true; }
      virtual void VOnKeyUp( const SDL_Scancode& keyCode ) override { m_KeyState[ keyCode ].m_bIsDown = false; }

      // Mouse
      virtual void VOnMouseMove( Point motion ) override;
      virtual void VOnMouseButtonDown( Uint8 button ) override { m_MouseButtonState[ button ].m_bIsDown = true; }
      virtual void VOnMouseButtonUp( Uint8 button ) override { m_MouseButtonState[ button ].m_bIsDown = false; }
      virtual void VSetCursorLocked( bool isLocked ) override { m_IsMouseLocked = isLocked; };
      virtual bool VGetIsCursorLocked( void ) const override { return m_IsMouseLocked; };

      virtual bool VOnMsgProc( const SDL_Event& event ) override;
      virtual void VOnTick( float deltaSeconds ) override;

      virtual int VRegisterController( shared_ptr< IController > pController ) override;
      virtual int VDeregisterController( shared_ptr< IController > pController ) override;
      virtual bool VIsControllerRegistered( shared_ptr< IController > pController ) const override;

      virtual bool VIsKeyDown( const SDL_Scancode& keyCode ) const override { return m_KeyState[ keyCode ].m_bIsDown && m_KeyState[ keyCode ].m_bIsMasked == false; }
      virtual bool VIsMouseButtonDown( Uint8 button ) const override { return m_MouseButtonState[ button ].m_bIsDown && m_MouseButtonState[ button ].m_bIsMasked == false; }

      virtual const Point& VGetMouseShift( void ) const { return m_MouseShift; };
      virtual const Point& VGetMousePosition( void ) const { return m_CurMousePos; };
   private:
      InputManager( void ) { m_pGUIManager = nullptr; };

   private:
      ButtonState					m_KeyState[ SDL_NUM_SCANCODES ];			// Which keys are up and down
      ButtonState              m_MouseButtonState[ 256 ];
      bool              m_IsMouseLocked;
      Point             m_LastMousePos;
      Point             m_CurMousePos;
      Point             m_MouseShift;
      
      std::unordered_set< shared_ptr< IController > > m_Controllers;
      GUIManager*       m_pGUIManager;
      static std::vector< SDL_Scancode > s_ValidScanCodes;
   private:
   };

