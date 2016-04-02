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
      Dialog( CEGUI::Window* pWindow, const std::wstring& msg, const std::wstring& title, int buttonFlags = MB_OK );
      ~Dialog();
      virtual int VOnRestore() { return 0; }
      virtual int VGetZOrder() const { return 99; }
      virtual void VSetZorder( const int zOrder ) { }
      virtual void VSetVisible( const bool isVisible ) { }
      static void OnGUIEvent( UINT nEvent, int nControlID );
      static int Ask( MessageBox_Questions question ); // this static function new a MessageBox based on qustion

   protected:
      CEGUI::Window* m_pWindow;
   };