#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: UserInterface.h
////////////////////////////////////////////////////////////////////////////////


#include "..\Main\Interfaces.h"


enum MessageBox_Questions 
   {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
   };


class BaseUI : public IScreenElement
   {
   public:
      BaseUI() { m_bIsVisible = true; m_posX = 0; m_posY = 0; width = 100; height = 100; }
      virtual void VOnUpdate( int ) { }
      virtual bool VIsVisible() const { return m_bIsVisible; }
      virtual void VSetVisiable( const bool visible ) { m_bIsVisible = visible; }

   protected:
      int m_posX;
      int m_posY;
      int width;
      int height;
      optional<int> result;
      bool m_bIsVisible;
   };