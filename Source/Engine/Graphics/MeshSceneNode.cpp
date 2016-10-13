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
#include "OpenGLRenderer.h"

#define VERTEX_LOCATION    0
#define UV_LOCATION        1
#define NORMAL_LOCATION    2

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\BasicVertexShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\BasicFragmentShader.fragmentshader";

MeshSceneNode::MeshSceneNode( 
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResouce, MaterialPtr pMaterial, RenderPass renderPass, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderPass, pTransform, pMaterial ),
   m_pMeshResource( pMeshResouce ), 
  // m_pMaterial(  ),
   m_VertexShader( Resource( VERTEX_SHADER_FILE_NAME ) ),
   m_FragmentShader( Resource( FRAGMENT_SHADER_FILE_NAME ) )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   m_MVPMatrix = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArrayObj = 0;

   m_ToWorldMatrix = 0;
   m_LightPosWorldSpace = 0;
   m_LigthDirection = 0;
   m_LightColor = 0;
   m_LightPower = 0;
   m_LightAmbient = 0;
   m_LightNumber = 0;
   m_MaterialAmbient = 0;
   m_MaterialDiffuse = 0;

   m_VerticesIndexCount = 0;
   }

MeshSceneNode::~MeshSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int MeshSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

	glGenVertexArrays( 1, &m_VertexArrayObj );
	glBindVertexArray( m_VertexArrayObj );

   m_VertexShader.VOnRestore();
   m_FragmentShader.VOnRestore();

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.VGetShaderObject(), m_FragmentShader.VGetShaderObject() );

   m_VertexShader.VReleaseShader( m_Program );
   m_FragmentShader.VReleaseShader( m_Program );

   OpenGLRenderer::LoadTexture2D( &m_Texture, m_Props.GetMaterialPtr()->GetTextureResource() );

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

   m_MVPMatrix          = glGetUniformLocation( m_Program, "MVP" );
   m_TextureUni         = glGetUniformLocation( m_Program, "myTextureSampler" );
  
   m_ToWorldMatrix      = glGetUniformLocation( m_Program, "M" );
   m_LightPosWorldSpace = glGetUniformLocation( m_Program, "LightPosition_WorldSpace" );
   m_LigthDirection     = glGetUniformLocation( m_Program, "LighDirection" );
   m_LightColor         = glGetUniformLocation( m_Program, "LightColor" );
   m_LightPower         = glGetUniformLocation( m_Program, "LightPower" );
   m_LightAmbient       = glGetUniformLocation( m_Program, "LightAmbient" );
   m_LightNumber        = glGetUniformLocation( m_Program, "LightNumber" );

   m_EyeDirWorldSpace   = glGetUniformLocation( m_Program, "EyeDirection_WorldSpace" );
   
   m_MaterialDiffuse    = glGetUniformLocation( m_Program, "MaterialDiffuse" );
   m_MaterialAmbient    = glGetUniformLocation( m_Program, "MaterialAmbient" );

   // restore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int MeshSceneNode::VRender( Scene *pScene )
   {
	// Use our shader
	glUseProgram( m_Program );
   glBindVertexArray( m_VertexArrayObj );

   // Get the projection & view matrix from the camera class
	Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetWorldViewProjection( pScene );
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
   // 1-> how many matrix, GL_FALSE->should transpose or not
	glUniformMatrix4fv( m_MVPMatrix, 1, GL_FALSE, &mWorldViewProjection[0][0]);

   glUniformMatrix4fv( m_ToWorldMatrix, 1, GL_FALSE, &pScene->GetTopMatrix()[0][0] );

   auto pLightManager = pScene->GetLightManagerPtr();
   
   glUniform3fv( m_LightPosWorldSpace, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPosWorldSpace( ) );
   glUniform3fv( m_LigthDirection, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightDirection( ) );
   glUniform3fv( m_LightColor, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightColor( ) );
   glUniform1fv( m_LightPower, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPower( ) );
   glUniform3fv( m_LightAmbient, 1, ( const GLfloat* ) pLightManager->GetLightAmbient( ) );
   glUniform1i( m_LightNumber, pLightManager->GetActiveLightCount( ) );


   glUniform3fv( m_EyeDirWorldSpace, 1, ( const GLfloat* ) &pScene->GetCamera()->GetForward() );
   
   glUniform4fv( m_MaterialDiffuse, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetDiffuse( ) );
   glUniform3fv( m_MaterialAmbient, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetAmbient( ) );

	// Bind our texture in Texture Unit 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
   glUniform1i( m_TextureUni, 0);

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

void MeshSceneNode::ReleaseResource( void )
   {
   if( m_VertexArrayObj )
      {
      glDeleteVertexArrays( 1, &m_VertexArrayObj );
      m_VertexArrayObj = 0;
      }

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_Buffers ), &m_Buffers[ 0 ] );
   ENG_ZERO_MEM( m_Buffers );

   if( m_TextureUni )
      {
      glDeleteTextures( 1, &m_Texture );
      m_Texture = 0;
      }

   if( m_Program )
      {
      glDeleteProgram( m_Program );
      m_Program = 0;
      }
   
   }