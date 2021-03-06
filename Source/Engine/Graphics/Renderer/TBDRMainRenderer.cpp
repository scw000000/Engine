/*!
 * \file DeferredShader.cpp
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
#include "EngineStd.h"
#include "TBDRMainRenderer.h"
#include "RendererLoader.h"
#include "RenderManager.h"
#include "..\Light.h"
#include "..\..\Debugging\vsGLInfoLib.h"
#include "RenderHelper.h"

#define TILE_WIDTH 16u
#define TILE_HEIGHT 16u

#define GEOMETRY_PASS_VERTEX_LOCATION 0
#define GEOMETRY_PASS_UV_LOCATION 2
#define GEOMETRY_PASS_NORMAL_LOCATION 1

const char* const TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME = "Effects\\TileFrustumGeneration.cs.glsl";

const char* const LIGHT_CULL_COMPUTE_SHADER_FILE_NAME = "Effects\\LightCulling.cs.glsl";

const char* const GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredGeometry.vs.glsl";
const char* const GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredGeometry.fs.glsl";

const char* const GEOMETRY_SK_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredGeometrySK.vs.glsl";
const char* const GEOMETRY_SK_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredGeometry.fs.glsl";

const char* const LIGHT_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredLighting.vs.glsl";
const char* const LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredLighting.fs.glsl";

TBDRMainRenderer::TBDRMainRenderer( void ) : 
   m_TileFrustumShader( shared_ptr< Resource >( ENG_NEW Resource( TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME ) ) )
   {
   m_Shaders[ RenderPass_Geometry ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPass_Geometry ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );

   m_Shaders[ RenderPass_GeometrySK ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( GEOMETRY_SK_PASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPass_GeometrySK ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( GEOMETRY_SK_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );

   m_Shaders[ RenderPass_LightCulling ].push_back( shared_ptr< OpenGLShader >( ENG_NEW ComputeShader( shared_ptr< Resource >( ENG_NEW Resource( LIGHT_CULL_COMPUTE_SHADER_FILE_NAME ) ) ) ) );
   
   m_Shaders[ RenderPass_Lighting ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shaders[ RenderPass_Lighting ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) ) );

   //m_VertexShaders[ RenderPass_LightCalc ].VSetResource( Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) );
   //m_FragmentShaders[ RenderPass_LightCalc ].VSetResource( Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_Uniforms[ RenderPass_Geometry ] = std::vector< GLuint >( GeometryPassUni_Num, -1 );
   m_Uniforms[ RenderPass_GeometrySK ] = std::vector< GLuint >( GeometrySKPassUni_Num, -1 );
   m_Uniforms[ RenderPass_LightCulling ] = std::vector< GLuint >( LightCullPassUni_Num, -1 );
   m_Uniforms[ RenderPass_Lighting ] = std::vector< GLuint >( LightingPassUni_Num, -1 );

   ENG_ZERO_MEM( m_SSBOs );

   auto screenSize = g_pApp->GetScreenSize();
   m_TileNum[ 0 ] = ( unsigned int ) std::ceil( ( double ) screenSize.x / TILE_WIDTH );
   m_TileNum[ 1 ] = ( unsigned int ) std::ceil( ( double ) screenSize.y / TILE_HEIGHT );

   ENG_ASSERT( m_TileNum[ 0 ] * TILE_WIDTH >= screenSize.x );
   ENG_ASSERT( m_TileNum[ 1 ] * TILE_HEIGHT >= screenSize.y );
   
   }

void TBDRMainRenderer::VShutdown( void )
   {
 
   }

int TBDRMainRenderer::VOnRestore( Scene* pScene )
   {
   ReleaseResource();

   OnRestoreSSBO();

   OnRestoreTileFrustum( pScene );

   if( OnRestoreGeometryPass() != S_OK )
      {
      ENG_ASSERT( "Geometry Pass Restore Failed" );
      return S_FALSE;
      }

   if( OnRestourLightCullPass( pScene ) != S_OK )
      {
      ENG_ASSERT( "Light Culling Pass Restore Failed" );
      return S_FALSE;
      }

   if( OnRestoreLightingPass( pScene ) != S_OK )
      {
      ENG_ASSERT( "Geometry Pass Restore Failed" );
      return S_FALSE;
      }
   
   return S_OK;
   }

int TBDRMainRenderer::VPreRender( void )
   {
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );
   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   glClear( GL_DEPTH_BUFFER_BIT );
   return S_OK;
   }

int TBDRMainRenderer::VPostRender( void )
   {
   SDL_GL_SwapWindow( g_pApp->GetWindow() );
   return S_OK;
   }

void TBDRMainRenderer::VLoadLight( Lights& lights )
   {
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightProperties ] );

   LightProperties *ptr;
   ptr = ( LightProperties * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY );

   for( Lights::iterator lightIt = lights.begin(); lightIt != lights.end(); ++lightIt )
      {
      //   ptr = static_cast<char*>( ptr ) + sizeof( LightProperties );
      memcpy( ptr, lightIt->get()->GetLightPropertiesPtr().get(), sizeof( LightProperties ) );
      ++ptr;
      }

   glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightIndexCount ] );
      glClearBufferData( GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED, GL_UNSIGNED_INT, NULL );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

   glUseProgram( m_Programs[ RenderPass_LightCulling ] );
      glUniform1ui( m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_ValidLightNum ], std::min( lights.size(), MAXIMUM_LIGHTS_SUPPORTED ) );
   glUseProgram( 0 );

#ifdef _DEBUG
   /*bool test = false;
   if( test )
   {
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightProperties ] );
   ptr = ( LightProperties * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );
   if( ptr )
   {
   for( int i = 0; i < lights.size(); ++i )
   {
   std::stringstream ss;
   ENG_LOG( "Test", ToStr( ptr[ i ].m_Color ) );
   ENG_LOG( "Test", ToStr( ptr[ i ].m_Type ) );

   ENG_LOG( "Test", ToStr( ptr[ i ].m_PositionVS ) );
   ENG_LOG( "Test", ToStr( ptr[ i ].m_Enabled ) );

   ENG_LOG( "Test", ToStr( ptr[ i ].m_DirectionVS ) );

   ENG_LOG( "Test", ToStr( ptr[ i ].m_Attenuation ) );
   }
   glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
   }
   }*/
#endif // _DEBUG
   OpenGLRenderManager::CheckError();
   }

void TBDRMainRenderer::ReleaseResource( void ) 
   {
   // TODO: release SSBO & programs

//   return S_OK;
   }

int TBDRMainRenderer::OnRestoreSSBO( void )
   {
   glGenBuffers( SSBO_Num, m_SSBOs );
   for( int i = 0; i < SSBO_Num; ++i )
      {
      ENG_ASSERT( m_SSBOs[ i ] );
      }

   // Alignment checking
   ENG_ASSERT( sizeof( tileFrustum ) == 64 );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_TileFrustum ] );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( tileFrustum ) * m_TileNum[ 0 ] * m_TileNum[ 1 ], NULL, GL_DYNAMIC_DRAW );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightIndexCount ] );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( GLuint ), NULL, GL_DYNAMIC_DRAW );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightIndexList ] );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( GLuint ) * AVERAGE_OVERLAP_LIGHTS_PER_TILE * m_TileNum[ 0 ] * m_TileNum[ 1 ], NULL, GL_DYNAMIC_DRAW );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightIndexGrid ] );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( GLuint ) * 2 * m_TileNum[ 0 ] * m_TileNum[ 1 ], NULL, GL_DYNAMIC_DRAW );

   // Alignment checking
   ENG_ASSERT( sizeof( LightProperties ) % 16 == 0 );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightProperties ] );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( LightProperties ) * MAXIMUM_LIGHTS_SUPPORTED, NULL, GL_DYNAMIC_DRAW );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );


   OpenGLRenderManager::CheckError();
   //VSGLInfoLib::getBufferInfo( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightProperties ] );
   return S_OK;
   }


int TBDRMainRenderer::OnRestoreTextures( GLuint depTex
                                             , GLuint mrt0Tex
                                             , GLuint mrt1Tex
                                             , GLuint mrt2Tex
#ifdef _DEBUG
                                             , GLuint tileDebugTex
#endif // _DEBUG
                                             , GLuint lightingTex
                                             , GLuint lightThresHoldTex
                                             )
   {
   m_UsedTextures[ UsdTex_Depth ] = depTex;
   m_UsedTextures[ UsdTex_Mrt0 ] = mrt0Tex;
   m_UsedTextures[ UsdTex_Mrt1 ] = mrt1Tex;
   m_UsedTextures[ UsdTex_Mrt2 ] = mrt2Tex;
#ifdef _DEBUG
   m_UsedTextures[ UsdTex_TileDebugging ] = tileDebugTex;
#endif // _DEBUG
   m_UsedTextures[ UsdTex_Lighting ] = lightingTex;
   m_UsedTextures[ UsdTex_LightThreshold ] = lightThresHoldTex;
   return S_OK;
   }

int TBDRMainRenderer::OnRestoreTileFrustum( Scene* pScene )
   {
   m_TileFrustumShader.VOnRestore();
   auto program = OpenGLRendererLoader::GenerateProgram( { m_TileFrustumShader.GetShaderObject() } );
   m_TileFrustumShader.VReleaseShader( program );
   glUseProgram( program );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_SSBOs[ SSBO_TileFrustum ] );

   auto tileSizeUni = glGetUniformLocation( program, "uTileSize" );
   glUniform2ui( tileSizeUni, TILE_WIDTH, TILE_HEIGHT );

   auto screenSize = g_pApp->GetScreenSize();
   auto screenSizeUni = glGetUniformLocation( program, "uScreenSize" );
   glUniform2ui( screenSizeUni, ( GLuint ) screenSize.x, ( GLuint ) screenSize.y );

   auto pCamera = pScene->GetCamera();
   float halfSizeNearPlaneY = std::tan( pCamera->GetFrustum().m_FovY / 2.0f );
   float halfSizeNearPlaneX = halfSizeNearPlaneY * pCamera->GetFrustum().m_Aspect;
   auto halfSizeNearPlaneUni = glGetUniformLocation( program, "uHalfSizeNearPlane" );
   glUniform2f( halfSizeNearPlaneUni, halfSizeNearPlaneX, halfSizeNearPlaneY );

   auto proj = pCamera->GetProjection();
   auto uProjUni = glGetUniformLocation( program, "uProj" );
   glUniformMatrix4fv( uProjUni, 1, GL_FALSE, &proj[ 0 ][ 0 ] );

   //auto invProj = proj.Inverse();
   //auto invProjUni = glGetUniformLocation( program, "uInvProj" );
   //glUniformMatrix4fv( invProjUni, 1, GL_FALSE, &invProj[ 0 ][ 0 ] );

   OpenGLRenderManager::CheckError();
   glDispatchCompute( m_TileNum[ 0 ], m_TileNum[ 1 ], 1u );
   glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_TileFrustum ] );

   // GLfloat *ptr;
   // ptr = ( GLfloat * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );

   //for( int i = 0; i < m_TileNum[ 0 ] * m_TileNum[ 1 ]; i++ )
   //   {
   //   for( int j = 0; j < 4; ++j )
   //      {
   //      if( i / m_TileNum[ 0 ] == 0 )
   //            {
   //            std::stringstream ss;
   //            ss << "T: " << i << "P: " << j << ": "
   //               << ptr[ i * 16 + j * 4 + 0 ] << ", "
   //               << ptr[ i * 16 + j * 4 + 1 ] << ", "
   //               << ptr[ i * 16 + j * 4 + 2 ] << ", "
   //               << ptr[ i * 16 + j * 4 + 3 ] << std::endl;
   //            ENG_LOG( "Test", ss.str() );
   //            }
   //      }

   //   }
   // glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

   glDeleteProgram( program );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

int TBDRMainRenderer::OnRestoreGeometryPass()
   {
   std::vector< RenderPass > geometryPasses = { RenderPass_Geometry, RenderPass_GeometrySK };
   for( auto geometryPass : geometryPasses )
      {
      GenerateProgram( geometryPass );

      glGenFramebuffers( 1, &m_FBO[ geometryPass ] );
      ENG_ASSERT( m_FBO[ geometryPass ] );
      glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ geometryPass ] );

      // glDrawBuffer( GL_NONE );
      // Depth buffer
      glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Depth ] );
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Depth ], 0 );

      // MRT 0 
      glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt0 ] );
      glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_Mrt0 ], 0 );

      // MRT 1
      glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt1 ] );
      glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_UsedTextures[ UsdTex_Mrt1 ], 0 );

      // MRT 2
      glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt2 ] );
      glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_UsedTextures[ UsdTex_Mrt2 ], 0 );

      GLuint outputAttatchments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
      glDrawBuffers( 3, outputAttatchments );

      glBindTexture( GL_TEXTURE_2D, 0 );
      glBindFramebuffer( GL_FRAMEBUFFER, 0 );

      //  glDrawBuffer( GL_NONE );
      m_Uniforms[ geometryPass ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ geometryPass ], "uMVP" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_MVP ] != -1 );

      m_Uniforms[ geometryPass ][ GeometryPassUni_PrevMVP ] = glGetUniformLocation( m_Programs[ geometryPass ], "uPrevMVP" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_PrevMVP ] != -1 );

      m_Uniforms[ geometryPass ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ geometryPass ], "uNormal" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_NormalMat ] != -1 );

      m_Uniforms[ geometryPass ][ GeometryPassUni_AlbedoTexture ] = glGetUniformLocation( m_Programs[ geometryPass ], "uAlbedoTex" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_AlbedoTexture ] != -1 );

      m_Uniforms[ geometryPass ][ GeometryPassUni_NormalMapTexture ] = glGetUniformLocation( m_Programs[ geometryPass ], "uNormalMapTex" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_MVP ] != -1 );

      m_Uniforms[ geometryPass ][ GeometryPassUni_UseNormalMap ] = glGetUniformLocation( m_Programs[ geometryPass ], "uUseNormalMap" );
      ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometryPassUni_MVP ] != -1 );

      if( geometryPass == RenderPass_GeometrySK )
         {
         m_Uniforms[ geometryPass ][ GeometrySKPassUni_BoneTransform ] = glGetUniformLocation( m_Programs[ geometryPass ], "uBoneTransform" );
         ENG_ASSERT( m_Uniforms[ geometryPass ][ GeometrySKPassUni_BoneTransform ] != -1 );
         }

      glUseProgram( m_Programs[ geometryPass ] );
      glUniform1i( m_Uniforms[ geometryPass ][ GeometryPassUni_AlbedoTexture ], 0 );
      glUniform1i( m_Uniforms[ geometryPass ][ GeometryPassUni_NormalMapTexture ], 1 );
      glUseProgram( 0 );

      auto result = glCheckFramebufferStatus( GL_FRAMEBUFFER );
      ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );

      glBindVertexArray( 0 );
      OpenGLRenderManager::CheckError();
      }
   
   return S_OK;
   }

int TBDRMainRenderer::OnRestourLightCullPass( Scene* pScene )
   {
   GenerateProgram( RenderPass_LightCulling );

   glUseProgram( m_Programs[ RenderPass_LightCulling ] );

   glGenFramebuffers( 1, &m_FBO[ RenderPass_Lighting ] );
   ENG_ASSERT( m_FBO[ RenderPass_Lighting ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Lighting ] );

   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Lighting ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_UsedTextures[ UsdTex_Lighting ], 0 );

   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_LightThreshold ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_UsedTextures[ UsdTex_LightThreshold ], 0 );

   GLuint outputAttatchments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
   glDrawBuffers( 2, outputAttatchments );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_DepthTex ] = glGetUniformLocation( m_Programs[ RenderPass_LightCulling ], "uDepthTex" );
   glUniform1i( m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_DepthTex ], 0 );

   m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_Proj ] = glGetUniformLocation( m_Programs[ RenderPass_LightCulling ], "uProj" );
   auto projMat = pScene->GetCamera()->GetProjection();
   glUniformMatrix4fv( m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_Proj ], 1, GL_FALSE, &projMat[ 0 ][ 0 ] );

   m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_ScreenSize ] = glGetUniformLocation( m_Programs[ RenderPass_LightCulling ], "uScreenSize" );
   auto screenSize = g_pApp->GetScreenSize();
   glUniform2ui( m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_ScreenSize ], screenSize.x, screenSize.y );

   m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_ValidLightNum ] = glGetUniformLocation( m_Programs[ RenderPass_LightCulling ], "uValidLightNum" );

   m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_DebugTex ] = glGetUniformLocation( m_Programs[ RenderPass_LightCulling ], "debugTex" );
   glUniform1i( m_Uniforms[ RenderPass_LightCulling ][ LightCullPassUni_DebugTex ], 1 );
   
   glUseProgram( 0 );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

int TBDRMainRenderer::OnRestoreLightingPass( Scene* pScene )
   {
   GenerateProgram( RenderPass_Lighting );

   glUseProgram( m_Programs[ RenderPass_Lighting ] );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_DepthTex ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uDepthTex" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_DepthTex ], 0 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT0 ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uMRT0" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT0 ], 1 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT1 ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uMRT1" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT1 ], 2 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_SSAOBlurTex ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uSSAOBlurTex" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_SSAOBlurTex ], 3 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_UseSSAO ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uUseSSAO" );
   glUniform1ui( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_UseSSAO ], 1u );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_TileNum ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uTileNum" );
   glUniform2ui( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_TileNum ], m_TileNum[ 0 ], m_TileNum[ 1 ] );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_HalfSizeNearPlane ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uHalfSizeNearPlane" );
   auto pCamera = pScene->GetCamera();
   float halfSizeNearPlaneY = std::tan( pCamera->GetFrustum().m_FovY / 2.0f );
   float halfSizeNearPlaneX = halfSizeNearPlaneY * pCamera->GetFrustum().m_Aspect;
   glUniform2f( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_HalfSizeNearPlane ], halfSizeNearPlaneX, halfSizeNearPlaneY );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_Proj ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uProj" );
   auto proj = pCamera->GetProjection();
   glUniformMatrix4fv( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_Proj ], 1, GL_FALSE, &proj[ 0 ][ 0 ] );

   glUseProgram( 0 );
   
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

void TBDRMainRenderer::LightCulling( Scene* pScene )
   {
   glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

   glUseProgram( m_Programs[ RenderPass_LightCulling ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_SSBOs[ SSBO_LightIndexCount ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, m_SSBOs[ SSBO_LightIndexList ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, m_SSBOs[ SSBO_LightIndexGrid ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, m_SSBOs[ SSBO_LightProperties ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, m_SSBOs[ SSBO_TileFrustum ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Depth ] );

#ifdef _DEBUG
   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_TileDebugging ] );
   glBindImageTexture( 1, m_UsedTextures[ UsdTex_TileDebugging ], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16 );
#endif // _DEBUG

   glDispatchCompute( m_TileNum[ 0 ], m_TileNum[ 1 ], 1u );

   glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BUFFER );

   glUseProgram( 0 );

   OpenGLRenderManager::CheckError();
   }

void TBDRMainRenderer::CalculateLighting( void )
   {
   glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Lighting ] );

   glUseProgram( m_Programs[ RenderPass_Lighting ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_SSBOs[ SSBO_LightIndexList ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, m_SSBOs[ SSBO_LightIndexGrid ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, m_SSBOs[ SSBO_LightProperties ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Depth ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt0 ] );

   glActiveTexture( GL_TEXTURE2 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_Mrt1 ] );

   glActiveTexture( GL_TEXTURE3 );
   glBindTexture( GL_TEXTURE_2D, m_UsedTextures[ UsdTex_SSAOBlur ] );

   OpenglRenderHelper::RenderQuad();

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   glUseProgram( 0 );

   /*static bool shouldTest = false;
   if( shouldTest )
   {
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_SSBOs[ SSBO_LightProperties ] );
   LightProperties* ptr = ( LightProperties * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );
   for( int i = 0; i < 2; ++i )
   {
   std::cout << ToStr( ptr[ i ].m_Color ) << std::endl;
   std::cout << ToStr( ptr[ i ].m_Enabled ) << std::endl;
   std::cout << ToStr( ptr[ i ].m_PositionVS ) << std::endl;
   std::cout << ToStr( ptr[ i ].m_DirectionVS ) << std::endl;
   std::cout << ToStr( ptr[ i ].m_Type ) << std::endl;
   std::cout << ToStr( ptr[ i ].m_Attenuation ) << std::endl;
   }

   glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
   }*/

   OpenGLRenderManager::CheckError();
   }

void TBDRMainRenderer::GenerateProgram( unsigned int renderPass )
   {
   for( auto shader : m_Shaders[ renderPass ] )
      {
      shader->VOnRestore();
      }

   m_Programs[ renderPass ] = OpenGLRendererLoader::GenerateProgram( m_Shaders[ renderPass ] );

   for( auto shader : m_Shaders[ renderPass ] )
      {
      shader->VReleaseShader( m_Programs[ renderPass ] );
      }
   }