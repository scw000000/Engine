#pragma once
/*!
 * \file BloomRenderer.h
 * \date 2017/06/25 13:19
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


#include "..\Shaders.h"

class BloomRenderer : public IRenderer
   {
      public:
      BloomRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      void OnRestoreTextures( GLuint lightingTex, GLuint lightingThresTex, GLuint bloomTex );
      void OnRender( void );
      private:
      enum FBOs
         {
         FBOs_Blur_Horizontal,
         FBOs_Blur_Vertical,
         FBOs_Combine,
         FBOs_Num
         };
      enum RenderPasses
         {
         RenderPasses_Blur,
         RenderPasses_Combine,
         RenderPasses_Num
         };
      enum UsedTextures
         {
         UsdTex_Lighting, // for final combination
         UsdTex_LightThreshold, // Gaussian ping-pong pass 0
         UsdTex_LightThreshold_temp, // Gaussian ping-pong pass 1
         UsdTex_Bloom, // the output 
         UsdTex_Num
         };
      GLuint m_UsedTextures[ UsdTex_Num ];
      enum BlurPass_Uniforms
         {
         BlurPass_Uniforms_IsHorizontal,
         BlurPass_Uniforms_SampleTex,
         BlurPass_Uniforms_Num
         };
      enum CombinePasss_Uniforms
         {
         CombinePasss_Uniforms_LightingTex,
         CombinePasss_Uniforms_BlurTex,
         CombinePasss_Uniforms_Num
         };
      std::vector< GLuint > m_Uniforms[ RenderPasses_Num ];
      std::vector< shared_ptr< OpenGLShader > > m_Shaders[ RenderPasses_Num ];
      GLuint m_Programs[ RenderPasses_Num ];
      GLuint m_FBOs[ FBOs_Num ];

   };