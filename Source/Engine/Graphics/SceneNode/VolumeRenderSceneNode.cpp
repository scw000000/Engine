x
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
#include "..\Graphics\Renderer\RendererLoader.h"
#include "..\Material.h"

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
                                                                     Vec3( 1, 1, 0 ),
                                                                     Vec3( 1, 0, 0 )
                                                                     };

VolumeRenderSceneNode::VolumeRenderSceneNode( const ActorId actorId,
                                              IRenderComponent* pRenderComponent,
                                              RenderGroup renderGroup,
                                              TransformPtr pTransform,
                                              shared_ptr<Resource> pVolumeTextureResource,
                                              shared_ptr<Resource> pTransferFunctionResource,
                                              const Vec3& textureDiemension,
                                              const Vec3& cuboidDimension )
                                              : SceneNode( actorId, pRenderComponent, renderGroup, pTransform ),
                                              m_FirstPassVertexShader( shared_ptr< Resource >( ENG_NEW Resource( FIRST_PASS_VERTEX_SHADER_FILE_NAME ) ) ),
                                              m_FirstPassFragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FIRST_PASS_FRAGMENT_SHADER_FILE_NAME ) ) ),
                                              m_SecondPassVertexShader( shared_ptr< Resource >( ENG_NEW Resource( SECOND_PASS_VERTEX_SHADER_FILE_NAME ) ) ),
                                              m_SecondPassFragmentShader( shared_ptr< Resource >( ENG_NEW Resource( SECOND_PASS_FRAGMENT_SHADER_FILE_NAME ) ) )
   {
   SetAlpha( 0.1f ); 

   m_pVolumeTextureResource = pVolumeTextureResource;
   m_pTransferFuncionResource = pTransferFunctionResource;

   m_TextureDimension = textureDiemension;

   m_VAO = 0;

   m_FirstPassProgram = 0;

   m_FirstPassMVPUni = -1;

   ENG_ZERO_MEM( m_VBOs );

   m_RenderedTextureObj = 0;
   m_RenderDepthBufferObj = 0;
   m_FrameBufferObj = 0;

   m_SecondPassProgram = 0;
   
   m_VolumeTextureObj = 0;
   m_TranserTextureObj = 0;

   m_ScreenSizeUni = -1;
   m_StepSizeUni = -1;
   m_TransferTextureUni = -1;
   m_RenderedTextureUni = -1;
   m_VolumeTextureUni = -1;
   m_SecondPassMVPUni = -1;
   
   auto halfCuboidDimension = cuboidDimension / 2.0f;
   m_VerticesLocation[ 0 ] = Vec3( -halfCuboidDimension.x, -halfCuboidDimension.y, halfCuboidDimension.z );
   m_VerticesLocation[ 1 ] = Vec3( halfCuboidDimension.x, -halfCuboidDimension.y, halfCuboidDimension.z );
   m_VerticesLocation[ 2 ] = Vec3( halfCuboidDimension.x, halfCuboidDimension.y, halfCuboidDimension.z );
   m_VerticesLocation[ 3 ] = Vec3( -halfCuboidDimension.x, halfCuboidDimension.y, halfCuboidDimension.z );

   // Back face
   m_VerticesLocation[ 4 ] = Vec3( -halfCuboidDimension.x, -halfCuboidDimension.y, -halfCuboidDimension.z );
   m_VerticesLocation[ 5 ] = Vec3( -halfCuboidDimension.x, halfCuboidDimension.y, -halfCuboidDimension.z );
   m_VerticesLocation[ 6 ] = Vec3( halfCuboidDimension.x, halfCuboidDimension.y, -halfCuboidDimension.z );
   m_VerticesLocation[ 7 ] = Vec3( halfCuboidDimension.x, -halfCuboidDimension.y, -halfCuboidDimension.z );
   }

VolumeRenderSceneNode::~VolumeRenderSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int VolumeRenderSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

   m_FirstPassVertexShader.VOnRestore();
   m_FirstPassFragmentShader.VOnRestore();

   m_FirstPassProgram = OpenGLRendererLoader::GenerateProgram( { m_FirstPassVertexShader.GetShaderObject(), m_FirstPassFragmentShader.GetShaderObject() } );

   m_FirstPassVertexShader.VReleaseShader( m_FirstPassProgram );
   m_FirstPassFragmentShader.VReleaseShader( m_FirstPassProgram );

   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

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

   m_FirstPassMVPUni = glGetUniformLocation( m_FirstPassProgram, "uMVP" );

   // ---------------------Set up second pass program---------------------

   m_SecondPassVertexShader.VOnRestore();
   m_SecondPassFragmentShader.VOnRestore();

   m_SecondPassProgram = OpenGLRendererLoader::GenerateProgram( { m_SecondPassVertexShader.GetShaderObject(), m_SecondPassFragmentShader.GetShaderObject() } );
   //m_SecondPassProgram = OpenGLRenderer::GenerateProgram( m_SecondPassVertexShader.VGetShaderObject(), m_SecondPassFragmentShader.VGetShaderObject() );

   m_SecondPassVertexShader.VReleaseShader( m_SecondPassProgram );
   m_SecondPassFragmentShader.VReleaseShader( m_SecondPassProgram );

   // ---------------------Read and load volume texture---------------------
   glGenTextures( 1, &m_VolumeTextureObj );
   // bind 3D texture target
   glBindTexture( GL_TEXTURE_3D, m_VolumeTextureObj );
   glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT );
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
   auto volumeTextureResHandle = g_pApp->m_pResCache->GetHandle( m_pVolumeTextureResource );
   auto pRawBuffer = volumeTextureResHandle->GetBuffer();
   glTexImage3D( GL_TEXTURE_3D, 0, GL_INTENSITY, m_TextureDimension.x, m_TextureDimension.y, m_TextureDimension.z, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pRawBuffer );

   // ---------------------Read and load transfer function texture---------------------
   glGenTextures( 1, &m_TranserTextureObj );
   glBindTexture( GL_TEXTURE_1D, m_TranserTextureObj );
   // Set texture sample function
   glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   // Set texture storage methods
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

   auto transferFunctionTextureResHandle = g_pApp->m_pResCache->GetHandle( m_pTransferFuncionResource );
   pRawBuffer = transferFunctionTextureResHandle->GetBuffer();
   glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pRawBuffer );

   SetUpRenderedTexture();
   SetUpFrameBuffer();
   
   m_ScreenSizeUni = glGetUniformLocation( m_SecondPassProgram, "uScreenSize" );
   m_StepSizeUni = glGetUniformLocation( m_SecondPassProgram, "uStepSize" );
   m_TransferTextureUni = glGetUniformLocation( m_SecondPassProgram, "uTransferFunction" );
   m_RenderedTextureUni = glGetUniformLocation( m_SecondPassProgram, "uExitPoints" );
   m_VolumeTextureUni = glGetUniformLocation( m_SecondPassProgram, "uVolumeTexture" );
   m_SecondPassMVPUni = glGetUniformLocation( m_SecondPassProgram, "uMVP" );

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   // restore all of its children
   SceneNode::VOnRestore( pScene );
   return S_OK;
   }

int VolumeRenderSceneNode::VRender( Scene *pScene )
   {
   auto screenSize = g_pApp->GetScreenSize();
   glEnable( GL_CULL_FACE );
   glCullFace( GL_FRONT );

   glBindVertexArray( m_VAO );
   glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_FrameBufferObj );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glUseProgram( m_FirstPassProgram );
   
   Mat4x4 globalToWorld = VGetGlobalTransformPtr()->GetToWorld();

   // Get the projection & view matrix from the camera class
   Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * globalToWorld;

   glUniformMatrix4fv( m_FirstPassMVPUni, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );
   glDrawElements(
      GL_TRIANGLES,     
      36,
      GL_UNSIGNED_INT,  
      ( void* ) 0       
      );

   glCullFace( GL_BACK );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   glUseProgram( m_SecondPassProgram );

   glUniform2f( m_ScreenSizeUni, ( float ) screenSize.x, ( float ) screenSize.y );

   glUniform1f( m_StepSizeUni, 0.001f );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_1D, m_TranserTextureObj );
   glUniform1i( m_TransferTextureUni, 0 );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, m_RenderedTextureObj );
   glUniform1i( m_RenderedTextureUni, 1 );

   glActiveTexture( GL_TEXTURE2 );
   glBindTexture( GL_TEXTURE_3D, m_VolumeTextureObj );
   glUniform1i( m_VolumeTextureUni, 2 );

   glUniformMatrix4fv( m_SecondPassMVPUni, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );
   glDrawElements(
      GL_TRIANGLES,      // mode
      36,                // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0        // element array buffer offset
      );
   glBindVertexArray( 0 );
   return S_OK;
   }

void VolumeRenderSceneNode::ReleaseResource( void )
   {
  
   glDeleteVertexArrays( 1, &m_VAO );
   m_VAO = 0;


   glDeleteBuffers( ENG_ARRAY_LENGTH( m_VBOs ), &m_VBOs[ 0 ] );
   ENG_ZERO_MEM( m_VBOs );

   glDeleteProgram( m_FirstPassProgram );
   m_FirstPassProgram = 0;

   glDeleteTextures( 1, &m_RenderedTextureObj );
   m_RenderedTextureObj = 0;

   glDeleteFramebuffers( 1, &m_FrameBufferObj );
   m_FrameBufferObj = 0;

   glDeleteRenderbuffers( 1, &m_RenderDepthBufferObj );
   m_RenderDepthBufferObj = 0;

   glDeleteProgram( m_SecondPassProgram );
   m_SecondPassProgram = 0;

   glDeleteTextures( 1, &m_VolumeTextureObj );
   m_VolumeTextureObj = 0;

   glDeleteTextures( 1, &m_TranserTextureObj );
   m_TranserTextureObj = 0;
   }

void VolumeRenderSceneNode::SetUpRenderedTexture( void )
   {
   glGenTextures( 1, &m_RenderedTextureObj );
   ENG_ASSERT( m_RenderedTextureObj );

   glBindTexture( GL_TEXTURE_2D, m_RenderedTextureObj );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   auto screenSize = g_pApp->GetScreenSize();
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   }

void VolumeRenderSceneNode::SetUpFrameBuffer( void )
   {
   // create a depth buffer for our framebuffer
   glGenRenderbuffers( 1, &m_RenderDepthBufferObj );
   ENG_ASSERT( m_RenderDepthBufferObj );
   glBindRenderbuffer( GL_RENDERBUFFER, m_RenderDepthBufferObj );
   
   auto screenSize = g_pApp->GetScreenSize();
   glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y );

   // attach the texture and the depth buffer to the framebuffer
   glGenFramebuffers( 1, &m_FrameBufferObj );
   ENG_ASSERT( m_FrameBufferObj );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObj );
   // Set up rendering target
   glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderedTextureObj, 0 );
   glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderDepthBufferObj );
   
   ENG_ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE && "Frame buffer error" );
   }

void VolumeRenderSceneNode::SetUpNormalVolume( std::vector< Vec3 >& output, const char* pRawTexture )
   {
   auto getIntensity = [ &] ( int X, int Y, int Z ) -> char 
      {
      return pRawTexture[ X * ( int ) ( m_TextureDimension.y * m_TextureDimension.z ) + Y * ( int ) ( m_TextureDimension.z ) + Z ];
      };
   for( int i = 0; i < m_TextureDimension.x; ++i )
      {
      for( int j = 0; j < m_TextureDimension.y; ++j )
         {
         for( int k = 0; k < m_TextureDimension.z; ++k )
            {
            int prevX = ( i > 0 ) ? i - 1: i;
            int nextX = ( i < m_TextureDimension.x - 1 ) ? i + 1: i;

            int prevY = ( j > 0 ) ? j - 1 : j;
            int nextY = ( j < m_TextureDimension.y - 1 ) ? j + 1 : j;

            int prevZ = ( k > 0 ) ? k - 1 : k;
            int nextZ = ( k < m_TextureDimension.z - 1 ) ? k + 1 : k;

            output.push_back( Vec3( ( getIntensity( nextX, j, k ) - getIntensity( prevX, j, k ) ) / 2.0f,
               ( getIntensity( i, nextY, k ) - getIntensity( i, prevY, k ) ) / 2.0f,
               ( getIntensity( i, j, nextZ ) - getIntensity( i, j, prevZ ) ) / 2.0f
               ) );
            }
         }
      }
   }