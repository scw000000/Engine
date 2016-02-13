#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Initailzation.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _INITIALIZATION_H_
#define _INITIALIZATION_H_


extern bool CheckForJoystick(std::string hWnd);

struct EngineOptions
{
   EngineOptions();
	~EngineOptions() {
      }
	// Level option
	//std::string m_Level;

	// Rendering options
	//std::string m_Renderer;
	//bool m_runFullSpeed;
   int m_ScreenWidth;
   int m_ScreenHeight;
	//Point m_ScreenSize;

	

   void Init();
};


#endif // !_INITIALIZATION_H_
