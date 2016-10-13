#pragma once
/*!
 * \file VolumeRenderSceneNode.h
 * \date 2016/10/13 12:22
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "SceneNodes.h"
#include "Shaders.h"


class VolumeRenderSceneNode : public SceneNode
   {
   public:
      VolumeRenderSceneNode( const ActorId actorId,
                     IRenderComponent* pRenderComponent,
                     shared_ptr<Resource> pVolumeTextureResource,
                     const Vec3& dimension,
                     shared_ptr<Resource> pTransferFunctionResource,
                     RenderPass renderPass,
                     TransformPtr pTransform );
      ~VolumeRenderSceneNode( void );
      virtual int VOnRestore( Scene *pScene ) override;
      virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
      virtual int VRender( Scene *pScene )  override;

   protected:
      void ReleaseResource( void );

   protected:
      shared_ptr<Resource> m_pVolumeTextureResource;
      shared_ptr<Resource> m_pTransferFuncionResource;

      enum VB_TYPES
         {
         Vertex_Position,
         BackFace_Position,
         Vertex_Index,
         Num_Buffers
         };

      GLuint            m_FirstPassProgram;
      GLuint            m_FirstPassVAO;
      VertexShader		m_FirstPassVertexShader;
      FragmentShader		m_FirstPassFragmentShader;
      
      GLuint            m_VBOs[ Num_Buffers ];

      GLuint            m_SecondPassProgram;
      GLuint            m_SecondPassVAO;
      VertexShader		m_SecondPassVertexShader;
      FragmentShader		m_SecondPassFragmentShader;

      GLuint            m_MVPMatrix;
      GLuint            m_Texture;
      GLuint            m_TextureUni;

      Vec3              m_VerticesLocation[ 8 ];
      const static Vec3              s_BackFacePosition[];
      const static unsigned int              s_VerticesIndex[];
   };