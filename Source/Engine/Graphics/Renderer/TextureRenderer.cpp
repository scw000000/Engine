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
#include "RenderHelper.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\TextureRenderer.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\TextureRenderer.fs.glsl";

TextureRenderer::TextureRenderer( void ) : 
   m_VertexShader( shared_ptr< Resource >( ENG_NEW Resource( VERTEX_SHADER_FILE_NAME ) ) ),
   m_FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FRAGMENT_SHADER_FILE_NAME ) ) ) 
   {
   m_Program = 0;
   m_TextureUni = 0;
   }


void TextureRenderer::VShutdown( void )
   {
   ReleaseResource();
   }

int TextureRenderer::VOnRestore( Scene* pScene )
   {
   ReleaseResource();

   m_VertexShader.VOnRestore();
   m_FragmentShader.VOnRestore();

   m_Program = OpenGLRendererLoader::GenerateProgram( { m_VertexShader.GetShaderObject(), m_FragmentShader.GetShaderObject() } );

   m_VertexShader.VReleaseShader( m_Program );
   m_FragmentShader.VReleaseShader( m_Program );
   
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
   
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, textureObj );
   
   OpenglRenderHelper::RenderQuad();

   auto screenSize = g_pApp->GetScreenSize();
   glViewport( 0, 0, screenSize.x, screenSize.y );
   }

void TextureRenderer::ReleaseResource( void )
   {
   glDeleteProgram( m_Program );
   m_Program = 0;
   }