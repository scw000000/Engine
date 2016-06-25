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

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\BasicVertexShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\BasicFragmentShader.fragmentshader";

MeshSceneNode::MeshSceneNode( 
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResouce, MaterialPtr pMaterial, RenderPass renderPass, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderPass, pTransform, pMaterial ),
   m_pMeshResource( pMeshResouce ), 
  // m_pMaterial(  ),
   m_VertexShader( VERTEX_SHADER_FILE_NAME ),
   m_FragmentShader( FRAGMENT_SHADER_FILE_NAME )
   {
   m_Program = 0;
   m_VerTexBuffer = 0;
   m_UVBuffer = 0;

   m_IndexBuffer = 0;

   m_MVPMatrix = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArray = 0;

   m_NormalBuffer = 0;

   m_ToWorldMatrix = 0;
   m_LightPosWorldSpace = 0;
   m_LigthDirection = 0;
   m_LightColor = 0;
   m_LightPower = 0;
   m_LightAmbient = 0;
   m_LightNumber = 0;
   m_MaterialAmbient = 0;
   m_MaterialDiffuse = 0;

   m_VerticesCount = 0;
   }

MeshSceneNode::~MeshSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int MeshSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

	glGenVertexArrays( 1, &m_VertexArray );
	glBindVertexArray( m_VertexArray );

   m_VertexShader.OnRestore( pScene );
   m_FragmentShader.OnRestore( pScene );

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.GetVertexShader(), m_FragmentShader.GetFragmentShader() );

   m_VertexShader.ReleaseShader( m_Program );
   m_FragmentShader.ReleaseShader( m_Program );

 //  GLint result = GL_FALSE;
 //  if( !m_Program )
 //     {
 //     m_Program = glCreateProgram();
 //     result =  glGetError();
 //     if( result != GL_NO_ERROR )
 //        {
 //        return result;
 //        } 
 //     }

 //  // Link the program
	//glAttachShader( m_Program, m_VertexShader.GetVertexShader() );
	//glAttachShader( m_Program, m_FragmentShader.GetFragmentShader() );
	//glLinkProgram( m_Program );

 //  
 //  int infoLogLength;
	//// Check the program
	//glGetProgramiv( m_Program, GL_LINK_STATUS, &result );
	//glGetProgramiv( m_Program, GL_INFO_LOG_LENGTH, &infoLogLength );


   OpenGLRenderer::LoadTexture( &m_Texture, &m_Props.GetMaterialPtr()->GetTextureResource() );

   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( &*m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   m_VerticesCount = pMeshExtra->m_pScene->mMeshes[ 0 ]->mNumFaces * 3;
   SetRadius( pMeshExtra->m_Radius );
   OpenGLRenderer::LoadMesh( &m_VerTexBuffer, &m_UVBuffer, &m_IndexBuffer, &m_NormalBuffer, pMeshExtra->m_pScene );

   

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


   // resore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int MeshSceneNode::VRender( Scene *pScene )
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

   glEnableVertexAttribArray( 2 );
   glBindBuffer( GL_ARRAY_BUFFER, m_NormalBuffer );
   glVertexAttribPointer(
      2,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );    

   // Index buffer
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer );

   // Draw the triangles !
   glDrawElements(
      GL_TRIANGLES,      // mode
      m_VerticesCount,    // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glDisableVertexAttribArray( 0 );
   glDisableVertexAttribArray( 1 );
   glDisableVertexAttribArray( 2 );

   return S_OK;
   }

void MeshSceneNode::ReleaseResource( void )
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

   if( m_NormalBuffer )
      {
      glDeleteBuffers( 1, &m_NormalBuffer );
      m_NormalBuffer = 0;
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