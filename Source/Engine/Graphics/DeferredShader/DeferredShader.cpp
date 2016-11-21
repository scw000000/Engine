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
#include "DeferredShader.h"

#define GEOMETRY_PASS_VERTEX_LOCATION 0
#define GEOMETRY_PASS_UV_LOCATION 2
#define GEOMETRY_PASS_NORMAL_LOCATION 1


const char* const GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\MRT.vertexshader";
const char* const GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\MRT.fragmentshader";

const char* const LIGHT_PASS_VERTEX_SHADER_FILE_NAME = "";
const char* const LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME = "";

OpenGLDeferredShader::OpenGLDeferredShader( void )
   {
   m_VertexShaders[ RenderPass_Geometry ].VSetResource( Resource( GEOMETRY_PASS_VERTEX_SHADER_FILE_NAME ) );
   m_FragmentShaders[ RenderPass_Geometry ].VSetResource( Resource( GEOMETRY_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_VertexShaders[ RenderPass_Light ].VSetResource( Resource( LIGHT_PASS_VERTEX_SHADER_FILE_NAME ) );
   m_FragmentShaders[ RenderPass_Light ].VSetResource( Resource( LIGHT_PASS_FRAGMENT_SHADER_FILE_NAME ) );

   m_Uniforms = std::vector< std::vector< GLuint > >( RenderPass_Num );
   m_Uniforms[ RenderPass_Geometry ] = std::vector< GLuint >( GeometryPassUni_Num, -1 );

   }

void OpenGLDeferredShader::VPreRender( void )
   {
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   }

int OpenGLDeferredShader::VOnRestore( void )
   {
   ReleaseResource();

   if( OnRestoreGeometryPass() != S_OK )
      {
      ENG_ASSERT( "Geometry Pass Restore Failed" );
      return S_FALSE;
      }

   if( OnRestoreLightPass() != S_OK )
      {
      ENG_ASSERT( "Geometry Pass Restore Failed" );
      return S_FALSE;
      }
   
   return S_OK;
   }

int OpenGLDeferredShader::VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode )
   {
   glUseProgram( m_Programs[ RenderPass_Geometry ] );
   glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );

  /* glEnable( GL_CULL_FACE );
   glCullFace( GL_BACK );
   glEnable( GL_DEPTH_TEST );*/
  // auto viewTest = Mat4x4::LookAt( Vec3( 5, 5, 0 ), Vec3( 5, 5, 1 ), g_Up );
   auto view = pScene->GetCamera()->GetView();
   auto proj = pScene->GetCamera()->GetProjection();
   auto model = pNode->VGetGlobalTransformPtr()->GetToWorld();
   //auto mvp = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * pNode->VGetGlobalTransformPtr()->GetToWorld();
  // auto mvp = pScene->GetCamera()->GetProjection() * viewTest * pNode->VGetGlobalTransformPtr()->GetToWorld();
   auto mvp = proj * view * model;
   glUniformMatrix4fv( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ], 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

   Mat4x4 normalMat = ( proj * view );
   normalMat = normalMat.Inverse().Transpose();

   glUniformMatrix4fv( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ], 1, GL_FALSE, &normalMat[ 0 ][ 0 ] );

   auto bufferObj = pNode->VGetShadowVertexInfo();
   if( !bufferObj.m_Vertexbuffer || !bufferObj.m_NormalBuffer || !bufferObj.m_IndexBuffer )
      {
      return S_OK;
      }

   glBindBuffer( GL_VERTEX_ARRAY, bufferObj.m_Vertexbuffer );
   glEnableVertexAttribArray( GEOMETRY_PASS_VERTEX_LOCATION );
   glVertexAttribPointer(
      GEOMETRY_PASS_VERTEX_LOCATION, 
      3,                
      GL_FLOAT,           
      GL_FALSE,           
      0,               
      ( void* ) 0         
      );

   //glBindBuffer( GL_VERTEX_ARRAY, bufferObj.m_UVBuffer );
   //glEnableVertexAttribArray( GEOMETRY_PASS_UV_LOCATION );
   //glVertexAttribPointer(
   //   GEOMETRY_PASS_UV_LOCATION,
   //   2,
   //   GL_FLOAT,
   //   GL_FALSE,
   //   0,
   //   ( void* ) 0
   //   );

   /*glBindBuffer( GL_VERTEX_ARRAY, bufferObj.m_NormalBuffer );
   glEnableVertexAttribArray( GEOMETRY_PASS_NORMAL_LOCATION );
   glVertexAttribPointer(
      GEOMETRY_PASS_NORMAL_LOCATION,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      ( void* ) 0
      );*/

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferObj.m_IndexBuffer );

   //glActiveTexture( GL_TEXTURE0 );
   //glBindTexture( GL_TEXTURE_2D, bufferObj.m_TextureObj );
   //glUniform1i( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ], 0 );

   glDrawElements(
      GL_TRIANGLES,    
      bufferObj.m_VertexCount,   
      GL_UNSIGNED_INT, 
      ( void* ) 0        
      );

   glUseProgram( 0 );
   glBindVertexArray( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   return S_OK;
   }

void OpenGLDeferredShader::ReleaseResource( void ) 
   {
   


   }

int OpenGLDeferredShader::OnRestoreGeometryPass( void )
   {
   m_VertexShaders[ RenderPass_Geometry ].VOnRestore();

   m_FragmentShaders[ RenderPass_Geometry ].VOnRestore();

   m_Programs[ RenderPass_Geometry ] = OpenGLRenderer::GenerateProgram( m_VertexShaders[ RenderPass_Geometry ].VGetShaderObject(), m_FragmentShaders[ RenderPass_Geometry ].VGetShaderObject() );

   m_VertexShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );
   m_FragmentShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );

   glGenVertexArrays( 1, &m_VAOs[ RenderPass_Geometry ] );
   glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );

   glGenFramebuffers( 1, &m_FBO[ RenderPass_Geometry ] );
   ENG_ASSERT( m_FBO[ RenderPass_Geometry ] );
   glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );

   auto screenSize = g_pApp->GetScreenSize();

   // Create all textures at once
   glGenTextures( SST_Num, m_SST );
   for( int i = 0; i < SST_Num; ++i )
      {
      ENG_ASSERT( m_SST[ i ] );
      }   

   // Depth buffer
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_SST[ SST_Depth ], 0 );

   // Normal 
  // glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Normal ] );
  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  // glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_SST[ SST_Normal ], 0 );

   // Texture
  // glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Texture ] );
  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
  //// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  // glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_SST[ SST_Texture ], 0 );

   /*GLuint outputAttatchments[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
   glDrawBuffers( 1, outputAttatchments );*/

   m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MVP ] != -1 );

   /*m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uNMat" );
   ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_NormalMat ] != -1 );*/

   //m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMeshTexture" );
   //ENG_ASSERT( m_Uniforms[ RenderPass_Geometry ][ GeometryPassUni_MeshTexture ] != -1 );

   

   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );


   //m_VertexShaders[ RenderPass_Geometry ].VOnRestore();

   //m_FragmentShaders[ RenderPass_Geometry ].VOnRestore();

   //m_Programs[ RenderPass_Geometry ] = OpenGLRenderer::GenerateProgram( m_VertexShaders[ RenderPass_Geometry ].VGetShaderObject(), m_FragmentShaders[ RenderPass_Geometry ].VGetShaderObject() );

   //m_VertexShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );
   //m_FragmentShaders[ RenderPass_Geometry ].VReleaseShader( m_Programs[ RenderPass_Geometry ] );

   //glGenVertexArrays( 1, &m_VAOs[ RenderPass_Geometry ] );
   //glBindVertexArray( m_VAOs[ RenderPass_Geometry ] );

   //glGenFramebuffers( 1, &m_FBO[ RenderPass_Geometry ] );
   //ENG_ASSERT( m_FBO[ RenderPass_Geometry ] );
   //glBindFramebuffer( GL_FRAMEBUFFER, m_FBO[ RenderPass_Geometry ] );

   //auto screenSize = g_pApp->GetScreenSize();

   //// create a depth buffer for our framebuffer
   //GLuint depthBuffer;
   //glGenRenderbuffers( 1, &depthBuffer );
   //ENG_ASSERT( depthBuffer );
   //glBindRenderbuffer( GL_RENDERBUFFER, depthBuffer );
   //glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y );
   //glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_FBO[ RenderPass_Geometry ] );


   // Create all textures at once
  /* glGenTextures( SST_Num, m_SST );
   for( int i = 0; i < SST_Num; ++i )
      {
      ENG_ASSERT( m_SST[ i ] );
      }
*/
   // glBindRenderbuffer( GL_RENDERBUFFER, m_SST[ SST_Depth ] );
   //glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y );

   /*glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SST[ SST_Depth ], 0 );

   m_MVPUni = -1;
   m_MVPUni = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uMVP" );
   ENG_ASSERT( m_MVPUni != -1 );

   m_MUni = -1;
   m_MUni = glGetUniformLocation( m_Programs[ RenderPass_Geometry ], "uM" );
   ENG_ASSERT( m_MUni != -1 );

   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );*/

  // glDrawBuffer( GL_NONE );

   glUseProgram( 0 );
   glBindVertexArray( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   return S_OK;
   }

int OpenGLDeferredShader::OnRestoreLightPass( void )
   {
   return S_OK;
   }