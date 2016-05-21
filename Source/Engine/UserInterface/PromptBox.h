#pragma once
/*!
 * \file Dialog.h
 * \date 2016/05/15 19:15
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

#include "UserInterface.h"
#include "CEGUI/CEGUI.h"

// 

enum MessageBox_Questions 
   {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
   };

class CEGUI::Window;
class CEGUI::FrameWindow;
  
class PromptBox
   {
   friend class GUIManager;
   public:  
      PromptBox( CEGUI::Window* pRoot, Uint32 eventType,const std::wstring& msg, const std::wstring& title, int buttonFlags = MB_OK);
      ~PromptBox( void );
      CEGUI::FrameWindow* GetWindow( void ) { return m_pWindow; }

   protected:
      bool OnButtonClicked( const CEGUI::EventArgs& arg );
      
   protected:
      CEGUI::FrameWindow* m_pWindow;
      Uint32 m_EventType;  
    };

// deprecated
//// This calss is made for letting Human view treating it like one of screen element in sceen element list 
//class ScreenElementScene : public IScreenElement, public Scene
//   {
//   public:
//	   ScreenElementScene(shared_ptr<IRenderer> renderer) : Scene( renderer ) { }
//      virtual ~ScreenElementScene(void) { ENG_WARNING("~ScreenElementScene()");  }
//
//	   // IScreenElement Implementation
//	   virtual void VOnUpdate( const unsigned long deltaMs ) override { OnUpdate( deltaMs ); };
//	   virtual int VOnRestore() override { OnRestore(); return S_OK; }
//	   virtual int VOnRender( double fTime, float fElapsedTime ) override { OnRender(); return S_OK; }
//	   virtual int VOnLostDevice() override { OnLostDevice(); return S_OK; } 
//	   virtual int VGetZOrder() const override { return 0; }
//	   virtual void VSetZOrder(int const zOrder) override { }
//
//	   // Don't handle any messages
//	   virtual int VOnMsgProc( const SDL_Event& event ) override { return 0; }
//
//	   virtual bool VIsVisible() const override { return true; }
//	   virtual void VSetVisible( const bool visible) override { }
//
//      // expanded function for adding actor to scene
//	   virtual bool VAddChild( ActorId id, shared_ptr<ISceneNode> kid ) { return Scene::AddChild(id, kid); }
//   };