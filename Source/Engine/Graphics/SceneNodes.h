#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: SceneNodes.h
////////////////////////////////////////////////////////////////////////////////

#include "Material.h"


class BaseRenderComponent;

typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

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
      void GetRelTransform( Mat4x4* const toRelative, Mat4x4* const fromRelative ) const;

      const char* Name( void ) const { return m_Name.c_str(); }

      //bool HasAlpha( void ) const {};
      // virtual float Alpha( void ) const {}

      RenderPass GetRenderPass( void ) const { return m_RenderPass; }
      float GetRadius( void ) const { return m_Radius; }
      
      const Material& GetMaterial( void ) const {}

   protected:
      ActorId m_ActorId;
      std::string m_Name;
      Mat4x4 m_ToRelSpace;
      Mat4x4 m_FromRelSpace;
      float m_Radius;
      RenderPass m_RenderPass;
      Material m_Material;
      //AlphaType m_AlphaType;

   private:

   };

typedef std::vector< std::shared_ptr< ISceneNode > > SceneNodeList;

class SceneNode : public ISceneNode
   {
	friend class Scene;

   public:
      // TODO: finish constructor
	   SceneNode( ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Mat4x4 *to, const Mat4x4 *from = NULL );

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
      virtual void SetRelPosition( const Vec3 &pos ) { m_Props.m_ToRelSpace.SetPosition( pos ); }

	   Vec3 GetWorldPosition( void ) const;

	   Vec3 GetRelDirection( void ) const { return m_Props.m_ToRelSpace.GetDirection(); }

	   void SetRadius(const float radius) { m_Props.m_Radius = radius; }
	   //void SetMaterial(const Material &mat) { m_Props.m_Material = mat; }

   protected:
	   SceneNodeList			m_Children;
	   SceneNode				*m_pParent;
	   SceneNodeProperties		m_Props;
   	WeakBaseRenderComponentPtr	m_RenderComponent;
   };



struct AlphaSceneNode
   {
	shared_ptr<ISceneNode> m_pNode;
	Mat4x4 m_Concat;
	float m_ScreenZ;

	// For the STL sort...
	bool const operator <(AlphaSceneNode const &other) { return m_ScreenZ < other.m_ScreenZ; }
   };

typedef std::list<AlphaSceneNode *> AlphaSceneNodes;

class RootNode : public SceneNode
   {
   public:
	   RootNode( void );
	   virtual bool VAddChild( shared_ptr<ISceneNode> child );
	   virtual int VRenderChildren( Scene *pScene );
	   virtual bool VRemoveChild( ActorId id );
	   virtual bool VIsVisible( Scene *pScene ) const { return true; }
   };


class CameraNode : public SceneNode
   {
   public:
	   CameraNode( Mat4x4 const *t, Frustum const &frustum ) 
	      : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, t),
	      m_Frustum(frustum),
	      m_IsActive(true),
	      m_IsDebugCamera(false),
         m_pTarget(shared_ptr<SceneNode>()),
	     m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ) { }

	   virtual int VRender( Scene *pScene ) override;
	   virtual int VOnRestore( Scene *pScene ) override;
	   virtual bool VIsVisible( Scene *pScene ) const { return m_IsActive; }

	   const Frustum &GetFrustum( void ) { return m_Frustum; }
	   void SetTarget(shared_ptr<SceneNode> pTarget) { m_pTarget = pTarget; }
	   void ClearTarget( void ) { m_pTarget = shared_ptr<SceneNode>(); }
	   shared_ptr<SceneNode> GetTarget( void ) { return m_pTarget; }

	   Mat4x4 GetWorldViewProjection(Scene *pScene) const;
	   int SetViewTransform(Scene *pScene);

	   Mat4x4 GetProjection() { return m_Projection; }
	   Mat4x4 GetView( void ) { return m_View; }

	   void SetCameraOffset( const Vec4 & cameraOffset )
	      {
		   m_CamOffsetVector = cameraOffset;
	      }

   protected:
	   Frustum			m_Frustum;
      Mat4x4			m_Projection;
	   Mat4x4			m_View;
	   bool			m_IsActive;
	   bool			m_IsDebugCamera;
	   shared_ptr<SceneNode> m_pTarget;
	   Vec4			m_CamOffsetVector;	//Direction of camera relative to target.
   };