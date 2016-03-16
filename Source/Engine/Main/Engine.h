#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Engine.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENGINE_H_
#define _ENGINE_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Mainloop/Initialization.h"
#include "../Main/BaseGameLogic.h"



class EngineApp
   {
   public:
      EngineApp();
      ~EngineApp();

      HWND EngineApp::GetHwnd( SDL_Window* window );
      virtual TCHAR *VGetGameAppDirectory(){ return _T("Engine\\base\\0.1"); };
      void MainLoop();
      void AbortGame() { m_bQuitting = true; }
      bool InitInstance( SDL_Window* window, int screenWidth, int screenHeight );
      

      
   public:
      struct EngineOptions m_EngineOptions;
      TCHAR m_saveGameDirectory[MAX_PATH];
      BaseGameLogic *m_pGame;
      

   protected:
      bool m_bIsRunning;
      bool m_bQuitting;
      bool m_bQuitRequested;
      Point m_screenSize;					// game screen size
      SDL_Window *m_window;

   private:
      
      static void MsgProc();
      static void OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  );
      void OnClose();
      
      /* 
      Why use TCHAR? because we may support multiple languages, some of them 
      will take 2 char instead of one for each character, in this case, we set the char class to ANSI (1) or Unicode (2)
      according to the confing in project properities, and the TCHAR means this char size will change automatically according
      to the project setting, also the ## symbol in the macro _T is token pasting operator, which would turn _T("Unicode") into L"Unicode" 
      if we are using Unicode
      http://www.codeproject.com/Articles/76252/What-are-TCHAR-WCHAR-LPSTR-LPWSTR-LPCTSTR-etc
      */
      virtual TCHAR *VGetGameTitle() { return _T("Engine"); };
      BaseGameLogic *VCreateGameAndView();
      BaseGameLogic* GetGameLogic(void) const { return m_pGame; }
   private:
 
   };

extern EngineApp *g_pApp;

#endif