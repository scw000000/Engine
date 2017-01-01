/*!
 * \file TexureRenderer.cpp
 * \date 2016/10/29 20:09
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
#include "TextureRenderer.h"
#include "RendererLoader.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\TextureRenderer.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\TextureRenderer.fs.glsl";

#define VERTEX_LOCATION    0
#define UV_LOCATION        1

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

TextureRenderer::TextureRenderer( void ) : 
   m_VertexShader( Resource( VERTEX_SHADER_FILE_NAME ) ),
   m_FragmentShader( Resource( FRAGMENT_SHADER_FILE_NAME ) )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );
   m_VAO = 0;
   m_TextureUni = 0;
   }


void TextureRenderer::VShutdown( void )
   {
   ReleaseResource();
   }

int TextureRenderer::VOnRestore( Scene* pScene )
   {
   ReleaseResource();

   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

   m_VertexShader.VOnRestore();
   m_FragmentShader.VOnRestore();

   m_Program = OpenGLRendererLoader::GenerateProgram( { m_VertexShader.GetShaderObject(), m_FragmentShader.GetShaderObject() } );

   m_VertexShader.VReleaseShader( m_Program );
   m_FragmentShader.VReleaseShader( m_Program );

   // 1st attribute buffer : vertices
   glGenBuffers( 1, &m_Buffers[ VBOs_Vertex ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ VBOs_Vertex ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_POSITION ), QUAD_VERTEX_POSITION, GL_STATIC_DRAW );
   glEnableVertexAttribArray( VERTEX_LOCATION );
   glVertexAttribPointer(
      VERTEX_LOCATION,                  // attribute
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
   glEnableVertexAttribArray( UV_LOCATION );
   glVertexAttribPointer(
      UV_LOCATION,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( void* ) 0                          // array buffer offset
      );

   glGenBuffers( 1, &m_Buffers[ VBOs_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Buffers[ VBOs_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_INDEX ), QUAD_VERTEX_INDEX, GL_STATIC_DRAW );

   glUseProgram( m_Program );
   m_TextureUni = glGetUniformLocation( m_Program, "uTexture" );
   glUniform1i( m_TextureUni, 0 );
   glUseProgram( 0 );

   return S_OK;
   }

int TextureRenderer::VPreRender( void )
   {
   return S_OK;
   }
int TextureRenderer::VPostRender( void )
   {
   return S_OK;
   }

void TextureRenderer::DrawTexture( GLuint textureObj, const Point& offset, const Point& dimension )
   {
   glViewport( offset.x, offset.y, dimension.x, dimension.y );

   glUseProgram( m_Program );
   glBindVertexArray( m_VAO );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, textureObj );
   
   glDrawElements(
      GL_TRIANGLES,      // mode
      ENG_ARRAY_SIZE_IN_ELEMENTS( QUAD_VERTEX_INDEX ),    // count
      GL_UNSIGNED_SHORT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );
   auto screenSize = g_pApp->GetScreenSize();
   glViewport( 0, 0, screenSize.x, screenSize.y );
   }

void TextureRenderer::ReleaseResource( void )
   {
   glDeleteVertexArrays( 1, &m_VAO );
   m_VAO = 0;

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_Buffers ), &m_Buffers[ 0 ] );
   ENG_ZERO_MEM( m_Buffers );

   glDeleteProgram( m_Program );
   m_Program = 0;
   }