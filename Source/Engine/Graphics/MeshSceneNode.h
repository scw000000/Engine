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
      ~MeshSceneNode( void );
	   virtual int VOnRestore( Scene *pScene ) override; ;
	   virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
      // This function is called by Scene
      // bref: load constant buffers for material, light, etc in shaders
      // load vertex buffer & index buffer and make draw call
	   virtual int VRender( Scene *pScene )  override;
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);
      GLuint GetProgram( void ) { return m_Program; };

   protected:
	   GLuint            m_Program;
      GLuint            m_VerTexBuffer;
      GLuint            m_UVBuffer;
      GLuint            m_MVPMatrix;
      GLuint            m_Texture;
      GLuint            m_VertexArray;
      Resource			   m_MeshResource;
	   VertexShader		m_VertexShader;
	   FragmentShader		m_FragmentShader;

	//float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
   };