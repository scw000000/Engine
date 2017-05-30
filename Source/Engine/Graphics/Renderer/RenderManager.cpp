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

// Called in EngineApp::InitInstance
int OpenGLRenderManager::VInit( void ) 
   {

   return S_OK;
   }

int OpenGLRenderManager::VOnRestore( Scene* pScene )
   {
   m_MainRenderer.VOnRestore( pScene );
   m_TextureDrawer.VOnRestore( pScene );
   return S_OK;
   }

int OpenGLRenderManager::VPreRender( void ) 
   {
   m_MainRenderer.VPreRender();
   m_TextureDrawer.VPreRender();
   return S_OK;
   }

int OpenGLRenderManager::VLightingPass( Scene* pScene )
   {
   auto screensize = g_pApp->GetScreenSize();
   float xSize = 300.f;
   float ySize = xSize * ( float ) screensize.y / ( float ) screensize.x;
   m_TextureDrawer.DrawTexture( m_MainRenderer.m_SST[ DeferredMainRenderer::SST_TileDebugging ], Point( 0, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) ); // for testing
   m_TextureDrawer.DrawTexture( m_MainRenderer.m_SST[ DeferredMainRenderer::SST_AlbedoMetalness ], Point( 300, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) );
   m_TextureDrawer.DrawTexture( m_MainRenderer.m_SST[ DeferredMainRenderer::SST_NormalGlossiness ], Point( 600, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) );

   m_MainRenderer.LightCulling( pScene );
   m_MainRenderer.CalculateLighting();
   return S_OK;
   }

int OpenGLRenderManager::VSSAOPass( void )
   {
   return S_OK;
   }

int OpenGLRenderManager::VMotionBlurPass( void )
   {
   return S_OK;
   }

int OpenGLRenderManager::VBloomPass( void )
   {
   return S_OK;
   }

int OpenGLRenderManager::VToneMappingPass( void )
   {
   return S_OK;
   }

int OpenGLRenderManager::VPostRender( void ) 
   {
   m_TextureDrawer.VPostRender();

   m_MainRenderer.VPostRender();
   return S_OK;
   }

void OpenGLRenderManager::VShutDown( void ) 
   {
   m_TextureDrawer.VShutdown();
   m_MainRenderer.VShutdown();
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