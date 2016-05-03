#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "HumanView.h"
#include "GUIManager.h"
#include "..\Controller\Controller.h"

const unsigned int SCREEN_MAX_FRAME_RATE = 60;
const Uint64 SCREEN_MIN_RENDER_INTERVAL = ( SDL_GetPerformanceFrequency() / SCREEN_MAX_FRAME_RATE );
const GameViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView( shared_ptr<IRenderer> p_renderer )
   {
	m_pProcessManager = ENG_NEW ProcessManager;

	m_ViewId = gc_InvalidGameViewId;

	m_BaseGameState = BGS_Initializing;		// what is the current game state
   
   if ( p_renderer )
	   {
		// Moved to the HumanView class post press
		m_pScene.reset( ENG_NEW ScreenElementScene( p_renderer ) );

		Frustum frustum;
		frustum.Init( ENG_PI/4.0f, 4.0f / 3.0f, 0.1f, 2000.0f );
      
      m_pCamera.reset( ENG_NEW CameraNode( Vec3(0.0f, 0.0f, 0.0f), // position in World Space
								Vec3( 0.0f, 0.0f, 1.0f), // look target
								Vec3( 0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
                        , frustum ) );
		ENG_ASSERT( m_pScene && m_pCamera && _T("Out of memory") );     
		m_pScene->VAddChild( INVALID_ACTOR_ID, m_pCamera );
		m_pScene->SetCamera( m_pCamera );
	   m_pCamera->VOnRestore( &*m_pScene );
      }

   m_pGUIManager = ENG_NEW GUIManager;
   m_pGUIManager->Init( "GUI/" );
   }


HumanView::~HumanView()
{
	while (!m_ScreenElements.empty())
	   {
		m_ScreenElements.pop_front();		
	   }

	SAFE_DELETE( m_pProcessManager );
   SAFE_DELETE( m_pGUIManager );
	}

void HumanView::VOnRender( double fTime, float fElapsedTime )
   {
   m_currTick = SDL_GetPerformanceCounter();
   
   if( m_currTick == m_lastDraw ) // alreay draw in this tick, leave now
      {
      return;
      }

   if( m_runFullSpeed || ( ( m_currTick - m_lastDraw ) > SCREEN_MIN_RENDER_INTERVAL ) )
      {
      
      if( true )
         {
         for( auto it = m_ScreenElements.begin(); it != m_ScreenElements.end(); ++it )
            {
            if( (*it)->VIsVisible() )
               {
               (*it)->VOnRender( fTime, fElapsedTime );
               }
            }
         m_lastDraw = m_currTick;
         }
      }

   // Render GUI last, becaouse its on top of screen
   m_pGUIManager->OnRender( fTime, fElapsedTime );
   }

int HumanView::VOnRestore()
   {
	int hr = 0;
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	   {
	   ( (*i)->VOnRestore() );
	   }
	return hr;
   }

int HumanView::VOnLostDevice()
   {
   int hr = 0;
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	   {
	   ( (*i)->VOnLostDevice() );
	   }
	return hr; 
   }

// let the controller handle the input event
int HumanView::VOnMsgProc( SDL_Event event )
   {
   // Iterate through the screen layers first
	// In reverse order since we'll send input messages to the 
	// screen on top
   if( m_pGUIManager->OnMsgProc( event ) )
      {
      return 1;
      }

   if( m_pController && m_pController->VOnMsgProc( event ) )
      {
      return true;
      }

	for( ScreenElementList::reverse_iterator i=m_ScreenElements.rbegin(); i != m_ScreenElements.rend(); ++i )
	   {
		if ( (*i)->VIsVisible() )
		   {
			if ( (*i)->VOnMsgProc( event ) )
			   {
				return 1;
			   }
		   }
	   }

	// Inject userInput into controller here
   switch ( event.type ) 
	   {
      case WM_KEYDOWN:
			break;
	
        case WM_KEYUP:
			break;

		case WM_MOUSEMOVE:
			break;

		case WM_LBUTTONDOWN:
			break;

		case WM_LBUTTONUP:
			break;

		case WM_RBUTTONDOWN:
			break;

		case WM_RBUTTONUP:
			break;
		case WM_CHAR:
			break;
		default:
			return 0;
	   }

	return 0;
   }

void HumanView::VOnUpdate( const unsigned long deltaMs )
   {
   static bool test = true;
   if( m_pController )
      {
      m_pController->OnUpdate( deltaMs );
      }
   m_pGUIManager->OnUpdate( deltaMs );
   m_pProcessManager->UpdateProcesses( deltaMs );
   for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	   {
	   ( (*i)->VOnUpdate( deltaMs ) );
	   }
   }

void HumanView::VPushElement( shared_ptr<IScreenElement> pElement )
   {
   m_ScreenElements.push_front(pElement);
   }

void HumanView::VRemoveElement( shared_ptr<IScreenElement> pElement )
   {
   m_ScreenElements.remove(pElement);
   }

int HumanView::Ask( MessageBox_Questions question )
   {
   return m_pGUIManager->Ask( question );
   }

// TODO: refactor this method ( unnecessary param )
bool HumanView::LoadGame( TiXmlElement* pLevelData )
   {
   VPushElement( m_pScene );  
   m_pController.reset( ENG_NEW MovementController( m_pCamera, 0, 0, false ) );
   //m_pScene->VOnRestore();
   return true;
   }

