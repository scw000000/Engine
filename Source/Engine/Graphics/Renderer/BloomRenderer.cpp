/*!
 * \file BloomRenderer.cpp
 * \date 2017/06/25 13:20
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
#include "BloomRenderer.h"

#include "RendererLoader.h"
#include "RenderHelper.h"

const char* const BLURPASS_VERTEX_SHADER_FILE_NAME = "Effects\\QuadShader.vs.glsl";
const char* const BLURPASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\BloomBlur.fs.glsl";

const char* const COMBINEPASS_VERTEX_SHADER_FILE_NAME = "Effects\\QuadShader.vs.glsl";
const char* const COMBINEPASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\BloomCombine.fs.glsl";

#define BLOOM_ITERATION_NUM 5

BloomRenderer::BloomRenderer( void )
   {
   m_Shaders[ RenderPasses_Blur ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( BLURPASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPasses_Blur ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( BLURPASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   m_Uniforms[ RenderPasses_Blur ] = std::vector< GLuint >( BlurPass_Uniforms_Num );

   m_Shaders[ RenderPasses_Combine ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( COMBINEPASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPasses_Combine ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( COMBINEPASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   m_Uniforms[ RenderPasses_Combine ] = std::vector< GLuint >( CombinePasss_Uniforms_Num );
   }

void BloomRenderer::VShutdown( void )
   {

   }

int BloomRenderer::VOnRestore( Scene* pScene )
   {
   // Blur pass restore
   for( auto shader : m_Shaders[ RenderPasses_Blur ] )
      {
      shader->VOnRestore();
      }
   m_Programs[ RenderPasses_Blur ] = OpenGLRendererLoader::GenerateProgram( m_Shaders[ RenderPasses_Blur ] );
   for( auto shader : m_Shaders[ RenderPasses_Blur ] )
      {
      shader->VReleaseShader( m_Programs[ RenderPasses_Blur ] );
      }

   glUseProgram( m_Programs[ RenderPasses_Blur ] );

   m_Uniforms[ RenderPasses_Blur ][ BlurPass_Uniforms_IsHorizontal ] = glGetUniformLocation( m_Programs[ RenderPasses_Blur ], "uIsHorizontal" );
   // glUniform1i( m_Uniforms[ BlurPass_Uniforms_IsHorizontal ], 0 );

   m_Uniforms[ RenderPasses_Blur ][ BlurPass_Uniforms_SampleTex ] = glGetUniformLocation( m_Programs[ RenderPasses_Blur ], "uSampleTex" );
   glUniform1i( m_Uniforms[ RenderPasses_Blur ][ BlurPass_Uniforms_SampleTex ], 0 );

   glUseProgram( 0 );

   // FBO
   glGenFramebuffers( FBOs_Num, m_FBOs );
   // ENG_ASSERT( m_FBO );
   // Ping pong FBO0
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Blur_Horizontal ] );

   // Color output
   // glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold_temp ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_LightThreshold_temp ], 0 );

   // glBindTexture( GL_TEXTURE_2D, 0 );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );

   GLuint outputAttatchments[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers( 1, outputAttatchments );

   // Ping pong FBO1
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Blur_Vertical ] );

   // Color output
   // glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_LightThreshold ], 0 );

   // glBindTexture( GL_TEXTURE_2D, 0 );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );

   glDrawBuffers( 1, outputAttatchments );

   // Combine pass restore
   for( auto shader : m_Shaders[ RenderPasses_Combine ] )
      {
      shader->VOnRestore();
      }
   m_Programs[ RenderPasses_Combine ] = OpenGLRendererLoader::GenerateProgram( m_Shaders[ RenderPasses_Combine ] );
   for( auto shader : m_Shaders[ RenderPasses_Combine ] )
      {
      shader->VReleaseShader( m_Programs[ RenderPasses_Combine ] );
      }

   glUseProgram( m_Programs[ RenderPasses_Combine ] );

   m_Uniforms[ RenderPasses_Combine ][ CombinePasss_Uniforms_LightingTex ] = glGetUniformLocation( m_Programs[ RenderPasses_Combine ], "uLightingTex" );
   glUniform1i( m_Uniforms[ RenderPasses_Combine ][ CombinePasss_Uniforms_LightingTex ], 0 );

   m_Uniforms[ RenderPasses_Combine ][ CombinePasss_Uniforms_BlurTex ] = glGetUniformLocation( m_Programs[ RenderPasses_Combine ], "uBlurTex" );
   glUniform1i( m_Uniforms[ RenderPasses_Combine ][ CombinePasss_Uniforms_BlurTex ], 1 );

   // Combine FBO
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Combine ] );

   // Color output
   // glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Bloom ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_Bloom ], 0 );

   // glBindTexture( GL_TEXTURE_2D, 0 );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );

   glDrawBuffers( 1, outputAttatchments );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   OpenglRenderHelper::CheckError();
   return S_OK;
   };

int BloomRenderer::VPreRender( void )
   {
   return S_OK;
   }

int BloomRenderer::VPostRender( void )
   {
   return S_OK;
   };

void BloomRenderer::OnRestoreTextures( GLuint lightingTex, GLuint lightingThresTex, GLuint bloomTex )
   {
   m_UsedTextures[ UsdTex_Lighting ] = lightingTex;
   m_UsedTextures[ UsdTex_LightThreshold ] = lightingThresTex;
   m_UsedTextures[ UsdTex_Bloom ] = bloomTex;

   int textureWidth = 0;
   int textureHeight = 0;
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold ] );
   glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth );
   glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight );

   // Create all textures at once
   glGenTextures( 1, &m_UsedTextures[ UsdTex_LightThreshold_temp ] );
   ENG_ASSERT( m_UsedTextures[ UsdTex_LightThreshold_temp ] );

   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold_temp ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   OpenglRenderHelper::CheckError();
   }

void BloomRenderer::OnRender( void )
   {
   /*auto screenSize = g_pApp->GetScreenSize();
   glViewport( 0.f, 0.f, screenSize.x / 2.f, screenSize.y / 2.f );*/

   glUseProgram( m_Programs[ RenderPasses_Blur ] );

   for( int i = 0; i < BLOOM_ITERATION_NUM; ++i )
     {
     glActiveTexture( GL_TEXTURE0 );
     if( i & 1 ) // odd
        {
        glUniform1ui( m_Uniforms[ RenderPasses_Blur ][ BlurPass_Uniforms_IsHorizontal ], 0 );

        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Blur_Vertical ] );
        glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold_temp ] );
        }
     else
        {
        glUniform1ui( m_Uniforms[ RenderPasses_Blur ][ BlurPass_Uniforms_IsHorizontal ], 1 );

        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Blur_Horizontal] );
        glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold ] ); // Sample texture
        }

     OpenglRenderHelper::RenderQuad();
     }

   glUseProgram( m_Programs[ RenderPasses_Combine ] );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ FBOs_Combine ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Lighting ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ ( BLOOM_ITERATION_NUM & 1 ) ? UsdTex_LightThreshold_temp : UsdTex_LightThreshold ] );

   OpenglRenderHelper::RenderQuad();

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   glUseProgram( 0 );
   }