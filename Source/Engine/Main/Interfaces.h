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

enum Environment 
   {
   Environment_Unknown,
   Environment_Game,
   Environment_Editor
   };

class Actor;
class IActorComponent;
class IRenderComponent;

typedef unsigned int ActorId;
typedef unsigned int ComponentRegisterId;
typedef unsigned int ActorComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentRegisterId INVALID_COMPONENT_REGISTER_ID = 0;
const ActorComponentId INVALID_ACTOR_COMPONENT_ID = 0;

typedef shared_ptr< Actor > StrongActorPtr;
typedef weak_ptr< Actor > WeakActorPtr;
typedef shared_ptr< IActorComponent > StrongActorComponentPtr;
typedef weak_ptr< IActorComponent > WeakActorComponentPtr;

typedef shared_ptr< IRenderComponent > StrongRenderComponentPtr;
typedef weak_ptr< IRenderComponent > WeakRenderComponentPtr;

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
      virtual void VOnUpdate( float deltaSeconds ) = 0;
      virtual void VOnAttach( ViewId vid ) = 0;
   };

class Resource;

class IEngineLogic
   {
   public:
   	virtual WeakActorPtr VGetActor( ActorId id ) = 0;
      virtual StrongActorPtr VCreateActor( shared_ptr< Resource > pActorRes,
                                           TransformPtr pTransform,
                                           ActorId serversActorId ) = 0;
      virtual void VDestroyActor( ActorId actorId ) = 0;
	   virtual bool VLoadLevel( void ) = 0;
	   //virtual void VSetProxy()=0;				
      virtual int VOnRestore( void ) = 0;
	   virtual void VOnMsgProc( SDL_Event event ) = 0;
      virtual void VOnUpdate( float time, float deltaSeconds ) = 0;
      virtual void VOnRender( double fTime, float fElapsedTime ) = 0;
	   virtual void VMoveActor( ActorId id, Mat4x4 const &mat ) = 0;
      virtual void VAddView( shared_ptr<IView> pView ) = 0;
      virtual void VModifyActor( ActorId actorId, TiXmlElement *overrides ) = 0;
      virtual void VSetActorUpdate( bool isUpdatable ) = 0;
      virtual void VSetWorldUpdate( bool isUpdatable ) = 0;
      virtual void VOnFileDrop( const char* filePath, const Point& dropLocation ) = 0;
      virtual void VRenderDiagnostics( void ) const = 0;
      virtual void VClearWorld( void ) = 0;
      virtual void VStartAndPause( void ) = 0;
      virtual void VStop( void ) = 0;
      virtual void VSetIsSimulating( bool isOn ) = 0;
      virtual void VGameStart( void ) = 0;
      virtual void VSaveAllActors() = 0;
      virtual void VSaveActor( ActorId id ) = 0;
      virtual TiXmlElement* VGenerateWorldXML( void ) = 0;
      virtual void VSaveWorld( void ) = 0;
   };  

typedef std::list<shared_ptr<IView> > ViewList;

class IController
   {
      public:
      virtual ~IController( void ) {};
      virtual void VOnTick( float deltaSeconds ) = 0;
   };

class IKeyboardManager
   {
   public:
      virtual void VOnKeyDown( const SDL_Scancode& keyCode ) = 0;
      virtual void VOnKeyUp( const SDL_Scancode& keyCode ) = 0;
   };

class IMouseManager
   {
   public:
      virtual void VOnMouseMove( Point motion ) = 0;
	   virtual void VOnMouseButtonDown( Uint8 button ) = 0;
      virtual void VOnMouseButtonUp( Uint8 button ) = 0;
      virtual void VSetCursorLocked( bool isLocked ) = 0;
      virtual bool VGetIsCursorLocked( void ) const = 0;
   };

class IInputManager : public IKeyboardManager, public IMouseManager
   {
   public:
      virtual ~IInputManager( void ) {}
      virtual bool VOnMsgProc( const SDL_Event& event ) = 0;
      virtual void VOnTick( float deltaSeconds ) = 0;
      virtual int VRegisterController( shared_ptr< IController > pController ) = 0;
      virtual int VDeregisterController( shared_ptr< IController > pController ) = 0;
      virtual bool VIsControllerRegistered( shared_ptr< IController > pController ) const = 0;
      virtual bool VIsKeyDown( const SDL_Scancode& keyCode ) const = 0;
      virtual bool VIsMouseButtonDown( Uint8 button ) const = 0;
      virtual const Point& VGetMouseShift( void ) const = 0;
      virtual const Point& VGetMousePosition( void ) const = 0;
   };

class IJoystickManager
   {
   public:
	   virtual bool VOnButtonDown(const std::string &buttonName, int const pressure)=0;
	   virtual bool VOnButtonUp(const std::string &buttonName)=0;
	   virtual bool VOnJoystick(float const x, float const y)=0;
   };

class IGamepadManager
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

class IResourceExtraData
   {
   public:
      virtual ~IResourceExtraData() {}
      //virtual std::string VToString()=0;
   };

class IVideoResourceExtraData
   {
   public:
      virtual ~IVideoResourceExtraData() {}
      //virtual std::string VToString()=0;
   };

class IVideoResourceLoadingOptions 
   {
   public:
      virtual ~IVideoResourceLoadingOptions(){}
   };

// The resource loader is responsible for manipulate handle and setting its extra data
// when calling VLoadResource
class IResourceLoader
   {
   public:
	   virtual const std::vector< std::string >& VGetPattern( void ) const = 0;
      virtual bool VUseRawFile( void ) const = 0;
      virtual bool VDiscardRawBufferAfterLoad( void ) const = 0;
      virtual bool VAddNullZero( void ) const { return false; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	   virtual int VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) = 0;
      virtual bool VUsePreAllocate( void ) const = 0;
      virtual bool VIsPatternMatch( const char* str ) const = 0;
   };

class VideoResourceHandle;

class IVideoResourceLoader
   {
   public:
      virtual const std::vector< std::string >& VGetPattern( void ) const = 0;
      virtual int VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle ) = 0;
      virtual bool VIsPatternMatch( const char* str ) const = 0;
   };

// Each resource file may contains many resources
class IResourceFile
   {
   public:
	   virtual bool VOpen( void ) = 0;
	   virtual int VGetRawResourceSize( shared_ptr< Resource > pResource ) = 0;
      virtual int VGetRawResource( shared_ptr< Resource > pResource, char *buffer ) = 0;
	   virtual int VGetNumResources( void ) const = 0;
	   virtual std::string VGetResourceName(int num) const = 0;
      virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	   virtual ~IResourceFile() { }
   };

enum RenderGroup
   {
	RenderGroup_0,
	RenderGroup_Static = RenderGroup_0,
	RenderGroup_Actor,
	RenderGroup_Sky,
	RenderGroup_NotRendered,
	RenderGroup_Last
   };

enum ColliderTypes
   {
   ColliderType_Sphere,
   ColliderType_Box,
   ColliderType_Num
   };


class Scene;
class SceneNodeProperties;
class Material;
typedef shared_ptr<Material> MaterialPtr;
class ILightNode;
typedef std::list<shared_ptr<ILightNode> > Lights;

class IRenderer
   {
   public:
      virtual ~IRenderer() {}
      //
      virtual void VShutdown( void ) = 0;
      virtual int VOnRestore( Scene* pScene ) = 0;
	   virtual int VPreRender( void ) = 0;
      virtual int VPostRender( void ) = 0;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) = 0;
	   
      //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void ) = 0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void ) = 0;
      //
   };

class LightManager;

class IMainRenderer : public IRenderer
   {
   public:
      virtual void VLoadLight( Lights& lights ) = 0;
   };

class IRenderManager
   {
   public:
      ~IRenderManager( void ) { }

   public:
      virtual int VInit( void ) = 0;
      virtual int VOnRestore( Scene* pScene ) = 0;
      virtual int VPreRender( void ) = 0;
      virtual int VLightingPass( Scene* pScene ) = 0;
      virtual int VSSAOPass( void ) = 0;
      virtual int VMotionBlurPass( void ) = 0;
      virtual int VBloomPass( void ) = 0;
      virtual int VToneMappingPass( void ) = 0;
      virtual int VPostRender( void ) = 0;
      virtual void VShutDown( void ) = 0;
      virtual IMainRenderer& VGetMainRenderer( void ) = 0;
    //  virtual void VCheckError( void ) const = 0;
   };

class ISceneNode;
struct ShadowVertexInfo;

typedef std::vector< std::shared_ptr< ISceneNode > > SceneNodeList;

class ISceneNode
   {
   public:
	   virtual ~ISceneNode() { };
      
      virtual const SceneNodeProperties& VGetProperties( void ) const = 0;
      virtual SceneNodeProperties& VGetProperties( void ) = 0;

	   virtual void VSetTransformPtr( TransformPtr pNewTransform ) = 0;
      virtual void VSetTransform( const Transform& newTransform ) = 0;
      virtual TransformPtr VGetGlobalTransformPtr( void ) const = 0;

      virtual int VPreUpdate( Scene *pScene ) = 0;

      virtual int VOnUpdate( Scene *pScene, float deltaSeconds ) = 0;
      virtual int VDelegateUpdate( Scene *pScene, float deltaSeconds ) = 0;

	   virtual int VOnRestore( Scene *pScene ) = 0;

	   virtual int VPreRender( Scene *pScene ) = 0;
	   virtual bool VIsVisible( Scene *pScene ) = 0;
	   virtual int VRender( Scene *pScene ) = 0;
	   virtual int VRenderChildren( Scene *pScene )=0;
	   virtual int VPostRender( Scene *pScene ) = 0;

      virtual Vec3 GetToWorldPosition( void ) const = 0;
      virtual void SetToWorldPosition( const Vec3& pos ) = 0;
      virtual Vec3 VGetGlobalPosition( void ) const = 0;

	   virtual bool VAddChild( shared_ptr< ISceneNode > kid )=0;
	   virtual bool VRemoveChild( ActorId id ) = 0;
	   virtual int VOnLostDevice( Scene *pScene ) = 0;

      virtual void VSetParentNode( ISceneNode* pParent ) = 0;
      virtual ISceneNode* VGetParentNode( void ) = 0;
      virtual SceneNodeList& VGetChildrenSceneNodes( void ) = 0;

      virtual ShadowVertexInfo VGetShadowVertexInfo( void ) const = 0;
   };

class IScriptManager
   {
   public:
      virtual ~IScriptManager( void ){ }
      virtual bool VInit( void ) = 0;
      virtual void VExecuteFile( const char* resource ) = 0;
      virtual void VExecuteString( const char* str ) = 0;
   };

class IRigidBody;

class ICollider
   {
   public:
      virtual ~ICollider( void ) {};
      virtual float VGetMass( void ) const = 0;
      virtual void VSetMass( float mass ) = 0;
      virtual void VSetLocalCentroid( const Vec3& newCentriod ) = 0;
      virtual void VSetInertia( const Mat3x3& newInertia ) = 0;
      virtual weak_ptr<IRigidBody> VGetRigidBody( void ) const = 0;
      virtual void VSetRigidBody( weak_ptr<IRigidBody> pRigidBody ) = 0;
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const = 0;
      // The input and output should be in rigid body local space
      virtual Vec3 VSupportMapping( const Vec3& direction ) = 0;
      virtual Vec3 VGetRigidBodySpaceCentroid( void ) const = 0;
      virtual Mat3x3 VGetInertia( void ) const = 0;
   };

class IRigidBody
   {
   public:
      virtual ~IRigidBody() {}
      virtual void VUpdateVelocity( float deltaSecond ) = 0;
      virtual void VMoveForOneTimeStep( float deltaSecond ) = 0;

      virtual void VUpdateOrientation( void ) = 0;
      virtual void VUpdateRigidBodyInfo( void ) = 0;

      virtual void VAddCollider( shared_ptr<ICollider> collider ) = 0;

      virtual Vec3 VTransformToGlobal( const Vec3 &localVec, bool isPoint ) const = 0;
      virtual Vec3 VTransformToLocal( const Vec3 &globalVec, bool isPoint ) = 0;

      virtual void VApplyForceAt( const Vec3& force, const Vec3& globalPosition ) = 0;
      virtual void VApplyForce( const Vec3& force ) = 0;
      virtual void VUpdateGlobalInertia( void ) = 0;

      virtual void VSetWorldTransform( const Transform& transform ) = 0;
   };

class RigidBody;

typedef std::pair< shared_ptr<RigidBody>, shared_ptr<RigidBody> > CollisionPair;
typedef std::vector< CollisionPair > CollisionPairs;

class IBroadphase
   {
   public:
   virtual ~IBroadphase( void ) {};
   virtual void VAddRigidBody( shared_ptr< RigidBody > pRigidbody ) = 0;
   virtual void VRemoveRigidBody( shared_ptr< RigidBody > pRigidbody ) = 0;
   virtual void VCalcualteCollisionPairs( void ) = 0;
   virtual CollisionPairs& VGetCollisionPairs( void ) = 0;
   };

typedef unsigned int BoneId;