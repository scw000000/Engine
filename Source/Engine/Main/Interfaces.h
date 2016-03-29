#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Interfaces.h
////////////////////////////////////////////////////////////////////////////////

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
	   virtual bool VOnKeyDown(const BYTE c)=0;
	   virtual bool VOnKeyUp(const BYTE c)=0;
   };

class IPointerHandler
   {
   public:
	   virtual bool VOnPointerMove(const Point &pos, const int radius) = 0;
	   virtual bool VOnPointerButtonDown(const Point &pos, const int radius, const std::string &buttonName) = 0;
	   virtual bool VOnPointerButtonUp(const Point &pos, const int radius, const std::string &buttonName) = 0;
   };

class IJoystickHandler
   {
   public:
	   virtual bool VOnButtonDown(const std::string &buttonName, int const pressure)=0;
	   virtual bool VOnButtonUp(const std::string &buttonName)=0;
	   virtual bool VOnJoystick(float const x, float const y)=0;
   };

class IGamepadHandler
   {
   public:
	   virtual bool VOnTrigger(const std::string &triggerName, float const pressure)=0;
	   virtual bool VOnButtonDown(const std::string &buttonName, int const pressure)=0;
	   virtual bool VOnButtonUp(const std::string &buttonName)=0;
	   virtual bool VOnDirectionalPad(const std::string &direction)=0;
	   virtual bool VOnThumbstick(const std::string &stickName, float const x, float const y)=0;
   };

class IResourceLoader
   {
   public:
	   virtual std::string VGetPattern() = 0;
	   virtual bool VUseRawFile() = 0;
	   virtual bool VDiscardRawBufferAfterLoad() = 0;
      virtual bool VAddNullZero() { return false; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	   virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) = 0;
   };

class IResourceFile
   {
   public:
	   virtual bool VOpen() = 0;
	   virtual int VGetRawResourceSize(const Resource &r) = 0;
	   virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
	   virtual int VGetNumResources() const = 0;
	   virtual std::string VGetResourceName(int num) const = 0;
      virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	   virtual ~IResourceFile() { }
   };
