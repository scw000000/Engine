#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: GUIManager.h
////////////////////////////////////////////////////////////////////////////////

#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "UserInterface.h"

typedef shared_ptr<IScreenElement> StrongScreenElementPtr;



class GUIManager
   {
   friend class Dialog;
   public:
      GUIManager( void );
      void Init( const std::string& resourceDirectory );
      void Destory( void );
      
      void LoadScheme( const std::string& schemeFile );
      void SetFont( const std::string& fontFile );

      void OnUpdate( const int deltaMs );
      void OnRender( double fTime, float fElapsedTime );
      int  OnMsgProc( SDL_Event event ); // process the OS event

      int Ask( MessageBox_Questions question );
      bool HasModalDialog( void ){ return m_HasModalDialog != 0; }
      Uint32 GetModalEventType( void ){ return m_ModalEventType; }

   protected:
      static CEGUI::Key::Scan SDLKeyToCEGUIKey( SDL_Keycode key );
      static CEGUI::MouseButton SDLButtonTOCEGUIButton( Uint8 button );

   protected:
      static  CEGUI::OpenGL3Renderer* s_pRenderer;
      static  CEGUI::DefaultResourceProvider* s_pResProvider;
      CEGUI::GUIContext* m_pContext;
      CEGUI::Window* m_pRoot;
      CEGUI::Window* m_pPromptRoot;
      CEGUI::Window* m_pUIRoot;
      Uint32 m_ModalEventType;
      int m_HasModalDialog;
   };