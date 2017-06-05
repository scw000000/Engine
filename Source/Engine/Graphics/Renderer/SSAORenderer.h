#pragma once
/*!
 * \file SSAORenderer.h
 * \date 2017/06/04 10:48
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#define MAX_SSAO_SAMPLE_POINTS 300
#include "..\Shaders.h"

class Scene;

class SSAORenderer : public IRenderer
   {
   public:
      SSAORenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      void OnRestoreTextures( GLuint deptpTex, GLuint mrt0Tex, GLuint ssaoTex, GLuint ssaoBlurTex );
      void OnRender( void );
   private:
      
      enum RenderPass
         {
         RenderPass_Sample,
         RenderPass_Blur,
         RenderPass_Num
         };
      enum SamplePassUniforms
         {
         SamplePassUni_SampleCount,
         SamplePassUni_SampleVectors,
         SamplePassUni_DpethTexture,
         SamplePassUni_MRT0,
         SamplePassUni_NoiseTexure,
         SamplePassUni_ProjectMat,
         SamplePassUni_HalfSizeNearPlane,
         SamplePassUni_NoiseTextureSampleScale,
         SamplePassUni_Num
         };
      enum BlurPassUniforms
         {
         BlurPassUni_SSAOTex,
         BlurPassUni_ScreenSize,
         BlurPassUni_Num
         };
      enum UsedTextures
         {
         UsdTex_Depth,
         UsdTex_Mrt0,
         UsdTex_SSAO,
         UsdTex_SSAOBlur,
         UsdTex_Num
         };
      GLuint m_UsedTextures[ UsdTex_Num ];

      GLuint m_NoiseTex;
      GLuint m_SamplePoints[ MAX_SSAO_SAMPLE_POINTS ];
      std::vector< GLuint > m_Uniforms[ RenderPass_Num ];
      std::vector< shared_ptr< OpenGLShader > > m_Shaders[ RenderPass_Num ];
      GLuint m_Programs[ RenderPass_Num ];
      GLuint m_FBOs[ RenderPass_Num ];

   };