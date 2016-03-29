#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "HumanView.h"

const unsigned int SCREEN_MAX_FRAME_RATE = 60;
const unsigned int SCREEN_MIN_RENDER_INTERVAL = ( SDL_GetPerformanceFrequency() / SCREEN_MAX_FRAME_RATE );
const GameViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView()
   {
   }


HumanView::~HumanView()
   {
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
      
      }

   }

long HumanView::VOnRestore()
{
	long hr = 0;
	//for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	//{
//		V_RETURN ( (*i)->VOnRestore() );
	//}

	return hr;
}