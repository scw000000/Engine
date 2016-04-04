#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "HumanView.h"
#include "GUIManager.h"

const unsigned int SCREEN_MAX_FRAME_RATE = 60;
const Uint64 SCREEN_MIN_RENDER_INTERVAL = ( SDL_GetPerformanceFrequency() / SCREEN_MAX_FRAME_RATE );
const GameViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView( )
   {
	m_pProcessManager = ENG_NEW ProcessManager;

	m_ViewId = gc_InvalidGameViewId;

	m_BaseGameState = BGS_Initializing;		// what is the current game state
   
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
   m_pGUIManager->OnMsgProc( event );
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

	int result = 0;
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
   if( test )
      {
      test = false;
      m_pGUIManager->Ask( QUESTION_QUIT_GAME );
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

