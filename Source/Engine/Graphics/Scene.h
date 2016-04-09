#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#include "SceneNodes.h"
#include "MatrixStack.h"
#include "SceneNodes.h"

typedef std::map< ActorId, shared_ptr< ISceneNode > > ActorSceneMap;

// TODO: complete implementation
class Scene
   {
   public:
      Scene( void );
      virtual ~Scene( void );
      int OnRender( void );
      int OnRestore( void );
      int OnLostDevice( void );
      int OnUpdate( const unsigned long deltaMs );
      shared_ptr< ISceneNode > FindSceneNode( ActorId id );
      bool AddChild( ActorId id, shared_ptr< ISceneNode > sceneId );
      bool RemoveChild( ActorId id );
      void PushAndSetMatrix(const Mat4x4 &toWorld)
	      {
         m_MatrixStack.PushRelMatrix( toWorld );
		   Mat4x4 mat = m_MatrixStack.GetTop();
	   //	m_Renderer->VSetWorldTransform(&mat);
	      }

	   void PopMatrix() 
	      {
		   //Scene::PopMatrix - Chapter 16, page 541
		   m_MatrixStack.Pop(); 
		   Mat4x4 mat = m_MatrixStack.GetTop();
	      //m_Renderer->VSetWorldTransform(&mat);
	      }

	   void AddAlphaSceneNode( AlphaSceneNode *asn ) { m_AlphaSceneNodes.push_back( asn ); }

   protected:
      void RenderAlphaPass();
      
   protected:
      shared_ptr<SceneNode>         m_Root;
     // shared_ptr<CameraNode>      m_Camera;
      //shared_ptr<IRenderer> m_Renderer;
      MatrixStack                   m_MatrixStack;
      AlphaSceneNodes               m_AlphaSceneNodes;
      ActorSceneMap                 m_ActorMap;
      // LightManager *m_pLightManager;
      
   };
