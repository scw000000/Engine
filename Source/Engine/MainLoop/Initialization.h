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

class EngineOptions
   {
   public:
      EngineOptions();
	   ~EngineOptions() { SAFE_DELETE(m_pDoc); }
      void Init( const char* xmlFileName );

   public:
      std::string GetRenderer( void ) const { return m_Renderer; };
      bool GetIsRunFullSpeed( void ) const { return m_RunFullSpeed; }
      Point GetScreenSize( void ) const { return m_ScreenSize; }
      void SetScreenSize( const Point& newSize ) { m_ScreenSize = newSize; }

      bool GetIsUsingDevDirectory( void ) const { return m_UseDevDir; }
      Environment GetEngineEnvironment( void ) const { return m_Environment; }
      std::string GetAssetsDirectory( void ) const { return m_AssetsDirectory; }
   //   std::string GetLevelDirectory( void ) const { return m_LevelDirectory; }
   //   std::string GetLevelInstanceDirectory( void ) const { return m_LevelInstanceDirectory; }
      std::string GetFullLevelDirectory( void ) const { return m_LevelDirectory + m_LevelInstanceDirectory; }
      void SetLevelInstanceDirectory( const std::string& newDir ) { m_LevelInstanceDirectory = newDir; }
      std::string GetGUIDirectory( void ) const { return m_GUIDirectory; }
      std::string GetFullActorInstanceDirectory( void ) const { return GetFullLevelDirectory() + m_ActorInstanceDirectory; };
      std::string GetGUILayoutFieName( void ) const { return m_Layout; }
      bool GetShowMouseCursor( void ) const { return m_ShowMouseCursor; }
      TiXmlDocument* GetEngineOptionDoc( void ) const { return m_pDoc; }

   protected:
      // Rendering options
      std::string m_Renderer;
      bool m_RunFullSpeed;
      Point m_ScreenSize;

      bool m_UseDevDir;
      Environment m_Environment;
      std::string m_AssetsDirectory;
      std::string m_LevelDirectory;
      std::string m_LevelInstanceDirectory;
      std::string m_ActorInstanceDirectory;
      std::string m_GUIDirectory;
      std::string m_Layout;
      bool m_ShowMouseCursor;

      // TiXmlElement - look at this to find other options added by the developer
	   TiXmlDocument *m_pDoc;
   };


