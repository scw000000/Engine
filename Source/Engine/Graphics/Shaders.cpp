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
#include ".\Renderer\RendererLoader.h"
#include "Scene.h"


OpenGLShader::OpenGLShader( shared_ptr< Resource > pShaderResource ) : m_pShaderResource( pShaderResource )
   {
   m_ShaderObj = 0;
   }

void OpenGLShader::CompileAndSetShader( GLuint shaderType )
   {
   OpenGLRendererLoader::CompileAndLoadShader( m_ShaderObj, m_pShaderResource, shaderType );
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

VertexShader::VertexShader( shared_ptr< Resource > pResource ) : OpenGLShader( pResource )
   {
   }

VertexShader::~VertexShader( void )
   {
   }

void VertexShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_VERTEX_SHADER );
   }

FragmentShader::FragmentShader( shared_ptr< Resource > pResource ) : OpenGLShader( pResource )
   {
   }

FragmentShader::~FragmentShader( void )
   {

   }

void FragmentShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_FRAGMENT_SHADER );
   }

void ComputeShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_COMPUTE_SHADER );
   }
