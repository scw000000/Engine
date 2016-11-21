#pragma once
/*!
 * \file DeferredShader.h
 * \date 2016/11/20 9:01
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
#include "..\Shaders.h"

class IDeferredShader
   {
   public:
      virtual ~IDeferredShader( void ){ }
      virtual void VPreRender( void ) = 0;
      virtual int VOnRestore( void ) = 0;
      virtual int VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode ) = 0;
   };

class OpenGLDeferredShader : public IDeferredShader
   {
   public:
      OpenGLDeferredShader( void );
      virtual void VPreRender( void ) override;
      virtual int VOnRestore( void ) override;
      virtual int VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode ) override;
   
   protected:
      void ReleaseResource( void );
      int OnRestoreGeometryPass( void );
      int OnRestoreLightPass( void );

   private:
      enum RenderPass
         {
         RenderPass_Geometry,
         RenderPass_Light,
         RenderPass_Num
         };
      GLuint m_Programs[ RenderPass_Num ];
      
      VertexShader m_VertexShaders[ RenderPass_Num ];
      FragmentShader m_FragmentShaders[ RenderPass_Num ];

      GLuint m_VAOs[ RenderPass_Num ];

      enum ScreenSpaceTextures
         {
         SST_Depth,
         SST_Normal,
         SST_Texture,
         SST_Ambient,
         SST_Diffuse,
         SST_Specular,
         SST_Num
         };

      GLuint m_SST[ SST_Num ];

      GLuint m_FBO[ RenderPass_Num ];

      enum GeometryPassUniforms
         {
         GeometryPassUni_MVP,
         GeometryPassUni_NormalMat,
         GeometryPassUni_MeshTexture,
         GeometryPassUni_Num
         };

      std::vector< std::vector< GLuint > > m_Uniforms;

      GLuint m_MVPUni;
      GLuint m_MUni;
   };