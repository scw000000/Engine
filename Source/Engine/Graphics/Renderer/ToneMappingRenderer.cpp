/*!
 * \file ToneMappingRenderer.cpp
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

#include "EngineStd.h"
#include "ToneMappingRenderer.h"
#include "RendererLoader.h"
#include "RenderHelper.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\ToneMapping.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\ToneMapping.fs.glsl";

ToneMappingRenderer::ToneMappingRenderer( void )
   {
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   }

void ToneMappingRenderer::VShutdown( void )
   {

   }

int ToneMappingRenderer::VOnRestore( Scene* pScene )
   {
   for( auto shader : m_Shader )
      {
      shader->VOnRestore();
      }
   m_Program = OpenGLRendererLoader::GenerateProgram( m_Shader );
   for( auto shader : m_Shader )
      {
      shader->VReleaseShader( m_Program );
      }
     
   glUseProgram( m_Program );

   m_Uniforms[ Uniforms_LightingTex ] = glGetUniformLocation( m_Program, "uLightingTex" );
   glUniform1i( m_Uniforms[ Uniforms_LightingTex ], 0 );

   glUseProgram( 0 );

   OpenglRenderHelper::CheckError();
   return S_OK;
   };

int ToneMappingRenderer::VPreRender( void )
   {
   return S_OK;
   }

int ToneMappingRenderer::VPostRender( void )
   {
   return S_OK;
   };

void ToneMappingRenderer::OnRestoreTextures( GLuint lightingTex )
   {
   m_UsedTextures[ UsdTex_Lighting ] = lightingTex;
   }

void ToneMappingRenderer::OnRender( void )
   {
   glUseProgram( m_Program );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Lighting ] );

   OpenglRenderHelper::RenderQuad();

   glUseProgram( 0 );
   }