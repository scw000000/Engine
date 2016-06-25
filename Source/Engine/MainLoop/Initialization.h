#pragma once
/*!
 * \file Initialization.h
 * \date 2016/05/26 11:34
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
   public:
      EngineOptions();
	   ~EngineOptions() { SAFE_DELETE(m_pDoc); }
      
      // Rendering options
      std::string m_Renderer;

      bool m_UseDevDir;
      Environment m_Environment;
      std::string m_AssetsDirectory;
      std::string m_Level;
      std::string m_LevelDirectory;
      std::string m_Layout;
      std::string m_GUIDirectory;
      bool m_ShowMouseCursor;

      // TiXmlElement - look at this to find other options added by the developer
	   TiXmlDocument *m_pDoc;

	   bool m_runFullSpeed;
	   Point m_ScreenSize;
	
      void Init(const char* xmlFileName);
   };


