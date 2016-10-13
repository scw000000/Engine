/*!
 * \file SkyShpereSceneNode.cpp
 * \date 2016/05/03 15:07
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
#include "SkySceneNode.h"
#include "..\ResourceCache\TextureResource.h"
#include "OpenGLRenderer.h"
#include "..\ResourceCache\MeshResource.h"

#define VERTEX_LOCATION    0
#define UV_LOCATION        1

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\TextureVertexShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\TextureFragmentShader.fragmentshader";

SkySceneNode::SkySceneNode( 
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResource, shared_ptr<Resource> ptextureResource, RenderPass renderPass, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderPass, pTransform ),
   m_pMeshResource( pMeshResource ),
   m_pTextureResource( ptextureResource ),
   m_VertexShader( VERTEX_SHADER_FILE_NAME ),
   m_FragmentShader( FRAGMENT_SHADER_FILE_NAME ),
   m_IsActive( true )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   m_MVPMatrix = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArrayObj = 0;
   m_VerticesIndexCount = 0;
   }

SkySceneNode::~SkySceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int SkySceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();
   glGenVertexArrays( 1, &m_VertexArrayObj );
   glBindVertexArray( m_VertexArrayObj );

   m_VertexShader.OnRestore( pScene );
   m_FragmentShader.OnRestore( pScene );

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.GetVertexShader( ), m_FragmentShader.GetFragmentShader( ) );

   m_VertexShader.ReleaseShader( m_Program );
   m_FragmentShader.ReleaseShader( m_Program );


   OpenGLRenderer::LoadTexture2D( &m_Texture, *m_pTextureResource );
   
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;

   OpenGLRenderer::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], NULL, pMeshResHandle );

   // 1rst attribute buffer : vertices
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

   m_MVPMatrix          = glGetUniformLocation( m_Program, "MVP" );
   m_TextureUni         = glGetUniformLocation( m_Program, "myTextureSampler" );

   // restore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int SkySceneNode::VRender( Scene *pScene )
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
  
	// Bind our texture in Texture Unit 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i( m_TextureUni, 0);
  
   // Index buffer
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Buffers[ Index_Buffer ] );

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

void SkySceneNode::ReleaseResource( void )
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
