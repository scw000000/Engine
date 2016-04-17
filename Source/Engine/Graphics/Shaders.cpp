////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "Shaders.h"
#include "..\ResourceCache\ResourceCache.h"
#include "OpenGLRenderer.h"
#include "Scene.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\GameCode4_VS.hlsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\GameCode4_VS.hlsl";

VertexShader::VertexShader( void )
   {
   m_VertexShader = 0;
   }

VertexShader::~VertexShader( void )
   {
   if( m_VertexShader )
      {
      glDeleteShader( m_VertexShader );
      }
   }

GLint VertexShader::OnRestore( Scene* pScene )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );

   if( m_VertexShader && p_OpenGLRenderer->GetProgramID() )
      {
      glDetachShader( p_OpenGLRenderer->GetProgramID(), m_VertexShader );
      }

   if( m_VertexShader )
      {
      glDeleteShader( m_VertexShader );
      }
	
   // Create the shaders
	m_VertexShader = glCreateShader( GL_VERTEX_SHADER );

	Resource resource( VERTEX_SHADER_FILE_NAME );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the shader file from the zip file
	// Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + resource.m_name );

   GLchar* p_VSSourcePointer = ( GLchar* )pResourceHandle->GetBuffer();
   GLint result = p_OpenGLRenderer->CompileShader( &p_VSSourcePointer, m_VertexShader );

   return result;
   }

GLint VertexShader::SetupRender( Scene *pScene, SceneNode *pNode )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );
   if( !p_OpenGLRenderer )
      {
      ENG_ERROR( "shader pointer cast failed" );
      return 0;
      }
   GLuint programID = p_OpenGLRenderer->GetProgramID();
   GLint Result = GL_FALSE;
   int infoLogLength;

   // Set the vertex shader
   glAttachShader( programID, m_VertexShader );
   // Check the program
	glGetProgramiv( programID, GL_LINK_STATUS, &Result );
   glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ infoLogLength + 1];
		glGetProgramInfoLog( programID, infoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      return Result;
      }

	return Result;
   }

FragmentShader::FragmentShader( void )
   {
   m_FragmentShader = 0;
   }

FragmentShader::~FragmentShader( void )
   {
   if( m_FragmentShader )
      {
      glDeleteShader( m_FragmentShader );
      }
   }

GLint FragmentShader::OnRestore( Scene *pScene )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );

   if( m_FragmentShader && p_OpenGLRenderer->GetProgramID() )
      {
      glDetachShader( p_OpenGLRenderer->GetProgramID(), m_FragmentShader );
      }

   if( m_FragmentShader )
      {
      glDeleteShader( m_FragmentShader );
      }
	
   // Create the shaders
	m_FragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   // load resource
	Resource resource( FRAGMENT_SHADER_FILE_NAME );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the shader file from the zip file

	// Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + resource.m_name );

   GLchar* p_VSSourcePointer = ( GLchar* )pResourceHandle->GetBuffer();
   GLint result = p_OpenGLRenderer->CompileShader( &p_VSSourcePointer, m_FragmentShader );

   return result;
   }

GLint FragmentShader::SetupRender( Scene *pScene, SceneNode *pNode )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );
   if( !p_OpenGLRenderer )
      {
      ENG_ERROR( "shader pointer cast failed" );
      return 0;
      }
   GLuint programID = p_OpenGLRenderer->GetProgramID();
   GLint Result = GL_FALSE;
   int infoLogLength;

   // Set the vertex shader
   glAttachShader( programID, m_FragmentShader );
   // Check the program
	glGetProgramiv( programID, GL_LINK_STATUS, &Result );
   glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ infoLogLength + 1];
		glGetProgramInfoLog( programID, infoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
	   }

	return Result;
   }