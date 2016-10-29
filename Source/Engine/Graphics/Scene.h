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
#include "SceneNodes.h"
#include "TransformStack.h"
#include "SceneNodes.h"
#include "..\Event\Events.h"
#include "Light.h"

typedef std::map< ActorId, shared_ptr< ISceneNode > > ActorSceneMap;


class Scene
   {
   public:
      Scene( shared_ptr<IRenderer> renderer );
      virtual ~Scene( void );
      int OnRender( void );
      int OnRestore( void );
      int OnLostDevice( void );
      int OnUpdate( unsigned long deltaMs );
      shared_ptr< ISceneNode > FindSceneNode( ActorId id );
      bool AddChild( ActorId id, shared_ptr< ISceneNode > pNode );
      bool RemoveChild( ActorId id );
      void SetCamera(shared_ptr<CameraNode> camera) { m_pCamera = camera; }
      // This function is called by SceneNode::PreRender
      void PushAndSetTransform( TransformPtr pTransform )
	      {
         m_TransformStack.Push( *pTransform );
	      }

      void PushAndSetTransform( Transform transform )
         {
         m_TransformStack.Push( transform );
         }

	   void PopTransform() 
	      {
		   //Scene::PopMatrix - Chapter 16, page 541
		   m_TransformStack.Pop(); 
	      }
      Mat4x4 GetTopMatrix() { return m_TransformStack.GetTopMatrix(); }
      Transform GetTopTransform( void ) const{ return m_TransformStack.GetTopTransForm(); };

	   void AddAlphaSceneNode( AlphaSceneNode *asn ) { m_AlphaSceneNodes.push_back( asn ); }
      void NewRenderComponentDelegate( IEventPtr pEvent );
      void DestroyActorDelegate( IEventPtr pEvent );
      shared_ptr<IRenderer> GetRenderer( void ) { return m_pRenderer; }
      shared_ptr<CameraNode> GetCamera( void ) { return m_pCamera; }
      shared_ptr<LightManager> GetLightManagerPtr( void ){ return m_pLightManager; }
      Transform GetCameraGlobalTransform( void ) const { return m_CameraGlobalTransform; };

   protected:
      void RenderAlphaPass();
      
   protected:
      shared_ptr<SceneNode>         m_Root;
      shared_ptr<CameraNode>        m_pCamera;
      shared_ptr<IRenderer>         m_pRenderer;
      TransformStack                m_TransformStack;
      AlphaSceneNodes               m_AlphaSceneNodes;
      ActorSceneMap                 m_ActorMap;
      shared_ptr<LightManager>      m_pLightManager;
      Transform                     m_CameraGlobalTransform;
   };
