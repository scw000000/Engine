#pragma once
/*!
 * \file Scene.h
 * \date 2016/05/22 10:20
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
#include ".\SceneNode\SceneNodes.h"
#include "TransformStack.h"
#include "..\Event\Events.h"
#include "Light.h"

typedef std::map< ActorId, shared_ptr< ISceneNode > > ActorSceneMap;


class Scene
   {
   friend class LightManager;
   public:
      Scene( shared_ptr< IRenderManager > pRenderManager );
      virtual ~Scene( void );
      int PreRender( void );
      int OnRender( void );
      int OnRestore( void );
      int OnLostDevice( void );
      // For setting up global transform
      int PreUpdate( void );
      int OnUpdate( float deltaSeconds );
      shared_ptr< ISceneNode > FindSceneNode( ActorId id );
      bool AddChild( ActorId id, shared_ptr< ISceneNode > pNode );
      bool RemoveChild( ActorId id );
      void SetCamera(shared_ptr<CameraNode> camera) { m_pCamera = camera; }
      // This function is called by SceneNode::PreRender
    //  void PushAndSetTransform( TransformPtr pTransform )
	   //   {
    //     m_TransformStack.Push( *pTransform );
	   //   }

    //  void PushAndSetTransform( Transform transform )
    //     {
    //     m_TransformStack.Push( transform );
    //     }

	   //void PopTransform() 
	   //   {
		  // //Scene::PopMatrix - Chapter 16, page 541
		  // m_TransformStack.Pop(); 
	   //   }
   //   Mat4x4 GetTopMatrix() { return m_TransformStack.GetTopMatrix(); }
   //   Transform GetTopTransform( void ) const{ return m_TransformStack.GetTopTransForm(); };

	   void AddAlphaSceneNode( AlphaSceneNode *asn ) { m_AlphaSceneNodes.push_back( asn ); }
      void NewRenderComponentDelegate( IEventPtr pEvent );
      void DestroyActorDelegate( IEventPtr pEvent );
     // shared_ptr<IRenderer> GetRenderer( void ) { return m_pRenderer; }
      shared_ptr<CameraNode> GetCamera( void ) { return m_pCamera; }
      shared_ptr<LightManager> GetLightManagerPtr( void ){ return m_pLightManager; }

   protected:
      void RenderAlphaPass();
      
   protected:
      shared_ptr< SceneNode >         m_pRoot;
      shared_ptr< CameraNode >        m_pCamera;
    //  shared_ptr< IRenderer >         m_pRenderer;
      shared_ptr< IRenderManager >    m_pRenderManager;
     // TransformStack                m_TransformStack;
      AlphaSceneNodes               m_AlphaSceneNodes;
      ActorSceneMap                 m_ActorMap;
      shared_ptr<LightManager>      m_pLightManager;
   };
