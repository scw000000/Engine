#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Engine.h
////////////////////////////////////////////////////////////////////////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Mainloop/Initialization.h"
#include "../Main/BaseGameLogic.h"
#include "../ResourceCache/ResourceCache.h"
#include "../UserInterface/UserInterface.h"

class HumanView;

class EngineApp
   {
   public:
      EngineApp( void );
      ~EngineApp( void );
      
      virtual bool VLoadGame( void );
      void MainLoop( void );
      void AbortGame( void ) { m_bQuitting = true; }
      Uint32  GetWindowState( void );
      BaseGameLogic* GetGameLogic(void) const { return m_pGame; }
      bool IsRunning() { return m_bIsRunning; }

      bool InitInstance( SDL_Window* window, int screenWidth, int screenHeight );
      virtual TCHAR *VGetGameAppDirectory( void ){ return _T("Engine\\base\\0.1"); };
      SDL_Window* GetWindow() { return m_pWindow; }
      HWND GetHwnd( void );
      BaseGameLogic* VCreateGameAndView();
      Point GetScreenSize( void ) const { return m_screenSize; }

      int Modal( shared_ptr<Dialog> pModalScreen, int defaultAnswer );
      HumanView* GetHumanView( void );

      bool LoadStrings( std::string language );
      std::wstring GetString( std::wstring sID );

   public:
      struct EngineOptions m_EngineOptions;
      TCHAR m_saveGameDirectory[MAX_PATH];
      BaseGameLogic *m_pGame;
      ResCache *m_ResCache;

   protected:
      int PumpUntilMessage( Uint32& eventEnd, Sint32& code );
      void FlashWhileMinimized( void );
      int  PushUserEvent( Uint32 eventType, Sint32 code, void* d1 = NULL, void* d2 = NULL );

      void MsgProc( void );
      void OnUpdateGame( double fTime, float fElapsedTime );
      void OnFrameRender( double fTime, float fElapsedTime );
      void OnClose( void );
      /* 
      Why use TCHAR? because we may support multiple languages, some of them 
      will take 2 char instead of one for each character, in this case, we set the char class to ANSI (1) or Unicode (2)
      according to the confing in project properities, and the TCHAR means this char size will change automatically according
      to the project setting, also the ## symbol in the macro _T is token pasting operator, which would turn _T("Unicode") into L"Unicode" 
      if we are using Unicode
      http://www.codeproject.com/Articles/76252/What-are-TCHAR-WCHAR-LPSTR-LPWSTR-LPCTSTR-etc
      */
      virtual TCHAR* VGetGameTitle() { return _T("Engine"); };

   protected:
      bool m_bIsRunning;
      bool m_bQuitting;
      bool m_bQuitRequested;
      Point m_screenSize;					// game screen size
      SDL_Window *m_pWindow;
      Uint32 m_ShutDownEventType;
      std::map<std::wstring,std::wstring> m_textResource;

   private:
 
   };

extern EngineApp *g_pApp;
