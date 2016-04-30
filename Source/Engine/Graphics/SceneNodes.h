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
   //   const Mat4x4& GetToWorld( void ) const { return m_ToWorld; }
      const Mat4x4& GetFromWorld( void ) const { 
         //return m_Transform.GetFromWorld(); 
         return m_Transform.Inverse().GetToWorld();
         }
      Transform GetTransform( void ) const;

      const char* Name( void ) const { return m_Name.c_str(); }

      //bool HasAlpha( void ) const {};
      // virtual float Alpha( void ) const {}

      RenderPass GetRenderPass( void ) const { return m_RenderPass; }
      float GetRadius( void ) const { return m_Radius; }
      
      Material& GetMaterial( void ) { return m_Material; }

   protected:
      ActorId m_ActorId;
      std::string m_Name;
      Transform m_Transform;
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
	   SceneNode( ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Transform& transform = Transform::g_Identity );

	   virtual ~SceneNode();

	   virtual const SceneNodeProperties& VGetProperties() const { return m_Props; }

	   virtual void VSetTransform( const Transform& transform );

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
      
      // return position in parent space
	   virtual Vec3 GetToWorldPosition( void ) const { return m_Props.m_Transform.GetPosition(); }
	   // set position in actor space
      virtual void SetToWorldPosition( const Vec3 &pos ) { m_Props.m_Transform.SetPosition( pos ); }

	   Vec3 GetWorldPosition( void ) const;

	   Vec3 GetRelForward( void ) const { return m_Props.m_Transform.GetForward(); }

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
	Transform m_Concat;
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
	   CameraNode( const Transform& t, Frustum const &frustum );
      CameraNode( const Vec3& eye, const Vec3& center, const Vec3& up, Frustum const &frustum );

	   virtual int VRender( Scene *pScene ) override;
	   virtual int VOnRestore( Scene *pScene ) override;
	   virtual bool VIsVisible( Scene *pScene ) const { return m_IsActive; }
      virtual void VSetTransform( const Transform& transform );

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