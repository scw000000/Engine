#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: HumanView.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _HUMANVIEW_H_
#define _HUMANVIEW_H_

class HumanView : public IGameView
{
	friend class EngineApp;

public:
   HumanView();
   virtual ~HumanView();
//virtual void VOnRender(double fTime, float fElapsedTime);
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_ViewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_ViewId = vid; 
		m_ActorId = aid;
	}
   virtual long VOnRestore();

   //virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg ); //process the control input
	//virtual void VOnUpdate(const int deltaMilliseconds );
   
	// Virtual methods to control the layering of interface elements
	//virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	//virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);

	//void TogglePause(bool active);

	//HumanView(shared_ptr<IRenderer> renderer);
protected:
	GameViewId m_ViewId;
	ActorId m_ActorId;

private:

};

#endif // !_HUMANVIEW_H_
