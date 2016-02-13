#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: BasicGameLogic.h
////////////////////////////////////////////////////////////////////////////////


//#include "EngineStd.h"
#include "..\Main\Interfaces.h"
//typedef std::map<ActorId, StrongActorPtr> ActorMap;



class BaseGameLogic : public IGameLogic
{
	friend class EngineApp;						// This is only to gain access to the view list

protected:
	//GCCRandom m_random;								// our RNG
	//ActorMap m_actors;
	ActorId m_LastActorId;
//	BaseGameState m_State;							// game state: loading, running, etc.
	int m_ExpectedPlayers;							// how many local human players
	int m_ExpectedRemotePlayers;					// expected remote human players
	int m_ExpectedAI;								// how many AI players
	int m_HumanPlayersAttached;
	int m_AIPlayersAttached;
	int m_HumanGamesLoaded;
	GameViewList m_gameViews;						// views that are attached to our game
      
public:
   
	BaseGameLogic();
	virtual ~BaseGameLogic();
    bool Init(void);

	ActorId GetNewActorID( void )
	{
		return ++m_LastActorId;
	}
	
	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId=INVALID_ACTOR_ID);
	//virtual void VRemoveView(shared_ptr<IGameView> pView);

   /*
    virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);  // [rez] note: don't store this strong pointer outside of this class
    virtual void VDestroyActor(const ActorId actorId);
    virtual WeakActorPtr VGetActor(const ActorId actorId);
	virtual void VModifyActor(const ActorId actorId, TiXmlElement *overrides);

	virtual void VMoveActor(const ActorId id, Mat4x4 const &mat) {}

   */
protected:

};


