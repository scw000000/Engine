////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "Shaders.h"
#include "..\ResourceCache\ResourceCache.h"
#include "OpenGLRenderer.h"
#include "Scene.h"


VertexShader::VertexShader( const char* pResourceFile ) : m_ShaderResource( pResourceFile )
   {
   m_VertexShader = 0;
   }

VertexShader::~VertexShader( void )
   {
   }

GLint VertexShader::OnRestore( Scene* pScene )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );
	
   // Create the shaders
	m_VertexShader = glCreateShader( GL_VERTEX_SHADER );


   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &m_ShaderResource );  // this actually loads the shader file from the zip file

   if( !pResourceHandle )
      {
      ENG_ERROR( "Invalid shader file path" );
      }
	// Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + m_ShaderResource.m_name );

   GLchar* p_VSSourcePointer = ( GLchar* )pResourceHandle->GetBuffer();
   GLint result = p_OpenGLRenderer->CompileShader( &p_VSSourcePointer, m_VertexShader );

   return result;
   }

// This function is called in sceneNode::VRender
GLint VertexShader::SetupRender( Scene *pScene, SceneNode *pNode )
   {
   

	return GL_NO_ERROR;
   }

void VertexShader::ReleaseShader( GLuint program )
   {
   if( program && m_VertexShader )
      {
      glDetachShader( program, m_VertexShader );
	
	   glDeleteShader( m_VertexShader );
      }
   m_VertexShader = 0;
   }

FragmentShader::FragmentShader( const char* pResourceFile ) : m_ShaderResource( pResourceFile )
   {
   m_FragmentShader = 0;
   }

FragmentShader::~FragmentShader( void )
   {

   }

GLint FragmentShader::OnRestore( Scene *pScene )
   {
   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );
	
   // Create the shaders
	m_FragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   // load resource

   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &m_ShaderResource );  // this actually loads the shader file from the zip file
   if( !pResourceHandle )
      {
      ENG_ERROR( "Invalid shader file path" );
      }

	// Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + m_ShaderResource.m_name );

   GLchar* p_VSSourcePointer = ( GLchar* )pResourceHandle->GetBuffer();
   GLint result = p_OpenGLRenderer->CompileShader( &p_VSSourcePointer, m_FragmentShader );

   return result;
   }

GLint FragmentShader::SetupRender( Scene *pScene, SceneNode *pNode )
   {
   return GL_NO_ERROR;
   }

void FragmentShader::ReleaseShader( GLuint program )
   {
   if( program && m_FragmentShader )
      {
      glDetachShader( program, m_FragmentShader );
	
	   glDeleteShader( m_FragmentShader );
      }
   m_FragmentShader = 0;
   }