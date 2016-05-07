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

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\TextureVertexShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\TextureFragmentShader.fragmentshader";

SkySceneNode::SkySceneNode( 
   const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, shared_ptr<Resource> pMeshResource, shared_ptr<Resource> ptextureResource, RenderPass renderPass, TransformPtr pTransform )
   : SceneNode( actorId, renderComponent, renderPass, pTransform ), 
   m_pMeshResource( pMeshResource ),
   m_pTextureResource( ptextureResource ),
   m_VertexShader( VERTEX_SHADER_FILE_NAME ),
   m_FragmentShader( FRAGMENT_SHADER_FILE_NAME ),
   m_IsActive( true )
   {
   m_Program = 0;
   m_VerTexBuffer = 0;
   m_UVBuffer = 0;
   m_IndexBuffer = 0;
   m_MVPMatrix = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArray = 0;
   }

SkySceneNode::~SkySceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int SkySceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();
   glGenVertexArrays( 1, &m_VertexArray );
   glBindVertexArray( m_VertexArray );

   m_VertexShader.OnRestore( pScene );
   m_FragmentShader.OnRestore( pScene );

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.GetVertexShader( ), m_FragmentShader.GetFragmentShader( ) );

   m_VertexShader.ReleaseShader( m_Program );
   m_FragmentShader.ReleaseShader( m_Program );


   OpenGLRenderer::LoadTexture( &m_Texture, *m_pTextureResource );
   
   float radius;

   OpenGLRenderer::LoadMesh( &m_VerTexBuffer, &radius, &m_UVBuffer, &m_IndexBuffer, NULL,*m_pMeshResource );

   m_MVPMatrix          = glGetUniformLocation( m_Program, "MVP" );
   m_TextureUni         = glGetUniformLocation( m_Program, "myTextureSampler" );


   // resore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int SkySceneNode::VRender( Scene *pScene )
   {
	// Use our shader
	glUseProgram( m_Program );
   glBindVertexArray( m_VertexArray );

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

		// 1rst attribute buffer : vertices
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, m_VerTexBuffer );
	glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
   
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray( 1 );
	glBindBuffer( GL_ARRAY_BUFFER, m_UVBuffer );
	glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

   // Force the Mesh to reload to getting vertex number
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( *m_pMeshResource );
      
	//// Draw the triangle !
 //  // Beware of vertex numbers, I may have to use index buffer
	//glDrawArrays( GL_TRIANGLES, 0, pAiScene->mMeshes[0]->mNumVertices );

   // Index buffer
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer );

   // Draw the triangles !
   glDrawElements(
      GL_TRIANGLES,      // mode
      pAiScene->mMeshes[0]->mNumFaces * 3,    // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0           // element array buffer offset
      );

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );

   return S_OK;
   }

void SkySceneNode::ReleaseResource( void )
   {
   if( m_VertexArray )
      {
      glDeleteVertexArrays( 1, &m_VertexArray );
      m_VertexArray = 0;
      }
   if( m_VerTexBuffer )
      {
      glDeleteBuffers( 1, &m_VerTexBuffer );
      m_VerTexBuffer = 0;
      }

   if( m_UVBuffer )
      {
      glDeleteBuffers( 1, &m_UVBuffer );
      m_UVBuffer = 0;
      }

   if( m_IndexBuffer )
      {
      glDeleteBuffers( 1, &m_IndexBuffer );
      m_IndexBuffer = 0;
      }

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
