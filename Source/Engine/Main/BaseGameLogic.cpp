////////////////////////////////////////////////////////////////////////////////
// Filename: BaseGameLogic.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "EngineStd.h"
#include "BaseGameLogic.h"

BaseGameLogic::BaseGameLogic()
   {
   
   }

BaseGameLogic::~BaseGameLogic()
   {
   
   }

bool BaseGameLogic::Init()
   {
   return true;
   }

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId)
   {
   // This makes sure that all views have a non-zero view id.
	int viewId = static_cast<int>(m_gameViews.size());
	m_gameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
   }