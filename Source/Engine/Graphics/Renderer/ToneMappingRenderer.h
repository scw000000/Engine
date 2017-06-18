#pragma once
/*!
 * \file ToneMappingRenderer.h
 * \date 2017/06/18 11:02
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

class ToneMappingRenderer : public IRenderer
   {
   public:
      ToneMappingRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      void OnRestoreTextures( GLuint lightingTex );
      void OnRender( void );
   private:
      enum UsedTextures
         {
         UsdTex_Lighting,
         UsdTex_Num
         };
      GLuint m_UsedTextures[ UsdTex_Num ];
      enum Uniforms
         {
         Uniforms_LightingTex,
         Uniforms_Num
         };
      GLuint m_Uniforms[ Uniforms_Num ];
      std::vector< shared_ptr< OpenGLShader > > m_Shader;
      GLuint m_Program;
      GLuint m_FBO;

   };