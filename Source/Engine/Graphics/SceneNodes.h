#pragma once
/*!
 * \file SceneNodes.h
 * \date 2016/05/01 10:08
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  TODO: make data of actor component shared with scene node instead of copying it
 *
 * \note
 */


#include "Material.h"


class RenderComponent;


//   This enum defines the different types of alpha blending
//   types that can be set on a scene node.
enum AlphaType {
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
      Mat4x4 GetFromWorld( void ) const { return m_pTransform->GetFromWorld(); }
      TransformPtr GetTransformPtr( void ) const;

      /**
       * @brief If you don't want to the change value of transform, call it instead of GetTransformPtr 
       *
       * @param   void
       * @return const Transform&
       */
      const Transform& GetTransform( void ) const;
      Transform GetTransform( void );

      const char* Name( void ) const { return m_Name.c_str(); }

      void SetAlpha( const float alpha );
      float GetAlpha( void ) const;

      RenderPass GetRenderPass( void ) const { return m_RenderPass; }
      float GetRadius( void ) const { return m_Radius; }
      
      MaterialPtr GetMaterialPtr( void ) { return m_pMaterial; }
      void SetMaterialPtr( MaterialPtr pMaterial ) { pMaterial = m_pMaterial; }

   protected:
      ActorId m_ActorId;
      std::string m_Name;
      TransformPtr m_pTransform;
      float m_Radius;
      RenderPass m_RenderPass;
      MaterialPtr m_pMaterial;

   private:

   };

typedef std::vector< std::shared_ptr< ISceneNode > > SceneNodeList;

class SceneNode : public ISceneNode
   {
	friend class Scene;
   
   public:
      // TODO: finish constructor
   SceneNode( ActorId actorId, IRenderComponent* pRenderComponent, RenderPass renderPass, TransformPtr pNewTransform = TransformPtr( ENG_NEW Transform ), MaterialPtr pMaterial = MaterialPtr() );

	   virtual ~SceneNode();

      virtual const SceneNodeProperties& VGetProperties() const override { return m_Props; }

	   /**
	    * @brief non constant version of getter function
	    *
	    * @return SceneNodeProperties&
	    */
      virtual SceneNodeProperties& VGetProperties() override { 
        return const_cast< SceneNodeProperties& >(static_cast< const SceneNode* >(this)->VGetProperties() );
         }

	   virtual void VSetTransformPtr( TransformPtr pNewTransform ) override;
      virtual void VSetTransform( const Transform& newTransform ) override;

	   virtual int VOnRestore( Scene *pScene ) override;
      virtual int VOnUpdate( Scene *pScene, unsigned long elapsedMs ) override;

	   virtual int VPreRender( Scene *pScene ) override;
	   virtual bool VIsVisible( Scene *pScene ) override;
	   virtual int VRender( Scene *pScene ) { return S_OK; } 
	   virtual int VRenderChildren( Scene *pScene ) override;
	   virtual int VPostRender( Scene *pScene ) override;

	   virtual bool VAddChild( shared_ptr<ISceneNode> child ) override;
	   virtual bool VRemoveChild( ActorId id ) override;
	   virtual int VOnLostDevice( Scene *pScene ) override;
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);

      void SetAlpha( float alpha );
	   float GetAlpha() const { return m_Props.GetAlpha(); }
      
      // return position in parent space
	   virtual Vec3 GetToWorldPosition( void ) const override { return m_Props.m_pTransform->GetToWorldPosition(); }
	   // set position in actor space
      virtual void SetToWorldPosition( const Vec3 &pos )  override { m_Props.m_pTransform->SetPosition( pos ); }

      Vec3 GetForward( void ) const { return m_Props.m_pTransform->GetForward(); }

      virtual Vec3 VGetWorldPosition( void ) const override final;

	   void SetRadius( float radius ) { m_Props.m_Radius = radius; }

      virtual void VSetParentNode( ISceneNode* pParent ) override { m_pParent = pParent; }
      virtual ISceneNode* VGetParentNode( void ) override { return m_pParent; };

	   /*void SetMaterial( const Material& mat ) { m_Props.m_Material = mat; }*/

   protected:
	   SceneNodeList			m_Children;
      ISceneNode				*m_pParent;
	   SceneNodeProperties		m_Props;
      IRenderComponent*	m_pRenderComponent;
   };



struct AlphaSceneNode
   {
	shared_ptr<ISceneNode> m_pNode;
	Transform m_Concat;
	float m_ScreenZ;

	// For the STL sort...
   // To render alpha object, they must be rendered from back to front
   // to get desired effect
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
	   CameraNode( TransformPtr pTransform, Frustum const &frustum );
      CameraNode( const Vec3& eye, const Vec3& center, const Vec3& up, Frustum const &frustum );

	   virtual int VRender( Scene *pScene ) override;
	   virtual int VOnRestore( Scene *pScene ) override;
	   virtual bool VIsVisible( Scene *pScene ) const { return m_IsActive; }
      virtual void VSetTransform( const Transform& newTransform ) override ;

	   const Frustum &GetFrustum( void ) { return m_Frustum; }
	   void SetTarget(shared_ptr<SceneNode> pTarget) { m_pTarget = pTarget; }
	   void ClearTarget( void ) { m_pTarget = shared_ptr<SceneNode>(); }
	   
      shared_ptr<SceneNode> GetTarget( void ) { return m_pTarget; }

	   Mat4x4 GetWorldViewProjection(Scene *pScene);
	   int SetViewTransform(Scene *pScene);

	   Mat4x4 GetProjection() { return m_Projection; }
	   Mat4x4 GetView( void ) { return m_View; }

	   void SetCameraOffset( const Vec4 & cameraOffset ) { m_CamOffsetVector = cameraOffset; }
      /**
       * @brief return projection point on screen position in world space
       *
       * @param  distance float distance
       * @param  screenPosition const Point & screenPosition
       * @return Vec3
       */
       void GetScreenProjectPoint( Vec3& start, Vec3& end, const Point& screenPosition, float distance );

   protected:
	   Frustum			m_Frustum;
      Mat4x4			m_Projection;
	   Mat4x4			m_View;
	   bool			m_IsActive;
	   bool			m_IsDebugCamera;
	   shared_ptr<SceneNode> m_pTarget;
	   Vec4			m_CamOffsetVector;	//Direction of camera relative to target.
   };