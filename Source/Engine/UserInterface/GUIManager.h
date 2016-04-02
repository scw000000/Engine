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

      void OnRender( double fTime, float fElapsedTime );

      static int Ask( MessageBox_Questions question );

   protected:
      static CEGUI::Window* CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position = Vec4(), const Vec4& size = Vec4() );

     // MessageBox* CreateWidget( const std::string &type, const std::string& name = "" );

   protected:
      static  CEGUI::OpenGL3Renderer* s_pRenderer;
      static  CEGUI::DefaultResourceProvider* s_pResProvider;
      CEGUI::GUIContext* m_pContext;
      CEGUI::Window* m_pRoot;
      CEGUI::Window* m_pPromptRoot;
      CEGUI::Window* m_pUIRoot;
   };