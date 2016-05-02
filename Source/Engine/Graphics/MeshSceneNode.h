#pragma once
/*!
 * \file MeshSceneNode.h
 * \date 2016/05/02 11:09
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
#include "Shaders.h"

// TODO: prevent redundent resource file in MeshComponent
class MeshSceneNode : public SceneNode
   {
   public:
	   MeshSceneNode( const ActorId actorId, 
                     WeakBaseRenderComponentPtr renderComponent, 
                     const Resource& meshResouce, 
                     const MaterialPtr& pMaterialPtr, 
                     RenderPass renderPass,  
                     TransformPtr pTransform );
      ~MeshSceneNode( void );
	   virtual int VOnRestore( Scene *pScene ) override; ;
	   virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
      // This function is called by Scene
      // bref: load constant buffers for material, light, etc in shaders
      // load vertex buffer & index buffer and make draw call
	   /**
	    * @brief 
	    *
	    * @param  pScene Scene * pScene
	    * @return int
	    */
	    virtual int VRender( Scene *pScene )  override;
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);
      GLuint GetProgram( void ) { return m_Program; };

   protected:
      

   protected:
	   GLuint            m_Program;
      GLuint            m_VerTexBuffer;
      GLuint            m_UVBuffer;
      GLuint            m_MVPMatrix;
      GLuint            m_Texture;
      GLuint            m_TextureUni;
      GLuint            m_VertexArray;
      Resource			   m_MeshResource;
      MaterialPtr       m_pMaterial;
	   VertexShader		m_VertexShader;
	   FragmentShader		m_FragmentShader;
      Material          m_Material;

      GLuint            m_NormalBuffer;
      GLuint            m_ToWorldMatrix;
      GLuint            m_LightPosWorldSpace;
      GLuint            m_LigthDirection;
      GLuint            m_LightDiffuse;
      GLuint            m_LightPower;
      GLuint            m_LightAmbient;
      GLuint            m_LightNumber;

      GLuint            m_MaterialAmbient;
      GLuint            m_MaterialDiffuse;
	//float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
   };