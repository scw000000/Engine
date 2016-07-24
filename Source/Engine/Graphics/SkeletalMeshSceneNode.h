#pragma once
/*!
 * \file SkeletalMeshSceneNode.h
 * \date 2016/07/23 10:56
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
#include "Shaders.h"

class SkeletalMeshSceneNode : public SceneNode
   {
      public:
      SkeletalMeshSceneNode( const ActorId actorId,
                     IRenderComponent* pRenderComponent,
                     shared_ptr<Resource> pSkMeshResouce,
                     MaterialPtr pMaterial,
                     RenderPass renderPass,
                     TransformPtr pTransform );
      ~SkeletalMeshSceneNode( void );
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

      protected:
      void ReleaseResource( void );

      protected:
      GLuint            m_Program;
      GLuint            m_VerTexBuffer;
      GLuint            m_UVBuffer;

      GLuint            m_IndexBuffer;

      GLuint            m_MVPMatrix;
      GLuint            m_Texture;
      GLuint            m_TextureUni;
      GLuint            m_VertexArray;
      shared_ptr<Resource>	 m_pSkeletalMeshResource;
      //  MaterialPtr       m_pMaterial;
      VertexShader		m_VertexShader;
      FragmentShader		m_FragmentShader;

      GLuint            m_NormalBuffer;
      GLuint            m_ToWorldMatrix;
      GLuint            m_LightPosWorldSpace;
      GLuint            m_LigthDirection;
      GLuint            m_LightColor;
      GLuint            m_LightPower;
      GLuint            m_LightAmbient;
      GLuint            m_LightNumber;

      GLuint            m_EyeDirWorldSpace;

      GLuint            m_MaterialAmbient;
      GLuint            m_MaterialDiffuse;

      unsigned long     m_VerticesIndexCount;
      //float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
   };