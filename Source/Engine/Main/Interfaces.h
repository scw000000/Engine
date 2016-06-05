#pragma once
/*!
 * \file Interfaces.h
 * \date 2016/05/19 19:43
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

enum Renderer
	{
   Renderer_Unknown,
   Renderer_OpenGL,
	Renderer_D3D9,
	Renderer_D3D11
	};

class Actor;
class IActorComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<IActorComponent> StrongActorComponentPtr;
typedef weak_ptr<IActorComponent> WeakActorComponentPtr;

//
// class IScreenElement							- Chapter 10, page 285
//
class IScreenElement
   {
   public:
	   virtual int VOnRestore() = 0;
	   virtual int VOnLostDevice() = 0;
	   virtual int VOnRender( double fTime, float fElapsedTime ) = 0;
	   virtual void VOnUpdate( const unsigned long deltaMs ) = 0;

	   virtual int VGetZOrder() const = 0;
	   virtual void VSetZOrder(int const zOrder) = 0;
	   virtual bool VIsVisible() const = 0;
	   virtual void VSetVisible( const bool visible) = 0;

	   virtual int VOnMsgProc( const SDL_Event& event ) = 0;

	   virtual ~IScreenElement() { };
	   virtual bool const operator <(IScreenElement const &other) { return VGetZOrder() < other.VGetZOrder(); }
   };

typedef unsigned int ViewId;
extern const ViewId gc_InvalidGameViewId;

enum ViewType 
   {
   View_Human,
   View_Remote,
   View_AI,
   View_Recorder,
   View_Other
   };


class IView 
   {
   public:
      virtual int VOnRestore( void ) = 0;
      virtual void VOnRender( double fTime, float fElapsedTime ) = 0;
      virtual int VOnMsgProc( SDL_Event event ) = 0;
      virtual ViewType VGetType( void ) = 0;
      virtual ViewId VGetId( ) const = 0;
      virtual void VOnUpdate( const unsigned long deltaMs ) = 0;
      virtual void VOnAttach( ViewId vid ) = 0;
   };

class IEngineLogic
   {
   public:
   	virtual WeakActorPtr VGetActor( const ActorId id ) = 0;
      virtual StrongActorPtr VCreateActor( const std::string &actorResource, TiXmlElement *overrides, const Mat4x4 *initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID) = 0;
      virtual void VDestroyActor( const ActorId actorId ) = 0;
	   virtual bool VLoadLevel( const char* levelResource ) = 0;
	   //virtual void VSetProxy()=0;				
      virtual int VOnRestore( void ) = 0;
	   virtual void VOnMsgProc( SDL_Event event ) = 0;
      virtual void VOnUpdate( float time, float elapsedTime ) = 0;
      virtual void VOnRender( double fTime, float fElapsedTime ) = 0;
	   virtual void VMoveActor( const ActorId id, Mat4x4 const &mat ) = 0;
      virtual void VAddView( shared_ptr<IView> pView ) = 0;
      virtual void VModifyActor( const ActorId actorId, TiXmlElement *overrides ) = 0;
      virtual void VSetActorUpdate( bool isUpdatable ) = 0;
      virtual void VSetWorldUpdate( bool isUpdatable ) = 0;
   };

typedef std::list<shared_ptr<IView> > ViewList;

class IKeyboardHandler
   {
   public:
	   virtual bool VOnKeyDown( const SDL_Scancode& keyCode ) = 0;
	   virtual bool VOnKeyUp( const SDL_Scancode& keyCode ) = 0;
   };

class IPointerHandler
   {
   public:
	   virtual bool VOnPointerMove( Point motion ) = 0;
	   virtual bool VOnPointerButtonDown( Uint8 button ) = 0;
	   virtual bool VOnPointerButtonUp( Uint8 button ) = 0;
      virtual void VSetPointerLocked( bool isLocked ) = 0;
      virtual bool VIsPointerLocked( void ) = 0;
   };

class IController : public IKeyboardHandler, public IPointerHandler
   {
   public:
      virtual ~IController( void ){}
      virtual bool VOnMsgProc( const SDL_Event& event ) = 0;
      virtual void VOnTickUpdate( unsigned long deltaMilliseconds ) = 0;
      virtual void VOnUpdate( unsigned long deltaMilliseconds ) = 0;
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

class Resource;
class IResourceFile;
class ResHandle;

// The resource loader is responsible for manipulate handle and setting its extra data
// when calling VLoadResource
class IResourceLoader
   {
   public:
	   virtual const std::vector< std::string >& VGetPattern( void ) = 0;
	   virtual bool VUseRawFile( void ) = 0;
	   virtual bool VDiscardRawBufferAfterLoad( void ) = 0;
      virtual bool VAddNullZero( void ) { return false; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	   virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) = 0;
      virtual bool VUsePreAllocate( void ) = 0;
      virtual bool VIsPatternMatch( const char* str ) = 0;
   };

// Each resource file may contains many ressources
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



enum RenderPass
   {
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
   };

class Scene;
class SceneNodeProperties;
class Material;
typedef shared_ptr<Material> MaterialPtr;
class LightNode;
typedef std::list<shared_ptr<LightNode> > Lights;




class IRenderer
   {
   public:
	   virtual void VSetBackgroundColor( Color color ) = 0; 
	   virtual GLuint VOnRestore( void ) = 0;
	   virtual void VShutdown( void ) = 0;
	   virtual bool VPreRender( void ) = 0;
	   virtual bool VPostRender( void ) = 0;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) = 0;
	   
      // Unnecessarry functions for OpenGL
      //virtual void VSetWorldTransform( const Mat4x4 *m )=0;
	   //virtual void VSetViewTransform( const Mat4x4 *m )=0;
	   //virtual void VSetProjectionTransform( const Mat4x4 *m )=0;
	   
      //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void ) = 0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void ) = 0;
	   virtual void VDrawLine( const Vec3& from,const Vec3& to,const Color& color ) const = 0;
   };

class ISceneNode
   {
   public:
	   virtual ~ISceneNode() { };
      
      virtual const SceneNodeProperties& VGetProperties( void ) const = 0;
      virtual SceneNodeProperties& VGetProperties( void ) = 0;

	   virtual void VSetTransformPtr( TransformPtr pNewTransform ) = 0;
      virtual void VSetTransform( const Transform& newTransform ) = 0;

	   virtual int VOnUpdate( Scene *pScene, const unsigned long deltaMs ) = 0;
	   virtual int VOnRestore( Scene *pScene ) = 0;

	   virtual int VPreRender( Scene *pScene ) = 0;
	   virtual bool VIsVisible( Scene *pScene ) = 0;
	   virtual int VRender( Scene *pScene ) = 0;
	   virtual int VRenderChildren( Scene *pScene )=0;
	   virtual int VPostRender( Scene *pScene ) = 0;

      virtual Vec3 GetToWorldPosition( void ) const = 0;
      virtual void SetToWorldPosition( const Vec3& pos ) = 0;

	   virtual bool VAddChild( shared_ptr<ISceneNode> kid )=0;
	   virtual bool VRemoveChild( ActorId id ) = 0;
	   virtual int VOnLostDevice( Scene *pScene ) = 0;

//	   virtual int VPick( Scene *pScene, RayCast *pRayCast ) = 0;
   };

class IGamePhysics 
   {
   public:

      // Initialiazation and Maintenance of the Physics World
      virtual bool VInitialize( ) = 0;
      virtual void VSyncVisibleScene( ) = 0;
      virtual void VOnUpdate( const float deltaSeconds ) = 0;

      // Initialization of Physics Objects
      virtual void VAddSphere( float radius, WeakActorPtr actor, /*const Mat4x4& initialTransform, */const std::string& densityStr, const std::string& physicsMaterial ) = 0;
      virtual void VAddBox( const Vec3& dimensions, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial ) = 0;
      virtual void VAddPointCloud( Vec3 *verts, int numPoints, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial ) = 0;
      virtual void VRemoveActor( ActorId id ) = 0;

      // Debugging
      virtual void VRenderDiagnostics( ) = 0;

      // Physics world modifiers
      virtual void VCreateTrigger( WeakActorPtr pGameActor, const Vec3 &pos, const float dim ) = 0;
      virtual void VApplyForce( const Vec3 &dir, float newtons, ActorId aid ) = 0;
      virtual void VApplyTorque( const Vec3 &dir, float newtons, ActorId aid ) = 0;
      virtual bool VKinematicMove( const Transform &trans, ActorId aid ) = 0;

      // Physics actor states
      virtual void VRotateY( ActorId actorId, float angleRadians, float time ) = 0;
      virtual float VGetOrientationY( ActorId actorId ) = 0;
      virtual void VStopActor( ActorId actorId ) = 0;
      virtual Vec3 VGetVelocity( ActorId actorId ) = 0;
      virtual void VSetVelocity( ActorId actorId, const Vec3& vel ) = 0;
      virtual Vec3 VGetAngularVelocity( ActorId actorId ) = 0;
      virtual void VSetAngularVelocity( ActorId actorId, const Vec3& vel ) = 0;
      virtual void VTranslate( ActorId actorId, const Vec3& vec ) = 0;

      virtual void VSetTransform( const ActorId id, const Transform& trans ) = 0;
      virtual Transform VGetTransform( const ActorId id ) = 0;

      virtual ~IGamePhysics( void ){ };
   };


class IScriptManager
   {
   public:
      virtual ~IScriptManager( void ){ }
      virtual bool VInit( void ) = 0;
      virtual void VExecuteFile( const char* resource ) = 0;
      virtual void VExecuteString( const char* str ) = 0;
   };
