/*!
 * \file VolumeRenderSceneNode.cpp
 * \date 2016/10/13 12:22
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
#include "VolumeRenderSceneNode.h"

const char* const FIRST_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\VolumeRenderingFirstPass.vertexshader";
const char* const FIRST_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\VolumeRenderingFirstPass.fragmentshader";

const char* const SECOND_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\VolumeRenderingSecondPass.vertexshader";
const char* const SECOND_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\VolumeRenderingSecondPass.fragmentshader";

#define VERTEX_POSITION 0
#define BACKFACE_POSITION 1

const unsigned int VolumeRenderSceneNode::s_VerticesIndex[] = {   0, 1, 2, 0, 2, 3,    // Front face
                                                         4, 5, 6, 4, 6, 7,    // Back face
                                                         5, 3, 2, 5, 2, 6,  // Top face
                                                         4, 7, 1, 4, 1, 0, // Bottom face
                                                         7, 6, 2, 7, 2, 1, // Right face
                                                         4, 0, 3, 4, 3, 5  // Left face 
                                                      };

const Vec3 VolumeRenderSceneNode::s_BackFacePosition[] = {// Front face 
                                                                     Vec3( 0, 0, 1 ),
                                                                     Vec3( 1, 0, 1 ),
                                                                     Vec3( 1, 1, 1 ),
                                                                     Vec3( 0, 1, 1 ),

                                                                     // Back face
                                                                     Vec3( 0, 0, 0 ),
                                                                     Vec3( 0, 1, 0 ),
                                                                     Vec3( 1, 0, 0 ),
                                                                     Vec3( 1, 0, 0 )
                                                                     };

VolumeRenderSceneNode::VolumeRenderSceneNode( const ActorId actorId,
                                              IRenderComponent* pRenderComponent,
                                              shared_ptr<Resource> pVolumeTextureResource,
                                              const Vec3& dimension,
                                              shared_ptr<Resource> pTransferFunctionResource,
                                              RenderPass renderPass,
                                              TransformPtr pTransform )
                                              : SceneNode( actorId, pRenderComponent, renderPass, pTransform ),
                                              m_FirstPassVertexShader( FIRST_PASS_VERTEX_SHADER_FILE_NAME ),
                                              m_FirstPassFragmentShader( FIRST_PASS_FRAGMENT_SHADER_FILE_NAME ),
                                              m_SecondPassVertexShader( SECOND_PASS_VERTEX_SHADER_FILE_NAME ),
                                              m_SecondPassFragmentShader( SECOND_PASS_FRAGMENT_SHADER_FILE_NAME )
   {
   m_FirstPassProgram = 0;
   m_SecondPassProgram = 0;
   ENG_ZERO_MEM( m_VBOs );

   m_MVPMatrix = -1;
   m_Texture = 0;
   m_TextureUni = 0;
   m_FirstPassVAO = 0;
   m_SecondPassVAO = 0;

   auto halfDimension = dimension / 2.0f;
   m_VerticesLocation[ 0 ] = Vec3( -halfDimension.x, -halfDimension.y, halfDimension.z );
   m_VerticesLocation[ 1 ] = Vec3( halfDimension.x, -halfDimension.y, halfDimension.z );
   m_VerticesLocation[ 2 ] = Vec3( halfDimension.x, halfDimension.y, halfDimension.z );
   m_VerticesLocation[ 3 ] = Vec3( -halfDimension.x, halfDimension.y, halfDimension.z );

   // Back face
   m_VerticesLocation[ 4 ] = Vec3( -halfDimension.x, -halfDimension.y, -halfDimension.z );
   m_VerticesLocation[ 5 ] = Vec3( -halfDimension.x, halfDimension.y, -halfDimension.z );
   m_VerticesLocation[ 6 ] = Vec3( halfDimension.x, halfDimension.y, -halfDimension.z );
   m_VerticesLocation[ 7 ] = Vec3( halfDimension.x, -halfDimension.y, -halfDimension.z );
   }

VolumeRenderSceneNode::~VolumeRenderSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int VolumeRenderSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

   glGenVertexArrays( 1, &m_FirstPassVAO );
   glBindVertexArray( m_FirstPassVAO );

   m_FirstPassVertexShader.OnRestore( pScene );
   m_FirstPassFragmentShader.OnRestore( pScene );

   m_FirstPassProgram = OpenGLRenderer::GenerateProgram( m_FirstPassVertexShader.GetVertexShader(), m_FirstPassFragmentShader.GetFragmentShader() );

   m_FirstPassVertexShader.ReleaseShader( m_FirstPassProgram );
   m_FirstPassFragmentShader.ReleaseShader( m_FirstPassProgram );


   //OpenGLRenderer::LoadTexture2D( &m_Texture, m_Props.GetMaterialPtr()->GetTextureResource() );

   //shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   //shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   //m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;
   //SetRadius( pMeshExtra->m_Radius );
   //OpenGLRenderer::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], &m_Buffers[ Normal_Buffer ], pMeshResHandle );
   // 1st attribute buffer : vertices
   glGenBuffers( 1, &m_VBOs[ Vertex_Position ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ Vertex_Position ] );
   glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( Vec3 ), m_VerticesLocation, GL_STATIC_DRAW );
   glEnableVertexAttribArray( VERTEX_POSITION );
   glVertexAttribPointer(
      VERTEX_POSITION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   // 2nd attribute buffer : 3D sampling poisition
   glGenBuffers( 1, &m_VBOs[ BackFace_Position ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ BackFace_Position ] );
   glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( Vec3 ), s_BackFacePosition, GL_STATIC_DRAW );
   glEnableVertexAttribArray( BACKFACE_POSITION );
   glVertexAttribPointer(
      BACKFACE_POSITION,                                // attribute
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( void* ) 0                          // array buffer offset
      );

   glGenBuffers( 1, &m_VBOs[ Vertex_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBOs[ Vertex_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof( unsigned int ), s_VerticesIndex, GL_STATIC_DRAW );

   m_MVPMatrix = glGetUniformLocation( m_FirstPassProgram, "MVP" );

   /*m_TextureUni = glGetUniformLocation( m_Program, "myTextureSampler" );

   m_ToWorldMatrix = glGetUniformLocation( m_Program, "M" );
   m_LightPosWorldSpace = glGetUniformLocation( m_Program, "LightPosition_WorldSpace" );
   m_LigthDirection = glGetUniformLocation( m_Program, "LighDirection" );
   m_LightColor = glGetUniformLocation( m_Program, "LightColor" );
   m_LightPower = glGetUniformLocation( m_Program, "LightPower" );
   m_LightAmbient = glGetUniformLocation( m_Program, "LightAmbient" );
   m_LightNumber = glGetUniformLocation( m_Program, "LightNumber" );

   m_EyeDirWorldSpace = glGetUniformLocation( m_Program, "EyeDirection_WorldSpace" );

   m_MaterialDiffuse = glGetUniformLocation( m_Program, "MaterialDiffuse" );
   m_MaterialAmbient = glGetUniformLocation( m_Program, "MaterialAmbient" );*/

   // restore all of its children
   SceneNode::VOnRestore( pScene );

   return S_OK;
   }

int VolumeRenderSceneNode::VRender( Scene *pScene )
   {
   glCullFace( GL_FRONT );
   // Use our shader
   glUseProgram( m_FirstPassProgram );
   glBindVertexArray( m_FirstPassVAO );

   // Get the projection & view matrix from the camera class
   Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetWorldViewProjection( pScene );
   // Send our transformation to the currently bound shader, 
   // in the "MVP" uniform
   // 1-> how many matrix, GL_FALSE->should transpose or not
   glUniformMatrix4fv( m_MVPMatrix, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );
/*
   glUniformMatrix4fv( m_ToWorldMatrix, 1, GL_FALSE, &pScene->GetTopMatrix()[ 0 ][ 0 ] );

   auto pLightManager = pScene->GetLightManagerPtr();

   glUniform3fv( m_LightPosWorldSpace, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPosWorldSpace() );
   glUniform3fv( m_LigthDirection, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightDirection() );
   glUniform3fv( m_LightColor, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightColor() );
   glUniform1fv( m_LightPower, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPower() );
   glUniform3fv( m_LightAmbient, 1, ( const GLfloat* ) pLightManager->GetLightAmbient() );
   glUniform1i( m_LightNumber, pLightManager->GetActiveLightCount() );


   glUniform3fv( m_EyeDirWorldSpace, 1, ( const GLfloat* ) &pScene->GetCamera()->GetForward() );

   glUniform4fv( m_MaterialDiffuse, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetDiffuse() );
   glUniform3fv( m_MaterialAmbient, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetAmbient() );*/

   // Bind our texture in Texture Unit 0
   /*glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_Texture );*/
   // Set our "myTextureSampler" sampler to user Texture Unit 0
   /*glUniform1i( m_TextureUni, 0 );*/

   glDrawElements(
      GL_TRIANGLES,      // mode
      36,    // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );
   glCullFace( GL_BACK );
   return S_OK;
   }

void VolumeRenderSceneNode::ReleaseResource( void )
   {
   if( m_FirstPassVAO )
      {
      glDeleteVertexArrays( 1, &m_FirstPassVAO );
      m_FirstPassVAO = 0;
      }

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_VBOs ), &m_VBOs[ 0 ] );
   ENG_ZERO_MEM( m_VBOs );

   if( m_TextureUni )
      {
      glDeleteTextures( 1, &m_Texture );
      m_Texture = 0;
      }

   if( m_FirstPassProgram )
      {
      glDeleteProgram( m_FirstPassProgram );
      m_FirstPassProgram = 0;
      }

   }