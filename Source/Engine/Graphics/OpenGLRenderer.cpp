////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLRenderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer( void )
   {
   m_Program = 0;
   m_MVP = 0;
   }

OpenGLRenderer::~OpenGLRenderer( void )
   {
   m_Program = 0;
   m_MVP = 0;
   }

void OpenGLRenderer::VSetBackgroundColor( Color color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }

GLuint OpenGLRenderer::VOnRestore( void )
   {
   m_Program = glCreateProgram();
   GLuint result =  glGetError();
   if( result != GL_NO_ERROR )
      {
      return result;
      }

   m_MVP = glGetUniformLocation( m_Program, "MVP");

   return result;
   }

void OpenGLRenderer::VShutdown( void )
   {
   glDeleteProgram( m_Program );
   }

bool OpenGLRenderer::VPreRender( void )
   {
   glUseProgram( m_Program );
   return true;
   }

bool OpenGLRenderer::VPostRender( void )
   {
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