#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "HumanView.h"

const GameViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView()
   {
   }


HumanView::~HumanView()
   {
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