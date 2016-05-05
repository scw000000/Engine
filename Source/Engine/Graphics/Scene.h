#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#include "SceneNodes.h"
#include "TransformStack.h"
#include "SceneNodes.h"
#include "..\Event\Events.h"
#include "Light.h"

typedef std::map< ActorId, shared_ptr< ISceneNode > > ActorSceneMap;

// TODO: complete implementation
class Scene
   {
   public:
      Scene( shared_ptr<IRenderer> renderer );
      virtual ~Scene( void );
      int OnRender( void );
      int OnRestore( void );
      int OnLostDevice( void );
      int OnUpdate( const unsigned long deltaMs );
      shared_ptr< ISceneNode > FindSceneNode( ActorId id );
      bool AddChild( ActorId id, shared_ptr< ISceneNode > pNode );
      bool RemoveChild( ActorId id );
      void SetCamera(shared_ptr<CameraNode> camera) { m_Camera = camera; }
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
      void NewRenderComponentDelegate( IEventDataPtr pEventData );
      shared_ptr<IRenderer> GetRenderer( void ) { return m_pRenderer; }
      shared_ptr<CameraNode> GetCamera( void ) { return m_Camera; }
      shared_ptr<LightManager> GetLightManagerPtr( void ){ return m_pLightManager; }

   protected:
      void RenderAlphaPass();
      
   protected:
      shared_ptr<SceneNode>         m_Root;
      shared_ptr<CameraNode>        m_Camera;
      shared_ptr<IRenderer>         m_pRenderer;
      TransformStack                m_TransformStack;
      AlphaSceneNodes               m_AlphaSceneNodes;
      ActorSceneMap                 m_ActorMap;
      shared_ptr<LightManager>      m_pLightManager;
      
   };
