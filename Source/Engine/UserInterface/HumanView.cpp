#pragma once
/*!
 * \file HumanView.cpp
 * \date 2016/05/19 21:10
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
#include "EngineStd.h"
#include "HumanView.h"
#include "MenuInterface.h"
#include "..\Graphics\Scene.h"
#include "..\Controller\EditorController.h"
#include "..\Controller\InputManager.h"

const unsigned int SCREEN_MAX_FRAME_RATE = 60;
const Uint64 SCREEN_MIN_RENDER_INTERVAL = ( SDL_GetPerformanceFrequency() / SCREEN_MAX_FRAME_RATE );
const ViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView( void )
   {
	m_pProcessManager = ENG_NEW ProcessManager;
	m_ViewId = gc_InvalidGameViewId;
   AttachLayout( shared_ptr<UserInterface>( ENG_NEW UserInterface( Resource( g_pApp->m_EngineOptions.GetGUILayoutFieName(), true ) ) ) );

   if ( g_pApp->m_pEngineLogic->m_pWrold )
	   {
      m_pWorld = g_pApp->m_pEngineLogic->m_pWrold;
		PerspectiveFrustum frustum;
      Point screenSize = g_pApp->m_EngineOptions.GetScreenSize();
      frustum.Init( ENG_PI / 4.0f, ( float ) screenSize.x / ( float ) screenSize.y, 0.1f, 1500.0f );
      
      m_pCamera.reset( ENG_NEW CameraNode( Vec3(-10.0f, 0.0f, -25.0f), // position in World Space
								Vec3( 0.0f, 0.0f, 1.0f), // look target
								Vec3( 0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
                        , frustum ) );
		ENG_ASSERT( m_pWorld && m_pCamera && _T("Out of memory") );     
		m_pWorld->AddChild( INVALID_ACTOR_ID, m_pCamera );
      m_pWorld->SetCamera( m_pCamera );
      }
   m_pController.reset( ENG_NEW EditorController( SInputManager::GetSingleton(), m_pCamera, 0.f, 0.f, true ) );
   }

HumanView::~HumanView()
   {
   DetachLayout();
	SAFE_DELETE( m_pProcessManager );
	}

void HumanView::VOnRender( double fTime, float fElapsedTime )
   {

   m_currTick = SDL_GetPerformanceCounter();
   
   if( m_currTick == m_lastDraw ) // already draw in this tick, leave now
      {
      return;
      }

   if( m_RunFullSpeed || ( ( m_currTick - m_lastDraw ) > SCREEN_MIN_RENDER_INTERVAL ) )
      {
      m_pWorld->SetCamera( m_pCamera );
      m_pWorld->OnRender();
      m_lastDraw = m_currTick;
      }
   }

int HumanView::VOnRestore()
   {
   m_pCamera->VOnRestore( &*m_pWorld );
	return S_OK;
   }

int HumanView::VOnLostDevice()
   {
	return S_OK; 
   }

// let the controller handle the input event
int HumanView::VOnMsgProc( SDL_Event event )
   {
   return 0;

 //  // Iterate through the screen layers first
	//// In reverse order since we'll send input messages to the 
	//// screen on top
 //  
 //  if( m_pController && m_pController->VOnMsgProc( event ) )
 //     {
 //     return true;
 //     }

	//// Inject userInput into controller here
 //  switch ( event.type ) 
	//   {
 //     case WM_KEYDOWN:
	//		break;
	//
 //       case WM_KEYUP:
	//		break;

	//	case WM_MOUSEMOVE:
	//		break;

	//	case WM_LBUTTONDOWN:
	//		break;

	//	case WM_LBUTTONUP:
	//		break;

	//	case WM_RBUTTONDOWN:
	//		break;

	//	case WM_RBUTTONUP:
	//		break;
	//	case WM_CHAR:
	//		break;
	//	default:
	//		return 0;
	//   }

	//return 0;
   }

void HumanView::VOnUpdate( float deltaSeconds )
   {
   static bool test = true;
   if( m_pController )
      {
      m_pController->VOnTick( deltaSeconds );
      }
   m_pProcessManager->UpdateProcesses( deltaSeconds );
   }

void HumanView::AttachLayout( shared_ptr<UserInterface> pUI )
   {
   if( m_pUserInterface )
      {
      DetachLayout();
      }
   m_pUserInterface = pUI;
   m_pUserInterface->VInit();
   g_pApp->m_pEngineLogic->m_pGUIManager->AttachLayout( m_pUserInterface );
   }

void HumanView::DetachLayout( void )
   {
   if( m_pUserInterface )
      {
      g_pApp->m_pEngineLogic->m_pGUIManager->DetachLayout( m_pUserInterface );
      }
   }

bool HumanView::HasModalWindow( void )
   {
   return m_pUserInterface->VHasModalWindow();
   }

 Uint32 HumanView::GetModalEventType( void )
   {
   return m_pUserInterface->GetModalEventType();
   }

int HumanView::Ask( MessageBox_Questions question )
   {
   return m_pUserInterface->VAsk( question );
   }

// TODO: refactor this method ( unnecessary param )
bool HumanView::LoadLevel( TiXmlElement* pLevelData )
   {
   return true;
   }

