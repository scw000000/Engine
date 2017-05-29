#pragma once
/*!
 * \file HumanView.h
 * \date 2016/05/19 21:09
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

#include "PromptBox.h"
#include "..\Controller\MovementController.h"

class UserInterface;

typedef std::list< shared_ptr< IScreenElement > > ScreenElementList;

class HumanView : public IView
   {
	friend class EngineApp;

   public:
      HumanView( void );
      virtual ~HumanView( void );
      virtual void VOnRender( double fTime, float fElapsedTime ) override;
	   virtual ViewType VGetType() override { return View_Human; }
	   virtual ViewId VGetId( void ) const override { return m_ViewId; }

	   virtual void VOnAttach( ViewId vid ) override { m_ViewId = vid; }
      virtual int VOnRestore( void ) override;
      virtual int VOnLostDevice( void );
      virtual int VOnMsgProc( SDL_Event event ) override; // process the OS event
	   virtual void VOnUpdate( float deltaSeconds ) override;
      
      void AttachLayout( shared_ptr<UserInterface> pUI );
      void DetachLayout( void );

      bool HasModalWindow( void );
      Uint32 GetModalEventType( void );
	   //// Virtual methods to control the layering of interface elements
	   //virtual void VPushElement( shared_ptr<IScreenElement> pElement );
	   //virtual void VRemoveElement( shared_ptr<IScreenElement> pElement );

      ProcessManager* GetProcessManager( void ) { return m_pProcessManager; }
	   //void TogglePause(bool active);
      int Ask( MessageBox_Questions question );
      
      bool LoadLevel( TiXmlElement* pLevelData );

   public:
      shared_ptr<IController> m_pController; 
      shared_ptr<UserInterface> m_pUserInterface;

   protected:
	   ViewId m_ViewId;
      Uint64 m_currTick; 
      Uint64 m_lastDraw; // last time the game rended
      bool m_RunFullSpeed; // if the view should ignore max rendering frequency setting
      ProcessManager *m_pProcessManager; // this Processmanager is for things like button animations
      shared_ptr<Scene> m_pWorld; // this scene will be pushed into screen element list by LoadGame( xml level)
      shared_ptr<CameraNode> m_pCamera;
      

   private:

   };


