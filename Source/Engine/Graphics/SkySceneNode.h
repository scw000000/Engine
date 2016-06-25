#pragma once
/*!
 * \file SkySceneNode.h
 * \date 2016/05/03 15:07
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


class SkySceneNode : public SceneNode
   {
   public:
	   SkySceneNode( const ActorId actorId, 
                    IRenderComponent* pRenderComponent,
                    shared_ptr<Resource> pMeshResource,
                    shared_ptr<Resource> pTextureResource, 
                    RenderPass renderPass,  
                    TransformPtr pTransform );
      ~SkySceneNode( void );
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

      virtual bool VIsVisible( Scene *pScene ) override { return m_IsActive; };

   protected:
      void ReleaseResource( void );

   protected:
      bool					   m_IsActive;

	   GLuint               m_Program;
      GLuint               m_VertexBuffer;
      GLuint               m_UVBuffer;
      GLuint               m_IndexBuffer;
      GLuint               m_MVPMatrix;
      GLuint               m_Texture;
      GLuint               m_TextureUni;
      GLuint               m_VertexArray;
      shared_ptr<Resource> m_pTextureResource;
      shared_ptr<Resource> m_pMeshResource;
	   VertexShader		   m_VertexShader;
	   FragmentShader		   m_FragmentShader;

      unsigned long     m_VerticesCount;

	//float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
   };
