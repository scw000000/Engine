#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.h
////////////////////////////////////////////////////////////////////////////////

#include "SceneNodes.h"
#include "Shaders.h"

class MeshSceneNode : public SceneNode
   {
   public:
	   MeshSceneNode( const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Resource meshResouce, RenderPass renderPass,  const Mat4x4 *transform );

	   virtual int VOnRestore( Scene *pScene ) override; ;
	   virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
	   virtual int VRender( Scene *pScene )  override;
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);

   protected:
	   Resource					m_MeshResource;

	   VertexShader		m_VertexShader;
	   FragmentShader		m_FragmentShader;

	//float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
   };