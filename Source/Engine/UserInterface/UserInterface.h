#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: UserInterface.h
////////////////////////////////////////////////////////////////////////////////


#include "..\Main\Interfaces.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"

enum MessageBox_Questions 
   {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
   };

  
class Dialog
   {
   public:  
      Dialog( CEGUI::Window* pRoot, const std::wstring& msg, const std::wstring& title, int buttonFlags = MB_OK);
      ~Dialog( void );
      virtual int VOnRestore( void ) { return 0; }
      virtual void VSetVisible( const bool isVisible ) { }
      

   protected:
      static CEGUI::Window* CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position = Vec4(), const Vec4& size = Vec4() );
      bool OnButtonClicked( const CEGUI::EventArgs& arg );

   protected:
      CEGUI::FrameWindow* m_pWindow;
       };