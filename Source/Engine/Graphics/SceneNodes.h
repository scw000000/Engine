#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: SceneNodes.h
////////////////////////////////////////////////////////////////////////////////

//typedef unsigned int ActorId;

//   This enum defines the different types of alpha blending
//   types that can be set on a scene node.
enum AlphaType
   {
	AlphaOpaque,
	AlphaTexture,
	AlphaMaterial,
	AlphaVertex
   };



// TODO: implement rest graphic class such as material, alpha, render component.... 
class SceneNodeProperties
   {
   friend class SceneNode;
   public:
      SceneNodeProperties( void );

      ActorId GetActorId( void ) const { return m_ActorId; }
      const Mat4x4& GetToRelSpace( void ) const { return m_ToRelSpace; }
      const Mat4x4& GetFromRelSpace( void ) const { return m_FromRelSpace; }
      void Transform( Mat4x4* const toRelative, Mat4x4* const fromRelative ) const;

      const char* Name( void ) const { return m_Name.c_str(); }

      //bool HasAlpha( void ) const {};
      // virtual float Alpha( void ) const {}

      RenderPass GetRenderPass( void ) const { return m_RenderPass; }
      float GetRadius( void ) const { return m_Radius; }
      
      //const Material& GetMaterial( void ) const {}

   protected:
      ActorId m_ActorId;
      std::string m_Name;
      Mat4x4 m_ToRelSpace;
      Mat4x4 m_FromRelSpace;
      float m_Radius;
      RenderPass m_RenderPass;
      //Material m_Material;
      //AlphaType m_AlphaType;

   private:

   };

typedef std::vector< std::shared_ptr< ISceneNode > > SceneNodeList;

class SceneNode : public ISceneNode
   {
	friend class Scene;

   public:
	   //SceneNode( ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Mat4x4 *to, const Mat4x4 *from = NULL );

	   virtual ~SceneNode();

	   virtual const SceneNodeProperties* const VGetProperties() const { return &m_Props; }

	   virtual void VSetRelTransform( const Mat4x4 *toRelative, const Mat4x4 *fromRelative = NULL );

	   virtual int VOnRestore( Scene *pScene );
	   virtual int VOnUpdate( Scene *, const unsigned long elapsedMs );

	   virtual int VPreRender( Scene *pScene );
	   virtual bool VIsVisible( Scene *pScene ) const;
	   virtual int VRender( Scene *pScene ) { return S_OK; } 
	   virtual int VRenderChildren( Scene *pScene );
	   virtual int VPostRender( Scene *pScene );

	   virtual bool VAddChild( shared_ptr<ISceneNode> child );
	   virtual bool VRemoveChild( ActorId id );
	   virtual int VOnLostDevice( Scene *pScene );
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);

	   void SetAlpha( float alpha );
	   //float GetAlpha() const { return m_Props.Alpha(); }
      
      // return position in actor space
	   virtual Vec3 GetRelPosition( void ) const { return m_Props.m_ToRelSpace.GetPosition(); }
	   // set position in actor space
      virtual void SeRelPosition( const Vec3 &pos ) { m_Props.m_ToRelSpace.SetPosition( pos ); }

	   Vec3 GetWorldPosition( void ) const;

	   Vec3 GetRelDirection( void ) const { return m_Props.m_ToRelSpace.GetDirection(); }

	   void SetRadius(const float radius) { m_Props.m_Radius = radius; }
	   //void SetMaterial(const Material &mat) { m_Props.m_Material = mat; }

   protected:
	   SceneNodeList			m_Children;
	   SceneNode				*m_pParent;
	   SceneNodeProperties		m_Props;
   //	WeakBaseRenderComponentPtr	m_RenderComponent;
   };

struct AlphaSceneNode
   {
	shared_ptr<ISceneNode> m_pNode;
	Mat4x4 m_Concat;
	float m_ScreenZ;

	// For the STL sort...
	bool const operator <(AlphaSceneNode const &other) { return m_ScreenZ < other.m_ScreenZ; }
   };

class RootNode : public SceneNode
   {
   public:
	   RootNode();
	   virtual bool VAddChild( shared_ptr<ISceneNode> child );
	   virtual int VRenderChildren( Scene *pScene );
	   virtual bool VRemoveChild( ActorId id );
	   virtual bool VIsVisible( Scene *pScene ) const { return true; }
   };

/* TODO: implement render component 
class CameraNode : public SceneNode
{
   public:
	   CameraNode( Mat4x4 const *t, Frustum const &frustum ) 
	      : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, t),
	      m_Frustum(frustum),
	      m_bActive(true),
	      m_DebugCamera(false),
	      m_pTarget(shared_ptr<SceneNode>()),
	      m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ) { }

	   virtual int VRender( Scene *pScene );
	   virtual int VOnRestore( Scene *pScene );
	   virtual bool VIsVisible( Scene *pScene ) const { return m_bActive; }

	   const Frustum &GetFrustum() { return m_Frustum; }
	   void SetTarget(shared_ptr<SceneNode> pTarget) { m_pTarget = pTarget; }
	   void ClearTarget() { m_pTarget = shared_ptr<SceneNode>(); }
	   shared_ptr<SceneNode> GetTarget() { return m_pTarget; }

	   Mat4x4 GetWorldViewProjection(Scene *pScene);
	   int SetViewTransform(Scene *pScene);

	   Mat4x4 GetProjection() { return m_Projection; }
	   Mat4x4 GetView() { return m_View; }

	   void SetCameraOffset( const Vec4 & cameraOffset )
	      {
		   m_CamOffsetVector = cameraOffset;
	      }

   protected:
	   Frustum			m_Frustum;
      Mat4x4			m_Projection;
	   Mat4x4			m_View;
	   bool			m_bActive;
	   bool			m_DebugCamera;
	   shared_ptr<SceneNode> m_pTarget;
	   Vec4			m_CamOffsetVector;	//Direction of camera relative to target.
   };
   */