#pragma once
/*!
 * \file GUIManager.h
 * \date 2016/05/19 15:43
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

#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "PromptBox.h"
#include "UserInterface.h"

typedef std::map< CEGUI::Window*, shared_ptr<UserInterface> >Layouts;


class GUIManager
   {
   friend class PromptBox;
   public:
      GUIManager( void );
      void Init( const std::string& resourceDirectory );
      void ShutDown( void );
      void Destory( void );
      
      void LoadScheme( const std::string& schemeFile );
      void SetFont( const std::string& fontFile );

      void OnUpdate( unsigned long deltaMs );
      void OnRender( double fTime, float fElapsedTime );
      void AttachLayout( shared_ptr<UserInterface> pUI );
      void DetachLayout( shared_ptr<UserInterface> pUI );

      int  OnMsgProc( SDL_Event event ); // process the OS event

      CEGUI::GUIContext* GetContext( void ) { return m_pContext; }
      
      static CEGUI::Window* CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position = Vec4(), const Vec4& size = Vec4() );
      void SetDisplayMouseCursor( bool isDisplay );

   protected:
      static CEGUI::Key::Scan SDLKeyToCEGUIKey( SDL_Keycode key );
      static CEGUI::MouseButton SDLButtonTOCEGUIButton( Uint8 button );
      bool OnMouseEntersArea( const CEGUI::EventArgs& arg );

   protected:
      static  CEGUI::OpenGL3Renderer* s_pRenderer;
      static  CEGUI::DefaultResourceProvider* s_pResProvider;
      Layouts m_Layouts;
      CEGUI::GUIContext* m_pContext;
      CEGUI::Window* m_pRoot;
      CEGUI::Window* m_pPromptRoot;
      CEGUI::Window* m_pUIRoot;
      std::string m_ResourceDir;
   };
