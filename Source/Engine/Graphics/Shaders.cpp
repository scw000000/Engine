/*!
 * \file Shaders.cpp
 * \date 2016/10/13 18:12
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
#include "Shaders.h"
#include "..\ResourceCache\ResourceCache.h"
#include "OpenGLRenderer.h"
#include "Scene.h"


OpenGLShader::OpenGLShader( const Resource& shaderResource ) : m_ShaderResource( shaderResource )
   {
   m_ShaderObj = 0;
   }

void OpenGLShader::CompileAndSetShader( GLuint shaderType )
   {
   // Create the shaders
   m_ShaderObj = glCreateShader( shaderType );

   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( m_ShaderResource );  // this actually loads the shader file from the zip file

   if( !pResourceHandle )
      {
      ENG_ERROR( "Invalid shader file path" );
      }
   // Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + m_ShaderResource.m_Name );

   GLchar* p_VSSourcePointer = ( GLchar* ) pResourceHandle->GetBuffer();

   glShaderSource( m_ShaderObj, 1, &p_VSSourcePointer, NULL );
   glCompileShader( m_ShaderObj );

   GLint result = GL_FALSE;

   // Check Vertex Shader compiling
   glGetShaderiv( m_ShaderObj, GL_COMPILE_STATUS, &result );
   if( result == GL_FALSE )
      {
      int infoLogLength;
      glGetShaderiv( m_ShaderObj, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* p_ErrMsg = ENG_NEW GLchar[ infoLogLength + 1 ];
      glGetShaderInfoLog( m_ShaderObj, infoLogLength, NULL, p_ErrMsg );
      ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      glDeleteShader( m_ShaderObj ); // Don't leak the shader.
      }
   }

void OpenGLShader::VReleaseShader( GLuint program )
   {
   if( program && m_ShaderObj )
      {
      glDetachShader( program, m_ShaderObj );
      glDeleteShader( m_ShaderObj );
      }
   m_ShaderObj = 0;
   }

VertexShader::VertexShader( const Resource& shaderResource ) : OpenGLShader( shaderResource )
   {
   }

VertexShader::~VertexShader( void )
   {
   }

void VertexShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_VERTEX_SHADER );
   }

FragmentShader::FragmentShader( const Resource& shaderResource ) : OpenGLShader( shaderResource )
   {
   }

FragmentShader::~FragmentShader( void )
   {

   }

void FragmentShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_FRAGMENT_SHADER );
   }