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
#include "DeferredMainRenderer.h"
#include "RendererLoader.h"
#include "RenderManager.h"
#include "..\Light.h"
#include "..\..\Debugging\vsGLInfoLib.h"

#define TILE_WIDTH 16u
#define TILE_HEIGHT 16u

#define GEOMETRY_PASS_VERTEX_LOCATION 0
#define GEOMETRY_PASS_UV_LOCATION 2
#define GEOMETRY_PASS_NORMAL_LOCATION 1

#define LIGHT_PASS_VERTEX_LOCATION    0
#define LIGHT_PASS_UV_LOCATION        1

const char* const TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME = "Effects\\TileFrustumGeneration.cs.glsl";

const char* const LIGHT_CULL_COMPUTE_SHADER_FILE_NAME = "Effects\\LightCulling.cs.glsl";

const char* const GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredGeometry.vs.glsl";
const char* const GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredGeometry.fs.glsl";

const char* const LIGHT_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredLighting.vs.glsl";
const char* const LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredLighting.fs.glsl";

const GLfloat QUAD_VERTEX_POSITION[] = {
   -1.0f, -1.0f, // bottom left
   1.0f, -1.0f, // bottom right
   1.0f, 1.0f, // top right
   -1.0f, 1.0f // top left
   };

const GLfloat QUAD_UV_POSITION[] = {
   0.0f, 0.0f, // bottom left
   1.0f, 0.0f, // bottom right
   1.0f, 1.0f, // top right
   0.0f, 1.0f // top left
   };

const unsigned short QUAD_VERTEX_INDEX[] = { 0, 1, 2, 0, 2, 3 };

DeferredMainRenderer::DeferredMainRenderer( void )
   {
   m_Shaders[ RenderPass_Geometry ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( Resource( GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME ) ) ) );
   m_Shaders[ RenderPass_Geometry ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( Resource( GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) );

   m_Shaders[ RenderPass_LightCulling ].push_back( shared_ptr< OpenGLShader >( ENG_NEW ComputeShader( Resource( LIGHT_CULL_COMPUTE_SHADER_FILE_NAME ) ) ) );
   
   m_Shaders[ RenderPass_Lighting ].push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) ) ) );
   m_Shaders[ RenderPass_Lighting ].push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ) );

   //m_VertexShaders[ RenderPass_LightCalc ].VSetResource( Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) );
   //m_FragmentShaders[ RenderPass_LightCalc ].VSetResource( Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_Uniforms[ RenderPass_Geometry ] = std::vector< GLuint >( GeometryPassUni_Num, -1 );
   m_Uniforms[ RenderPass_LightCulling ] = std::vector< GLuint >( LightCullPassUni_Num, -1 );
   m_Uniforms[ RenderPass_Lighting ] = std::vector< GLuint >( LightingPassUni_Num, -1 );

   m_TileFrustumShader.VSetResource( Resource( TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME ) );

   ENG_ZERO_MEM( m_SSBOs );

   auto screenSize = g_pApp->GetScreenSize();
   m_TileNum[ 0 ] = ( unsigned int ) std::ceil( ( double ) screenSize.x / TILE_WIDTH );
   m_TileNum[ 1 ] = ( unsigned int ) std::ceil( ( double ) screenSize.y / TILE_HEIGHT );

   ENG_ASSERT( m_TileNum[ 0 ] * TILE_WIDTH >= screenSize.x );
   ENG_ASSERT( m_TileNum[ 1 ] * TILE_HEIGHT >= screenSize.y );
   
   }

void DeferredMainRenderer::VShutdown( void )
   {
 
   }

int DeferredMainRenderer::VPreRender( void )
   {
   MainRenderer::VPreRender();
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   return S_OK;
   }

int DeferredMainRenderer::VOnRestore( Scene* pScene )
   {
   ReleaseResource();

   OnRestoreSSBO();

   OnRestoreTextures();

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

void DeferredMainRenderer::VLoadLight( Lights& lights )
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
   //ptr = ( LightProperties * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );
   //
   //for( int i = 0; i < MAXIMUM_LIGHTS_SUPPORTED; ++i )
   //   {
   //   std::stringstream ss;
   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_Color ) );
   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_Type ) );

   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_PositionVS ) );
   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_Enabled ) );

   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_DirectionVS ) );

   //   ENG_LOG( "Test", ToStr( ptr[ i ].m_Attenuation ) );
   //   }
   //glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

   
   OpenGLRenderManager::CheckError();
   }

//int OpenGLDeferredRenderer::VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode )
//   {
//   glUseProgram( m_Programs[ RenderPass_Geometry ] );
//   glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );
//   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );
//
//  /* glEnable( GL_CULL_FACE );
//   glCullFace( GL_BACK );
//   glEnable( GL_DEPTH_TEST );*/
//  // auto viewTest = Mat4x4::LookAt( Vec3( 5, 5, 0 ), Vec3( 5, 5, 1 ), g_Up );
//   auto view = pScene->GetCamera()->GetView();
//   auto proj = pScene->GetCamera()->GetProjection();
//   auto model = pNode->VGetGlobalTransformPtr()->GetToWorld();
//   //auto mvp = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * pNode->VGetGlobalTransformPtr()->GetToWorld();
//  // auto mvp = pScene->GetCamera()->GetProjection() * viewTest * pNode->VGetGlobalTransformPtr()->GetToWorld();
//   auto mvp = proj * view * model;
//   glUniformMatrix4fv( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ], 1, GL_FALSE, &mvp[ 0 ][ 0 ] );
//
//   Mat4x4 normalMat = ( proj * view );
//   normalMat = normalMat.Inverse().Transpose();
//
//  /* glUniformMatrix4fv( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ], 1, GL_FALSE, &normalMat[ 0 ][ 0 ] );*/
//
//   auto bufferObj = pNode->VGetShadowVertexInfo();
//   if( !bufferObj.m_Vertexbuffer || !bufferObj.m_NormalBuffer || !bufferObj.m_IndexBuffer )
//      {
//      return S_OK;
//      }
//
//   glBindBuffer( GL_ARRAY_BUFFER, bufferObj.m_Vertexbuffer );
//   glEnableVertexAttribArray( GEOMETRY_PASS_VERTEX_LOCATION );
//   glVertexAttribPointer(
//      GEOMETRY_PASS_VERTEX_LOCATION, 
//      3,                
//      GL_FLOAT,           
//      GL_FALSE,           
//      0,               
//      ( void* ) 0         
//      );
//
//   /*glBindBuffer( GL_ARRAY_BUFFER, bufferObj.m_UVBuffer );
//   glEnableVertexAttribArray( GEOMETRY_PASS_UV_LOCATION );
//   glVertexAttribPointer(
//      GEOMETRY_PASS_UV_LOCATION,
//      2,
//      GL_FLOAT,
//      GL_FALSE,
//      0,
//      ( void* ) 0
//      );*/
//
//   /*glBindBuffer( GL_VERTEX_ARRAY, bufferObj.m_NormalBuffer );
//   glEnableVertexAttribArray( GEOMETRY_PASS_NORMAL_LOCATION );
//   glVertexAttribPointer(
//      GEOMETRY_PASS_NORMAL_LOCATION,
//      3,
//      GL_FLOAT,
//      GL_FALSE,
//      0,
//      ( void* ) 0
//      );*/
//
//   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferObj.m_IndexBuffer );
//
//   /*glActiveTexture( GL_TEXTURE0 );
//   glBindTexture( GL_TEXTURE_2D, bufferObj.m_TextureObj );
//   glUniform1i( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ], 0 );*/
//
//   glDrawElements(
//      GL_TRIANGLES,    
//      bufferObj.m_VertexCount,   
//      GL_UNSIGNED_INT, 
//      ( void* ) 0        
//      );
//
//   ENG_ASSERT( !glGetError() );
//
//   glDisableVertexAttribArray( GEOMETRY_PASS_VERTEX_LOCATION );
//   glUseProgram( 0 );
//   glBindVertexArray( 0 );
//   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//
//   return S_OK;
//   }

void DeferredMainRenderer::ReleaseResource( void ) 
   {
   

//   return S_OK;
   }

int DeferredMainRenderer::OnRestoreSSBO( void )
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


int DeferredMainRenderer::OnRestoreTextures( void )
   {
   auto screenSize = g_pApp->GetScreenSize();

   // Create all textures at once
   glGenTextures( SST_Num, m_SST );
   for( int i = 0; i < SST_Num; ++i )
      {
      ENG_ASSERT( m_SST[ i ] );
      }
   // MRT 0
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_NormalGlossiness ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   // MRT 1
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_AlbedoMetalness ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   // Depth texture
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   // For depth test
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   // Tile debugging
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_TileDebugging ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glBindTexture( GL_TEXTURE_2D, 0 );
   // glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

int DeferredMainRenderer::OnRestoreTileFrustum( Scene* pScene )
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

int DeferredMainRenderer::OnRestoreGeometryPass( void )
   {
   GenerateProgram( RenderPass_Geometry );

 //  glGenVertexArrays( 1, &m_VAOs[ RenderPass_Geometry ] );
 //  glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );

   glGenFramebuffers( 1, &m_FBO[ RenderPass_Geometry ] );
   ENG_ASSERT( m_FBO[ RenderPass_Geometry ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );

  // glDrawBuffer( GL_NONE );
   // Depth buffer
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_SST[ SST_Depth ], 0 );

   // MRT 0 
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_NormalGlossiness ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_SST[ SST_NormalGlossiness ], 0 );

   // MRT 1
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_AlbedoMetalness ] );
   glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_SST[ SST_AlbedoMetalness ], 0 );

   GLuint outputAttatchments[ ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, /*GL_COLOR_ATTACHMENT2*/ };
   glDrawBuffers( 2, outputAttatchments );

   glBindTexture( GL_TEXTURE_2D, 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

  //  glDrawBuffer( GL_NONE );
   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] != -1 );

   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uNormal" );
   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] != -1 );
    
   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_AlbedoTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uAlbedoTex" );
   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_AlbedoTexture ] != -1 );

   auto result = glCheckFramebufferStatus( GL_FRAMEBUFFER );
   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );

   glBindVertexArray( 0 );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

int DeferredMainRenderer::OnRestourLightCullPass( Scene* pScene )
   {
   GenerateProgram( RenderPass_LightCulling );

   glUseProgram( m_Programs[ RenderPass_LightCulling ] );

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

int DeferredMainRenderer::OnRestoreLightingPass( Scene* pScene )
   {
   GenerateProgram( RenderPass_Lighting );

   glUseProgram( m_Programs[ RenderPass_Lighting ] );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_DepthTex ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uDepthTex" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_DepthTex ], 0 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT0 ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uMRT0" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT0 ], 1 );

   m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT1 ] = glGetUniformLocation( m_Programs[ RenderPass_Lighting ], "uMRT1" );
   glUniform1i( m_Uniforms[ RenderPass_Lighting ][ LightingPassUni_MRT1 ], 2 );

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

   glGenVertexArrays( 1, &m_LightingVAO );
   glBindVertexArray( m_LightingVAO );
   // 1st attribute buffer : vertices
   glGenBuffers( 1, &m_Buffers[ VBOs_Vertex ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ VBOs_Vertex ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_POSITION ), QUAD_VERTEX_POSITION, GL_STATIC_DRAW );
   glEnableVertexAttribArray( LIGHT_PASS_VERTEX_LOCATION );
   glVertexAttribPointer(
      LIGHT_PASS_VERTEX_LOCATION,                  // attribute
      2,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   // 2nd attribute buffer : UVs
   glGenBuffers( 1, &m_Buffers[ VBOs_UV ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ VBOs_UV ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_UV_POSITION ), QUAD_UV_POSITION, GL_STATIC_DRAW );
   glEnableVertexAttribArray( LIGHT_PASS_UV_LOCATION );
   glVertexAttribPointer(
      LIGHT_PASS_UV_LOCATION,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( void* ) 0                          // array buffer offset
      );

   glGenBuffers( 1, &m_Buffers[ VBOs_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Buffers[ VBOs_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_INDEX ), QUAD_VERTEX_INDEX, GL_STATIC_DRAW );

   glBindVertexArray( 0 );

   OpenGLRenderManager::CheckError();
   return S_OK;
   }

void DeferredMainRenderer::LightCulling( Scene* pScene )
   {
   glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

   glUseProgram( m_Programs[ RenderPass_LightCulling ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_SSBOs[ SSBO_LightIndexCount ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, m_SSBOs[ SSBO_LightIndexList ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, m_SSBOs[ SSBO_LightIndexGrid ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, m_SSBOs[ SSBO_LightProperties ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, m_SSBOs[ SSBO_TileFrustum ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_TileDebugging ] );
   glBindImageTexture( 1, m_SST[ SST_TileDebugging ], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16 );

   glDispatchCompute( m_TileNum[ 0 ], m_TileNum[ 1 ], 1u );

   glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BUFFER );

   glUseProgram( 0 );

   OpenGLRenderManager::CheckError();
   }

void DeferredMainRenderer::CalculateLighting( void )
   {
   glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

   glUseProgram( m_Programs[ RenderPass_Lighting ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_SSBOs[ SSBO_LightIndexList ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, m_SSBOs[ SSBO_LightIndexGrid ] );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, m_SSBOs[ SSBO_LightProperties ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_NormalGlossiness ] );

   glActiveTexture( GL_TEXTURE2 );
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_AlbedoMetalness ] );

   glBindVertexArray( m_LightingVAO );

   glDrawElements(
      GL_TRIANGLES,      // mode
      ENG_ARRAY_SIZE_IN_ELEMENTS( QUAD_VERTEX_INDEX ),    // count
      GL_UNSIGNED_SHORT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );

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

void DeferredMainRenderer::GenerateProgram( unsigned int renderPass )
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