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
	~EngineOptions() { SAFE_DELETE(m_pDoc); }

   // TiXmlElement - look at this to find other options added by the developer
	TiXmlDocument *m_pDoc;


	// Rendering options
	std::string m_Renderer;
	bool m_runFullSpeed;
	Point m_ScreenSize;

	

   void Init(const char* xmlFileName);
};


#endif // !_INITIALIZATION_H_
