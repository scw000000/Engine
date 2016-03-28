#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Interfaces.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTERFACES_H_
#define _INTERFACES_H_

class Actor;
class ActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;

class IGameLogic
{
public:
	virtual WeakActorPtr VGetActor(const ActorId id)=0;
   virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4 *initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID)=0;
   virtual void VDestroyActor(const ActorId actorId)=0;
	virtual bool VLoadGame(const char* levelResource)=0;
	//virtual void VSetProxy()=0;				
	virtual void VOnUpdate(float time, float elapsedTime)=0;
	virtual void VChangeState(enum BaseGameState newState)=0;
	virtual void VMoveActor(const ActorId id, Mat4x4 const &mat)=0;
};

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

typedef unsigned int GameViewId;
extern const GameViewId gc_InvalidGameViewId;

class IGameView 
{
public:
   virtual long VOnRestore()=0;
	virtual void VOnRender(double fTime, float fElapsedTime)=0;
	virtual GameViewType VGetType()=0;
	virtual GameViewId VGetId() const=0;
	virtual void VOnUpdate(unsigned long deltaMs)=0;
   virtual void VOnAttach(GameViewId vid, ActorId aid)=0;
	virtual ~IGameView() { };
};

typedef std::list<shared_ptr<IGameView> > GameViewList;

class IKeyboardHandler
{
public:
	//virtual bool VOnKeyDown(const BYTE c)=0;
	//virtual bool VOnKeyUp(const BYTE c)=0;
};

class IPointerHandler
{
public:
	//virtual bool VOnPointerMove(const Point &pos, const int radius) = 0;
	//virtual bool VOnPointerButtonDown(const Point &pos, const int radius, const std::string &buttonName) = 0;
	//virtual bool VOnPointerButtonUp(const Point &pos, const int radius, const std::string &buttonName) = 0;
};

#endif // !_INTERFACES_H_
