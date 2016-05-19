#pragma once
/*!
 * \file UserInterface.h
 * \date 2016/05/19 16:23
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
#include "CEGUI/CEGUI.h"
#include "..\ResourceCache\ResourceCache.h"
#include "PromptBox.h"
#include "GUIManager.h"

enum MessageBox_Questions;
class GUIManager;

class IUserInterface {
   public:
      virtual void VInit( void ) = 0;
      virtual void VSetDisplayMouseCursor( bool isDisplay ) = 0;
      virtual int  VAsk( MessageBox_Questions question ) = 0;
      virtual bool VHasModalWindow( void ) = 0;

   private:
   };

class UserInterface : public IUserInterface
   {
   public:
      UserInterface( const Resource& layoutRes );
      ~UserInterface( void );
      virtual void VInit( void ) override;
      virtual void VSetDisplayMouseCursor( bool isDisplay ) override;
      void SetMouseCursor( CEGUI::Window* pWindow, const CEGUI::Image* pImage );
      virtual int VAsk( MessageBox_Questions question ) override;
      virtual bool VHasModalWindow( void ) override;
      Uint32 GetModalEventType( void );

   private:
      int m_HasModalDialog;
      bool m_IsMouseCursorEnable;
      Uint32 m_ModalEventType;
      Resource m_LayoutResource;
      shared_ptr<GUIManager> m_pGUIManager;
      CEGUI::Window* m_pRoot;
      CEGUI::Window* m_pUIRoot;
      CEGUI::Window* m_pPromptRoot;
      CEGUI::Image* m_pMouseArrowImg;
   };