/*!
 * \file SSAORenderer.cpp
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

#include "EngineStd.h"
#include "SSAORenderer.h"
#include "RendererLoader.h"
#include "RenderHelper.h"

#define NOISE_TEXTURE_SIZE 4
#define NOISE_TEXTURE_AREA 4 * 4

const char* const SAMPLE_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\SSAOSample.vs.glsl";
const char* const SAMPLE_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\SSAOSample.fs.glsl";

SSAORenderer::SSAORenderer( void )
   {
   m_Uniforms[ RenderPass_Sample ] = std::vector< GLuint >( SamplePassUni_Num, 0 );
   m_Shaders[ RenderPass_Sample ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( SAMPLE_PASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPass_Sample ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( SAMPLE_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );

   m_Shaders[ RenderPass_Blur ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( SAMPLE_PASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPass_Blur ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( SAMPLE_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   }

void SSAORenderer::VShutdown( void )
   {
 
   }

int SSAORenderer::VOnRestore( Scene* pScene )
   {
   for( int i = 0; i < RenderPass_Num; ++i )
      {
      for( auto shader : m_Shaders[ i ] )
         {
         shader->VOnRestore();
         }
      m_Programs[ i ] = OpenGLRendererLoader::GenerateProgram( m_Shaders[ i ] );
      for( auto shader : m_Shaders[ i ] )
         {
         shader->VReleaseShader( m_Programs[ i ] );
         }
      }

   glUseProgram( m_Programs[ RenderPass_Sample ] );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_DpethTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uDepthTex" );
   glUniform1i( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_DpethTexture ], 0 );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_MRT0 ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uMRT0" );
   glUniform1i( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_MRT0 ], 1 );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_NoiseTexure ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uNoiseTex" );
   glUniform1i( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_NoiseTexure ], 2 );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_HalfSizeNearPlane ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uHalfSizeNearPlane" );
   auto pCamera = pScene->GetCamera();
   float halfSizeNearPlaneY = std::tan( pCamera->GetFrustum().m_FovY / 2.0f );
   float halfSizeNearPlaneX = halfSizeNearPlaneY * pCamera->GetFrustum().m_Aspect;
   glUniform2f( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_HalfSizeNearPlane ], halfSizeNearPlaneX, halfSizeNearPlaneY );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_NoiseTextureSampleScale ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uNoiseTexSampleScale" );
   auto screenSize = g_pApp->GetScreenSize();
   Vec2 noiseSampleScalar( screenSize.x / NOISE_TEXTURE_SIZE, screenSize.y / NOISE_TEXTURE_SIZE );
   glUniform2f( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_NoiseTextureSampleScale ], noiseSampleScalar.x, noiseSampleScalar.y );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_ProjectMat ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uProjectMat" );
   auto projMat = pScene->GetCamera()->GetProjection();
   glUniformMatrix4fv( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_ProjectMat ], 1, GL_FALSE, &projMat[ 0 ][ 0 ] );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_SampleCount ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uSampleCount" );
   glUniform1ui( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_SampleCount ], 100u );

   m_Uniforms[ RenderPass_Sample ][ SamplePassUni_SampleVectors ] = glGetUniformLocation( m_Programs[ RenderPass_Sample ], "uSampleVectors" );

   // Generate random sample vector
   Vec3 sampleVectors[ MAX_SSAO_SAMPLE_POINTS ];
   ENGRandom random;
   for( int i = 0; i < NOISE_TEXTURE_AREA; ++i )
      {
      // scale x, y to +-1, z to 0-1
      sampleVectors[ i ].x = ( random.Random() - 0.5f ) * 2.f;
      sampleVectors[ i ].y = ( random.Random() - 0.5f ) * 2.f;
      sampleVectors[ i ].z = random.Random();
      }
   glUniform3fv( m_Uniforms[ RenderPass_Sample ][ SamplePassUni_SampleVectors ], MAX_SSAO_SAMPLE_POINTS, (GLfloat *) &sampleVectors[ 0 ] );
   
   glUseProgram( 0 );
   
   // Generate noise texture
   //std::vector< Vec3 > noizeVec( noiseTexArea );
   Vec3 noiseData[ NOISE_TEXTURE_AREA ];
   
   for( int i = 0; i < NOISE_TEXTURE_AREA; ++i )
      {
      // scale x, y to +-1, z to 0
      noiseData[ i ].x = ( random.Random() - 0.5f ) * 2.f;
      noiseData[ i ].y = ( random.Random() - 0.5f ) * 2.f;
      noiseData[ i ].z = 0.f;
      }
   
   glGenTextures( 1, &m_NoiseTex );
   ENG_ASSERT( m_NoiseTex );
   
   // Depth texture
   glBindTexture( GL_TEXTURE_2D, m_NoiseTex );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, 4, 4, 0, GL_RGB, GL_FLOAT, ( void* ) &noiseData[ 0 ] );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glGenFramebuffers( 1, &m_FBOs[ RenderPass_Sample ] );
   ENG_ASSERT( m_FBOs[ RenderPass_Sample ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ RenderPass_Sample ] );

   // Color output
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_SSAO ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_SSAO ], 0 );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );

   GLuint outputAttatchments[] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers( 1, outputAttatchments );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   OpenglRenderHelper::CheckError();
   return S_OK; 
   };

int SSAORenderer::VPreRender( void )
   {
   return S_OK;
   }

int SSAORenderer::VPostRender( void )
   {
   return S_OK;
   };

void SSAORenderer::OnRestoreTextures( GLuint deptpTex, GLuint mrt0Tex, GLuint ssaoTex, GLuint ssaoBlurTex )
   {
   m_UsedTextures[ UsdTex_Depth ] = deptpTex;
   m_UsedTextures[ UsdTex_Mrt0 ] = mrt0Tex;
   m_UsedTextures[ UsdTex_SSAO ] = ssaoTex;
   m_UsedTextures[ UsdTex_SSAOBlur ] = ssaoBlurTex;
   }

void SSAORenderer::OnRender( void )
   {
   glUseProgram( m_Programs[ RenderPass_Sample ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBOs[ RenderPass_Sample ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Depth ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt0 ] );

   glActiveTexture( GL_TEXTURE2 );
   glBindTexture( GL_TEXTURE_2D, m_NoiseTex );

   OpenglRenderHelper::RenderQuad();

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   glUseProgram( 0 );
   }