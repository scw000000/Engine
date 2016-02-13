////////////////////////////////////////////////////////////////////////////////
// Filename: Initialization.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "Initialization.h"

bool CheckForJoystick(std::string hWnd)
   {
   return false;
   }

EngineOptions::EngineOptions()
   {
   m_ScreenWidth = 0;
   m_ScreenHeight = 0;
   }

void EngineOptions::Init()
   {
   m_ScreenWidth = 800;
   m_ScreenHeight = 600;
   }