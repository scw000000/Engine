////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "Shaders.h"
#include "..\ResourceCache\ResourceCache.h"
#include "OpenGLRenderer.h"
#include "Scene.h"

VertexShader::VertexShader( void )
   {
   m_VetrexShader = 0;
   }

VertexShader::~VertexShader( void )
   {
   if( m_VetrexShader )
      {
      glDeleteShader( m_VetrexShader );
      }
   }

GLint VertexShader::OnRestore( Scene* pScene )
   {
   GLuint ProgramID = glCreateProgram();

   shared_ptr<OpenGLRenderer> p_OpenGLRenderer = static_pointer_cast<OpenGLRenderer>( pScene->GetRenderer() );

   if( m_VetrexShader && p_OpenGLRenderer->GetProgramID() )
      {
      glDetachShader( p_OpenGLRenderer->GetProgramID(), m_VetrexShader );
      if( m_VetrexShader )
         {
         glDeleteShader( m_VetrexShader );
         }
      }
	
   // Create the shaders
	m_VetrexShader = glCreateShader( GL_VERTEX_SHADER );

   std::string vertexShaderFileName = "Effects\\GameCode4_VS.hlsl";
	Resource resource( vertexShaderFileName.c_str() );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the shader file from the zip file

	GLint result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + resource.m_name );
   GLchar* p_VSSourcePointer = ( GLchar* )pResourceHandle->GetBuffer();
	glShaderSource( m_VetrexShader, 1, &p_VSSourcePointer, NULL);
	glCompileShader( m_VetrexShader );

	// Check Vertex Shader compliing
	glGetShaderiv( m_VetrexShader, GL_COMPILE_STATUS, &result );
	glGetShaderiv( m_VetrexShader, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if ( InfoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ InfoLogLength + 1];
		glGetShaderInfoLog( m_VetrexShader, InfoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
	   }

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
   GLint Result = GL_FALSE;
   // Set the vertex shader
   glAttachShader(  p_OpenGLRenderer->GetProgramID(), m_VetrexShader );
   // Check the program
	glGetProgramiv( p_OpenGLRenderer->GetProgramID(), GL_LINK_STATUS, &Result );

	return Result;
   }