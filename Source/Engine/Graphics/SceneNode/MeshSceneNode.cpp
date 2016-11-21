/*!
 * \file MeshSceneNode.cpp
 * \date 2016/05/02 11:10
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */

#include "EngineStd.h"
#include "MeshSceneNode.h"
#include "..\ResourceCache\MeshResource.h"
#include "..\ResourceCache\TextureResource.h"
#include "..\OpenGLRenderer.h"

#define VERTEX_LOCATION    0
#define UV_LOCATION        1
#define NORMAL_LOCATION    2

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\BasicVertexShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\BasicFragmentShader.fragmentshader";

MeshSceneNode::MeshSceneNode( 
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResouce, MaterialPtr pMaterial, RenderGroup renderGroup, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderGroup, pTransform, pMaterial ),
   m_pMeshResource( pMeshResouce ),
   m_VertexShader( Resource( VERTEX_SHADER_FILE_NAME ) ),
   m_FragmentShader( Resource( FRAGMENT_SHADER_FILE_NAME ) )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   m_MVPUni = 0;
   m_MeshTextureObj = 0;
   m_MeshTextureUni = 0;
   m_VAO = 0;

   m_MUni = 0;
   m_LightPosWorldSpaceUni = 0;
   m_LigthDirectionUni = 0;
   m_LightColorUni = 0;
   m_LightPowerUni = 0;
   m_LightNumberUni = 0;
   m_ShadowMapMatrixUni = -1;
   memset( m_ShadowMapTextureUni, -1, sizeof( m_ShadowMapTextureUni ) );
   m_MaterialAmbientUni = 0;
   m_MaterialDiffuseUni = 0;
   m_MaterialSpecularUni = 0;

   m_VerticesIndexCount = 0;

   m_Props.SetEnableShadow( true );
   }

MeshSceneNode::~MeshSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int MeshSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

	glGenVertexArrays( 1, &m_VAO );
	glBindVertexArray( m_VAO );

   m_VertexShader.VOnRestore();
   m_FragmentShader.VOnRestore();

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.VGetShaderObject(), m_FragmentShader.VGetShaderObject() );

   m_VertexShader.VReleaseShader( m_Program );
   m_FragmentShader.VReleaseShader( m_Program );

   OpenGLRenderer::LoadTexture2D( &m_MeshTextureObj, m_Props.GetMaterialPtr()->GetTextureResource() );

   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;
   SetRadius( pMeshExtra->m_Radius );
   OpenGLRenderer::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], &m_Buffers[ Normal_Buffer ], pMeshResHandle );

   // 1st attribute buffer : vertices
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Vertex_Buffer ] );
   glEnableVertexAttribArray( VERTEX_LOCATION );
   glVertexAttribPointer(
      VERTEX_LOCATION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   // 2nd attribute buffer : UVs
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ UV_Buffer ] );
   glEnableVertexAttribArray( UV_LOCATION );
   glVertexAttribPointer(
      UV_LOCATION,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( void* ) 0                          // array buffer offset
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Normal_Buffer ] );
   glEnableVertexAttribArray( NORMAL_LOCATION );
   glVertexAttribPointer(
      NORMAL_LOCATION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   m_MVPUni          = glGetUniformLocation( m_Program, "uMVP" );
   m_MUni      = glGetUniformLocation( m_Program, "uM" );
   m_LightPosWorldSpaceUni = glGetUniformLocation( m_Program, "uLightPosition_WorldSpace" );
   m_LigthDirectionUni     = glGetUniformLocation( m_Program, "uLighDirection_WorldSpace" );
   m_LightColorUni         = glGetUniformLocation( m_Program, "uLightColor" );
   m_LightPowerUni         = glGetUniformLocation( m_Program, "uLightPower" );
   m_LightNumberUni        = glGetUniformLocation( m_Program, "uLightNumber" );

   m_ShadowMapMatrixUni    = glGetUniformLocation( m_Program, "uShadowMapMatrix" );
  
   for( int i = 0; i < MAXIMUM_LIGHTS_SUPPORTED; ++i )
      {
      std::string str( "uShadowMapTexture[" );
      str.push_back( '0' + i );
      str.push_back( ']' );
      m_ShadowMapTextureUni[ i ] = glGetUniformLocation( m_Program, str.c_str() );
      }
      
   m_EyePosWorldSpaceUni   = glGetUniformLocation( m_Program, "uEyePosition_WorldSpace" );
   
   m_MeshTextureUni     = glGetUniformLocation( m_Program, "uMeshTexture" );
   m_MaterialDiffuseUni    = glGetUniformLocation( m_Program, "uMaterialDiffuse" );
   m_MaterialAmbientUni    = glGetUniformLocation( m_Program, "uMaterialAmbient" );
   m_MaterialSpecularUni   = glGetUniformLocation( m_Program, "uMaterialSpecular" );
   // restore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int MeshSceneNode::VRender( Scene *pScene )
   {
	// Use our shader
	glUseProgram( m_Program );
   glBindVertexArray( m_VAO );

   Mat4x4 globalToWorld = VGetGlobalTransformPtr()->GetToWorld();
   // Get the projection & view matrix from the camera class
   Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * VGetGlobalTransformPtr()->GetToWorld();
   // Send our transformation to the currently bound shader, 
   // in the "MVP" uniform
   // 1-> how many matrix, GL_FALSE->should transpose or not
   glUniformMatrix4fv( m_MVPUni, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );

   glUniformMatrix4fv( m_MUni, 1, GL_FALSE, &( globalToWorld[ 0 ][ 0 ] ) );

   auto pLightManager = pScene->GetLightManagerPtr();
   
   glUniform3fv( m_LightPosWorldSpaceUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPosWorldSpace( ) );
   glUniform3fv( m_LigthDirectionUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightDirection( ) );
   glUniform3fv( m_LightColorUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightColor( ) );
   glUniform1fv( m_LightPowerUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPower( ) );
   glUniform1i( m_LightNumberUni, pLightManager->GetActiveLightCount( ) );

   auto shadowMapMatrix = pLightManager->GetShadowMapMatrix();
   glUniformMatrix4fv( m_ShadowMapMatrixUni, MAXIMUM_LIGHTS_SUPPORTED, GL_FALSE, &pLightManager->GetShadowMapMatrix()[ 0 ][ 0 ][ 0 ] );
   auto shadowMapTextureObj = pLightManager->GetShadowMaptexture();
   for( int i = 0; i < MAXIMUM_LIGHTS_SUPPORTED; ++i )
      {
      glActiveTexture( GL_TEXTURE1 + i );
      glBindTexture( GL_TEXTURE_2D, shadowMapTextureObj[ i ] );
      glUniform1i( m_ShadowMapTextureUni[ i ], i + 1 ); // the zero index is for mesh texture
      }

   glUniform3fv( m_EyePosWorldSpaceUni, 1, ( const GLfloat* ) &pScene->GetCamera()->VGetGlobalPosition() );
   
   glUniform4fv( m_MaterialDiffuseUni, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetDiffuse( ) );
   glUniform3fv( m_MaterialAmbientUni, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetAmbient( ) );
   glUniform3fv( m_MaterialSpecularUni, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetSpecular() );

	// Bind our texture in Texture Unit 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_MeshTextureObj );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
   glUniform1i( m_MeshTextureUni, 0 );

   // Draw the triangles !
   glDrawElements(
      GL_TRIANGLES,      // mode
      m_VerticesIndexCount,    // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );

   return S_OK;
   }

ShadowVertexInfo MeshSceneNode::VGetShadowVertexInfo( void ) const 
   { 
   ShadowVertexInfo ret;
   ret.m_Vertexbuffer = m_Buffers[ Vertex_Buffer ];
   ret.m_NormalBuffer = m_Buffers[ Normal_Buffer ];
   ret.m_UVBuffer = m_Buffers[ UV_Buffer ];
   ret.m_IndexBuffer = m_Buffers[ Index_Buffer ];
   ret.m_VertexCount = m_VerticesIndexCount;
   ret.m_TextureObj = m_MeshTextureObj;
   return ret;
   }


void MeshSceneNode::ReleaseResource( void )
   {
   glDeleteVertexArrays( 1, &m_VAO );
   m_VAO = 0;

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_Buffers ), &m_Buffers[ 0 ] );
   ENG_ZERO_MEM( m_Buffers );

   glDeleteTextures( 1, &m_MeshTextureObj );
   m_MeshTextureObj = 0;
   
   glDeleteProgram( m_Program );
   m_Program = 0;
   }