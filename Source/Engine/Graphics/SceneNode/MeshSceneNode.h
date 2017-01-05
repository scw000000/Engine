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
#include "..\Shaders.h"

class DeferredMainRenderer;

class MeshSceneNode : public SceneNode
   {
   public:
	   MeshSceneNode( const ActorId actorId, 
                     IRenderComponent* pRenderComponent,
                     shared_ptr<Resource> pMeshResouce, 
                     MaterialPtr pMaterial, 
                     RenderGroup renderGroup,
                     TransformPtr pTransform );
      ~MeshSceneNode( void );
	   virtual int VOnRestore( Scene *pScene ) override;
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
      GLuint GetProgram( void ) { return m_Program; };
      virtual ShadowVertexInfo VGetShadowVertexInfo( void ) const override;

   protected:
      void ReleaseResource( void );

   protected:
      shared_ptr< Material > m_pMaterial;
      shared_ptr<Resource>	 m_pMeshResource;
      
      GLuint            m_Program;
      VertexShader		m_VertexShader;
      FragmentShader		m_FragmentShader;

      GLuint            m_VAO;

      enum VB_TYPES
         {
         Vertex_Buffer,
         UV_Buffer, 
         Normal_Buffer,
         Index_Buffer, 
         Num_Buffers
         };

      GLuint            m_Buffers[ Num_Buffers ];

      GLuint            m_MVPUni;
      GLuint            m_MeshTextureObj;
      GLuint            m_MeshTextureUni;
      
      GLuint            m_MUni;
      GLuint            m_LightPosWorldSpaceUni;
      GLuint            m_LigthDirectionUni;
      GLuint            m_LightColorUni;
    //  GLuint            m_LightPowerUni;
      GLuint            m_LightAmbientUni;
      GLuint            m_LightNumberUni;

      GLuint            m_ShadowMapMatrixUni;
      GLuint            m_ShadowMapTextureUni[ MAXIMUM_SHADOWMAP_TEXTURE_SUPPORTED ];

      GLuint            m_EyePosWorldSpaceUni;

      GLuint            m_MaterialAmbientUni;
      GLuint            m_MaterialDiffuseUni;
      GLuint            m_MaterialSpecularUni;

      unsigned long     m_VerticesIndexCount;

      DeferredMainRenderer *m_pDeferredMainRenderer;
   };