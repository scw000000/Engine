#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: UserInterface.h
////////////////////////////////////////////////////////////////////////////////


#include "..\Main\Interfaces.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "..\Graphics\Scene.h"

const Uint32 g_QuitNoPrompt = -1;

enum MessageBox_Questions 
   {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
   };

  
class Dialog
   {
   friend class GUIManager;
   public:  
      Dialog( CEGUI::Window* pRoot, Uint32 eventType,const std::wstring& msg, const std::wstring& title, int buttonFlags = MB_OK);
      ~Dialog( void );
      virtual int VOnRestore( void ) { return 0; }
      virtual void VSetVisible( const bool isVisible ) { }
      

   protected:
      static CEGUI::Window* CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position = Vec4(), const Vec4& size = Vec4() );
      bool OnButtonClicked( const CEGUI::EventArgs& arg );

   protected:
      CEGUI::FrameWindow* m_pWindow;
      Uint32 m_EventType;  
    };

// This calss is made for letting Human view treating it like one of screen element in sceen element list 
class ScreenElementScene : public IScreenElement, public Scene
   {
   public:
	   ScreenElementScene(shared_ptr<IRenderer> renderer) : Scene( renderer ) { }
      virtual ~ScreenElementScene(void) { ENG_WARNING("~ScreenElementScene()");  }

	   // IScreenElement Implementation
	   virtual void VOnUpdate( const unsigned long deltaMs ) override { OnUpdate( deltaMs ); };
	   virtual int VOnRestore() override { OnRestore(); return S_OK; }
	   virtual int VOnRender( double fTime, float fElapsedTime ) override { OnRender(); return S_OK; }
	   virtual int VOnLostDevice() override { OnLostDevice(); return S_OK; } 
	   virtual int VGetZOrder() const override { return 0; }
	   virtual void VSetZOrder(int const zOrder) override { }

	   // Don't handle any messages
	   virtual int VOnMsgProc( const SDL_Event& event ) override { return 0; }

	   virtual bool VIsVisible() const override { return true; }
	   virtual void VSetVisible( const bool visible) override { }

      // expanded function for adding actor to scene
	   virtual bool VAddChild( ActorId id, shared_ptr<ISceneNode> kid ) { return Scene::AddChild(id, kid); }
   };