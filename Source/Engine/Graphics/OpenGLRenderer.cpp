////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLRenderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer( void )
   {
   
   }

OpenGLRenderer::~OpenGLRenderer( void )
   {
   
   }

void OpenGLRenderer::VSetBackgroundColor( Color color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }

GLuint OpenGLRenderer::VOnRestore( void )
   {
   
   return GL_NO_ERROR;
   }

void OpenGLRenderer::VShutdown( void )
   {
   }

bool OpenGLRenderer::VPreRender( void )
   {
   glClearDepth( 1.0 );
   // use previously setted clearColr to draw background
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   return true;
   }

bool OpenGLRenderer::VPostRender( void )
   {
   SDL_GL_SwapWindow( g_pApp->GetWindow() );
   return true;
   }
/*
void OpenGLRenderer::VSetWorldTransform( const Mat4x4 *m )
   {
   
   }

void OpenGLRenderer::VSetViewTransform( const Mat4x4 *m )
   {
   
   }

void OpenGLRenderer::VSetProjectionTransform( const Mat4x4 *m )
   {
   
   }*/

GLuint OpenGLRenderer::CompileShader( const GLchar* const* pSrcData, const GLuint shaderID )
   {
   glShaderSource( shaderID, 1, pSrcData, NULL);
	glCompileShader( shaderID );

   GLint result = GL_FALSE;
	int infoLogLength;
	// Check Vertex Shader compliing
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if ( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ infoLogLength + 1];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
	   }
   return result;
   }