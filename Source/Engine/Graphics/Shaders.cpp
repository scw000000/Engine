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


OpenGLShader::OpenGLShader( const Resource& shaderResource ) : m_ShaderResource( shaderResource )
   {
   m_ShaderObj = 0;
   }

void OpenGLShader::CompileAndSetShader( GLuint shaderType )
   {
   OpenGLRendererLoader::CompileAndLoadShader( m_ShaderObj, m_ShaderResource, shaderType );
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

void ComputeShader::VOnRestore( void )
   {
   CompileAndSetShader( GL_COMPUTE_SHADER );
   }
