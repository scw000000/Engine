/*!
 * \file MotionBlurRenderer.cpp
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

#include "EngineStd.h"
#include "MotionBlurRenderer.h"
#include "RendererLoader.h"
#include "RenderHelper.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\QuadShader.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\MotionBlur.fs.glsl";

MotionBlurRenderer::MotionBlurRenderer( void )
   {
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   }

void MotionBlurRenderer::VShutdown( void )
   {

   }

int MotionBlurRenderer::VOnRestore( Scene* pScene )
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

   m_Uniforms[ Uniforms_MotionVectorTex ] = glGetUniformLocation( m_Program, "uMotionVectorTex" );
   glUniform1i( m_Uniforms[ Uniforms_MotionVectorTex ], 0 );

   m_Uniforms[ Uniforms_LightingTex ] = glGetUniformLocation( m_Program, "uLightingTex" );
   glUniform1i( m_Uniforms[ Uniforms_LightingTex ], 1 );

   m_Uniforms[ Uniforms_BlurScale ] = glGetUniformLocation( m_Program, "uBlurScale" );
   glUniform1f( m_Uniforms[ Uniforms_BlurScale ], 5.0f );

   glUseProgram( 0 );

   // FBO
   glGenFramebuffers( 1, &m_FBO );
   ENG_ASSERT( m_FBO );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );

   // Color output
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_MotionBlur ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_MotionBlur ], 0 );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );

   GLuint outputAttatchments[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers( 1, outputAttatchments );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   OpenglRenderHelper::CheckError();
   return S_OK;
   };

int MotionBlurRenderer::VPreRender( void )
   {
   return S_OK;
   }

int MotionBlurRenderer::VPostRender( void )
   {
   return S_OK;
   };

void MotionBlurRenderer::OnRestoreTextures( GLuint mvTex, GLuint lightingTex, GLuint motionBlurTex )
   {
   m_UsedTextures[ UsdTex_Lighting ] = lightingTex;
   m_UsedTextures[ UsdTex_MotionVector ] = mvTex;
   m_UsedTextures[ UsdTex_MotionBlur ] = motionBlurTex;
   }

void MotionBlurRenderer::OnRender( void )
   {
   glUseProgram( m_Program );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_MotionVector ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Lighting ] );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );

   OpenglRenderHelper::RenderQuad();

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   glUseProgram( 0 );
   }