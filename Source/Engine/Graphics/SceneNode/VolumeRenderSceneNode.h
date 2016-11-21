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
#include "..\Shaders.h"


class VolumeRenderSceneNode : public SceneNode
   {
   public:
      VolumeRenderSceneNode( const ActorId actorId,
                     IRenderComponent* pRenderComponent,
                     RenderGroup renderGroup,
                     TransformPtr pTransform,
                     shared_ptr<Resource> pVolumeTextureResource,
                     shared_ptr<Resource> pTransferFunctionResource,
                     const Vec3& textureDiemension,
                     const Vec3& cuboidDimension );
      ~VolumeRenderSceneNode( void );
      virtual int VOnRestore( Scene *pScene ) override;
      virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
      virtual int VRender( Scene *pScene )  override;

   protected:
      void ReleaseResource( void );
      void SetUpRenderedTexture( void );
      void SetUpFrameBuffer( void );
      void SetUpNormalVolume( std::vector< Vec3 >& output, const char* pRawTexture );

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
      GLuint            m_VAO;

      GLuint            m_FirstPassProgram;
      
      VertexShader		m_FirstPassVertexShader;
      FragmentShader		m_FirstPassFragmentShader;
      
      GLuint            m_FirstPassMVPUni;

      GLuint            m_VBOs[ Num_Buffers ];

      GLuint            m_RenderedTextureObj;
      GLuint            m_RenderDepthBufferObj;
      GLuint            m_FrameBufferObj;

      GLuint            m_SecondPassProgram;

      VertexShader		m_SecondPassVertexShader;
      FragmentShader		m_SecondPassFragmentShader;

      GLuint            m_VolumeTextureObj;
      GLuint            m_TranserTextureObj;

      GLuint            m_ScreenSizeUni;
      GLuint            m_StepSizeUni;
      GLuint            m_TransferTextureUni;
      GLuint            m_RenderedTextureUni;
      GLuint            m_VolumeTextureUni;
      GLuint            m_SecondPassMVPUni;

      Vec3              m_VerticesLocation[ 8 ];
      Vec3              m_TextureDimension;
      const static Vec3              s_BackFacePosition[];
      const static unsigned int              s_VerticesIndex[];
   };