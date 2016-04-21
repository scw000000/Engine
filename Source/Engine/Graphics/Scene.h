#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#include "SceneNodes.h"
#include "MatrixStack.h"
#include "SceneNodes.h"
#include "..\Event\Events.h"

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
      bool AddChild( ActorId id, shared_ptr< ISceneNode > sceneId );
      bool RemoveChild( ActorId id );
      void SetCamera(shared_ptr<CameraNode> camera) { m_Camera = camera; }
      // This function is called by SceneNode::PreRender
      void PushAndSetMatrix( const Mat4x4 &toWorld )
	      {
         m_MatrixStack.PushRelMatrix( toWorld );
	      }

	   void PopMatrix() 
	      {
		   //Scene::PopMatrix - Chapter 16, page 541
		   m_MatrixStack.Pop(); 
	      }
      const Mat4x4 GetTopMatrix() const { return m_MatrixStack.GetTop(); }

	   void AddAlphaSceneNode( AlphaSceneNode *asn ) { m_AlphaSceneNodes.push_back( asn ); }
      void NewRenderComponentDelegate( IEventDataPtr pEventData );
      shared_ptr<IRenderer> GetRenderer( void ) { return m_pRenderer; }
      shared_ptr<CameraNode> GetCamera( void ) { return m_Camera; }

   protected:
      void RenderAlphaPass();
      
   protected:
      shared_ptr<SceneNode>         m_Root;
      shared_ptr<CameraNode>        m_Camera;
      shared_ptr<IRenderer>         m_pRenderer;
      MatrixStack                   m_MatrixStack;
      AlphaSceneNodes               m_AlphaSceneNodes;
      ActorSceneMap                 m_ActorMap;
      // LightManager *m_pLightManager;
      
   };
