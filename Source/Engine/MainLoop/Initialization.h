#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Initailzation.h
////////////////////////////////////////////////////////////////////////////////


extern bool CheckStorage(const DWORDLONG diskSpaceNeeded);
extern DWORD ReadCPUSpeed();
extern bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);
/*
L:long
P:pointer
C:constant
TSTR: T string
-> long pointer to constant T string
*/
extern bool IsOnlyInstance(LPCTSTR gameTitle);
extern const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory);

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


