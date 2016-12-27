/*!
 * \file RenderManager.cpp
 * \date 2016/12/26 13:12
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
#include "RenderManager.h"

OpenGLRenderManager::OpenGLRenderManager( void )
   {
  // m_pRenderer = ENG_NEW MainRenderer();
   }

int OpenGLRenderManager::VInit( void ) 
   {

   return S_OK;
   }

int OpenGLRenderManager::VOnRestore( void ) 
   {
   m_MainRenderer.VOnRestore();
   return S_OK;
   }

int OpenGLRenderManager::VPreRender( void ) 
   {
   m_MainRenderer.VPreRender();
   return S_OK;
   }

int OpenGLRenderManager::VPostRender( void ) 
   {
   m_MainRenderer.VPostRender();
   return S_OK;
   }

void OpenGLRenderManager::VShutDown( void ) 
   {

   }

void OpenGLRenderManager::CheckError( void ) 
   {
   GLenum errCode;
   while( ( errCode = glGetError() ) != GL_NO_ERROR )
      {
      std::string errorStr;
      switch( errCode )
         {
         case GL_INVALID_ENUM:                  errorStr = "INVALID_ENUM"; break;
         case GL_INVALID_VALUE:                 errorStr = "INVALID_VALUE"; break;
         case GL_INVALID_OPERATION:             errorStr = "INVALID_OPERATION"; break;
         case GL_STACK_OVERFLOW:                errorStr = "STACK_OVERFLOW"; break;
         case GL_STACK_UNDERFLOW:               errorStr = "STACK_UNDERFLOW"; break;
         case GL_OUT_OF_MEMORY:                 errorStr = "OUT_OF_MEMORY"; break;
         case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
         }
      ENG_ERROR( errorStr.c_str() );
      //  std::cout << error << " | " << file << " (" << line << ")" << std::endl;
      }
   }