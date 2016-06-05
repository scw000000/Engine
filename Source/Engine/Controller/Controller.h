#pragma once
/*!
 * \file Controller.h
 * \date 2016/06/05 14:10
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

class Controller : public IController
   {
   public:
      Controller( bool isMouseLocked );
      virtual void VOnTickUpdate( unsigned long deltaMilliseconds ) final override;
     

   protected:

   protected:
      bool					m_KeyButton[ SDL_NUM_SCANCODES ];			// Which keys are up and down
      bool              m_MouseButton[ 256 ];
      bool              m_IsMouseLocked;
      Point             m_LastMousePos;
      Point             m_CurMousePos;
      Point             m_MouseShift;

   private:     
      virtual bool VOnMsgProc( const SDL_Event& event ) override;
      virtual bool VOnPointerMove( Point motion ) override;
      virtual bool VOnPointerButtonDown( Uint8 button ) override { m_MouseButton[ button ] = true; return true; }
      virtual bool VOnPointerButtonUp( Uint8 button ) override { m_MouseButton[ button ] = false; return true; }
      virtual void VSetPointerLocked( bool isLocked ) override { m_IsMouseLocked = isLocked; };
      virtual bool VIsPointerLocked( void ) override { return m_IsMouseLocked; };

      virtual bool VOnKeyDown( const SDL_Scancode& keyCode ) override { m_KeyButton[ keyCode ] = true; return true; }
      virtual bool VOnKeyUp( const SDL_Scancode& keyCode ) override { m_KeyButton[ keyCode ] = false; return true; }
   };