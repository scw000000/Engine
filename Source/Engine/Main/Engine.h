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
      ~EngineApp();
      static void Init( void );
      void InitInstance( void );

      void Run();
      void AbortGame() { m_bQuitting = true; }

      BaseGameLogic *VCreateGameAndView();
      BaseGameLogic* GetGameLogic(void) const { return m_pGame; }
   public:
      struct EngineOptions m_EngineOptions;
      BaseGameLogic *m_pGame;
   protected:

   private:
      EngineApp();
      void InitSystems();
      void EngineLoop();
      static void MsgProc();
      static void OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext  );
      void OnClose();
      

   private:
      bool m_bQuitting;
      bool m_bReadyToQuit;
      SDL_Window *m_window;

      

   };

extern EngineApp *g_pApp;

#endif