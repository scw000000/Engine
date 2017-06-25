#pragma once
/*!
 * \file MotionBlurRenderer.h
 * \date 2017/06/24 17:51
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

class MotionBlurRenderer : public IRenderer
   {
      public:
      MotionBlurRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      void OnRestoreTextures( GLuint mvTex, GLuint lightingTex, GLuint motionBlurTex );
      void OnRender( void );
      private:
      enum UsedTextures
         {
         UsdTex_Lighting,
         UsdTex_MotionVector,
         UsdTex_MotionBlur,
         UsdTex_Num
         };
      GLuint m_UsedTextures[ UsdTex_Num ];
      enum Uniforms
         {
         Uniforms_MotionVectorTex,
         Uniforms_LightingTex,
         Uniforms_BlurScale,
         Uniforms_Num
         };
      GLuint m_Uniforms[ Uniforms_Num ];
      std::vector< shared_ptr< OpenGLShader > > m_Shader;
      GLuint m_Program;
      GLuint m_FBO;

   };