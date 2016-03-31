#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MessageBox.h
////////////////////////////////////////////////////////////////////////////////

#include "UserInterface.h"

class MessageBox : public BaseUI
   {
   public:
      MessageBox( std::wstring msg, std::wstring title, int buttonFlags = MB_OK );
      ~MessageBox();
      virtual int VOnRestore();
      virtual int VOnRender( double fTime, float fElapsedTime );
      virtual int VGetZOrder() const { return 99; }
      virtual void VSetZorder( const int zOrder ) { }
      virtual bool VIsVisible() const { return true; }
      virtual void VSetVisible( const bool isVisible ) { }
      virtual int VOnMsgProc( const SDL_Event event );
      static void OnGUIEvent( UINT nEvent, int nControlID );
      static int Ask( MessageBox_Questions question ); // this static function new a MessageBox based on qustion

   protected:
      SDL_MessageBoxData m_UIData;
   };



