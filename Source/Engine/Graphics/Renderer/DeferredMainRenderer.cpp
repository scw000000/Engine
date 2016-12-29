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
#include "..\..\Debugging\vsGLInfoLib.h"

#define TILE_WIDTH 16u
#define TILE_HEIGHT 16u

#define GEOMETRY_PASS_VERTEX_LOCATION 0
#define GEOMETRY_PASS_UV_LOCATION 2
#define GEOMETRY_PASS_NORMAL_LOCATION 1

const char* const TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME = "Effects\\TileFrustumShader.cs.glsl";

const char* const GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DeferredGeometry.vs.glsl";
const char* const GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DeferredGeometry.fs.glsl";

const char* const LIGHT_PASS_VERTEX_SHADER_FILE_NAME = "";
const char* const LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME = "";

DeferredMainRenderer::DeferredMainRenderer( void )
   {
   m_VertexShaders[ RenderPass_Geometry ].VSetResource( Resource( GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME ) );
   m_FragmentShaders[ RenderPass_Geometry ].VSetResource( Resource( GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_VertexShaders[ RenderPass_Light ].VSetResource( Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) );
   m_FragmentShaders[ RenderPass_Light ].VSetResource( Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_Uniforms = std::vector< std::vector< GLuint > >( RenderPass_Num );
   m_Uniforms[ RenderPass_Geometry ] = std::vector< GLuint >( GeometryPassUni_Num, -1 );

   m_TileFrustumShader.VSetResource( Resource( TILE_FRUSTUM_COMPUTE_SHADER_FILE_NAME ) );
   m_TileFrustumSSBO = 0;


   auto screenSize = g_pApp->GetScreenSize();
   m_TileNum[ 0 ] = std::ceil( ( double ) screenSize.x / TILE_WIDTH );
   m_TileNum[ 1 ] = std::ceil( ( double ) screenSize.y / TILE_HEIGHT );

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

int DeferredMainRenderer::VOnRestore( void )
   {
   ReleaseResource();

   OnRestoreSSBO();

   OnRestoreTileFrustum();

   OnRestoreTextures();

   if( OnRestoreGeometryPass() != S_OK )
      {
      ENG_ASSERT( "Geometry Pass Restore Failed" );
      return S_FALSE;
      }

   //if( OnRestoreLightPass() != S_OK )
   //   {
   //   ENG_ASSERT( "Geometry Pass Restore Failed" );
   //   return S_FALSE;
   //   }
   
   return S_OK;
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
   glGenBuffers( 1, &m_TileFrustumSSBO );
   ENG_ASSERT( m_TileFrustumSSBO );

   ENG_ASSERT( sizeof( tileFrustum ) == 64 );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_TileFrustumSSBO );
      glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( tileFrustum ) * m_TileNum[ 0 ] * m_TileNum[ 1 ], NULL, GL_DYNAMIC_DRAW );
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
   OpenGLRenderManager::CheckError();
   VSGLInfoLib::getBufferInfo( GL_SHADER_STORAGE_BUFFER, m_TileFrustumSSBO );
   return S_OK;
   }

int DeferredMainRenderer::OnRestoreTileFrustum( void )
   {

   m_TileFrustumShader.VOnRestore();
   auto program = OpenGLRendererLoader::GenerateProgram( { m_TileFrustumShader.VGetShaderObject() } );
   m_TileFrustumShader.VReleaseShader( program );
   glUseProgram( program );

   glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, m_TileFrustumSSBO );

   auto tileSizeUni = glGetUniformLocation( program, "uTileSize" );
   glUniform2ui( tileSizeUni, TILE_WIDTH, TILE_HEIGHT );

   auto screenSize = g_pApp->GetScreenSize();
   auto screenSizeUni = glGetUniformLocation( program, "uScreenSize" );
   glUniform2ui( screenSizeUni, ( GLuint ) screenSize.x, ( GLuint ) screenSize.y );

   auto pCamera = g_pApp->m_pEngineLogic->m_pWrold->GetCamera();
   float halfSizeNearPlaneY = std::tan( pCamera->GetFrustum().m_FovY / 2.0 );
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
   glBindBuffer( GL_SHADER_STORAGE_BUFFER, m_TileFrustumSSBO );

   GLfloat *ptr;
   ptr = ( GLfloat * ) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );

   for( int i = 0; i < m_TileNum[ 0 ] * m_TileNum[ 1 ]; i++ )
      {
      for( int j = 0; j < 4; ++j )
         {
         if( i / m_TileNum[ 0 ] == 0 )
               {
               std::stringstream ss;
               ss << "T: " << i << "P: " << j << ": "
                  << ptr[ i * 16 + j * 4 + 0 ] << ", "
                  << ptr[ i * 16 + j * 4 + 1 ] << ", "
                  << ptr[ i * 16 + j * 4 + 2 ] << ", "
                  << ptr[ i * 16 + j * 4 + 3 ] << std::endl;
               ENG_LOG( "Test", ss.str() );
               }
         }

      }

   glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

   glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
   OpenGLRenderManager::CheckError();
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
   // Normal texture
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_NormalGlossiness ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   //glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_SST[ SST_Normal ], 0 );
   
   //   // Texture
   //  // glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Texture ] );
   //  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
   //  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
   //  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
   //  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   //  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   //  // glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_SST[ SST_Texture ], 0 );
   //
   //   /*GLuint outputAttatchments[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
   //   glDrawBuffers( 1, outputAttatchments );*/
   //
   //   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
   //   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] != -1 );
   //
   //   /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uNMat" );
   //   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] != -1 );
   //*/
   //   /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMeshTexture" );
   //   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] != -1 );*/
   //
   //   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );

   // glDrawBuffer( GL_NONE );
   // Depth buffer
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   // For depth test
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   // glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

int DeferredMainRenderer::OnRestoreGeometryPass( void )
   {
   m_VertexShaders[ RenderPass_Geometry ].VOnRestore();

   m_FragmentShaders[ RenderPass_Geometry ].VOnRestore();

   m_Programs[ RenderPass_Geometry ] = OpenGLRendererLoader::GenerateProgram( { m_VertexShaders[ RenderPass_Geometry ].VGetShaderObject(), m_FragmentShaders[ RenderPass_Geometry ].VGetShaderObject() } );
 //  m_Programs[ RenderPass_Geometry ] = OpenGLRenderer::GenerateProgram( m_VertexShaders[ RenderPass_Geometry ].VGetShaderObject(), m_FragmentShaders[ RenderPass_Geometry ].VGetShaderObject() );

   m_VertexShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );
   m_FragmentShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );

   glGenVertexArrays( 1, &m_VAOs[ RenderPass_Geometry ] );
   glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );

   glGenFramebuffers( 1, &m_FBO[ RenderPass_Geometry ] );
   ENG_ASSERT( m_FBO[ RenderPass_Geometry ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );

//   // Depth buffer
//   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
//   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
//
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
//   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
//
//   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_SST[ SST_Depth ], 0 );
//
//   glBindTexture( GL_TEXTURE_2D, 0 );
//   // Normal 
//  // glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Normal ] );
//  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
//  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
//  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
//  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
//  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
//  // glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_SST[ SST_Normal ], 0 );
//   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
//   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] != -1 );
//
//   /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uNMat" );
//   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] != -1 );
//*/
//   /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMeshTexture" );
//   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] != -1 );*/
//
//   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );

  // glDrawBuffer( GL_NONE );
    // Depth buffer
    glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
    //glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    //// For depth test
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_SST[ SST_Depth ], 0 );
    
   // glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );

    
    // Normal 
    glBindTexture( GL_TEXTURE_2D, m_SST[ SST_NormalGlossiness ] );
    // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
    //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_SST[ SST_NormalGlossiness ], 0 );

    // Texture
    // glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Texture ] );
    // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
    //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    // glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_SST[ SST_Texture ], 0 );

    GLuint outputAttatchments[ ] = { GL_COLOR_ATTACHMENT0/*, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2*/ };
    glDrawBuffers( 1, outputAttatchments );
  //  glDrawBuffer( GL_NONE );
    m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
    ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] != -1 );

    m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uNormal" );
    ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] != -1 );
    
    /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMeshTexture" );
    ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] != -1 );*/
    auto result = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );
   
    glBindTexture( GL_TEXTURE_2D, 0 );
   glUseProgram( 0 );
   glBindVertexArray( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   return S_OK;
   }

int DeferredMainRenderer::OnRestoreLightPass( void )
   {
   return S_OK;
   }