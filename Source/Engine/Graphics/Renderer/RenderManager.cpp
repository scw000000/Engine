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
#include "RenderHelper.h"


OpenGLRenderManager::OpenGLRenderManager( void )
   {
  // m_pRenderer = ENG_NEW MainRenderer();
   }

// Called in EngineApp::InitInstance
int OpenGLRenderManager::VInit( void ) 
   {
   OnRestoreTextures();
   OpenglRenderHelper::OnRestore();

   return S_OK;
   }

int OpenGLRenderManager::VOnRestore( Scene* pScene )
   {
   
   m_MainRenderer.OnRestoreTextures( m_SST[ SST_Depth ]
                                     , m_SST[ SST_MRT0 ]
                                     , m_SST[ SST_MRT1 ] 
                                     , m_SST[ SST_MRT2 ]
#ifdef _DEBUG
                                     , m_SST[ SST_TileDebugging ]
#endif // _DEBUG                        
                                     , m_SST[ SST_Lighting ]
                                     );
   m_MainRenderer.VOnRestore( pScene );
   m_SSAORenderer.OnRestoreTextures( m_SST[ SST_Depth ], m_SST[ SST_MRT0 ], m_SST[ SST_SSAO ], m_SST[ SST_SSAOBlur ] );
   m_SSAORenderer.VOnRestore( pScene );
   m_MotionBlurRenderer.OnRestoreTextures( m_SST[ SST_MRT2 ], m_SST[ SST_Lighting ], m_SST[ SST_MotionBlur ] );
   m_MotionBlurRenderer.VOnRestore( pScene );
   m_ToneMappingRenderer.OnRestoreTextures( m_SST[ SST_MotionBlur ] );
   m_ToneMappingRenderer.VOnRestore( pScene );

   m_TextureDrawer.VOnRestore( pScene );
   return S_OK;
   }

int OpenGLRenderManager::VPreRender( void ) 
   {
   m_MainRenderer.VPreRender();
   m_SSAORenderer.VPreRender();
   m_ToneMappingRenderer.VPreRender();
   m_TextureDrawer.VPreRender();
   return S_OK;
   }

int OpenGLRenderManager::VLightingPass( Scene* pScene )
   {
   auto screensize = g_pApp->GetScreenSize();
   float xSize = 300.f;
   float ySize = xSize * ( float ) screensize.y / ( float ) screensize.x;
#ifdef _DEBUG
   // m_TextureDrawer.DrawTexture( m_SST[ SST_TileDebugging ], Point( 0, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) );
#endif // DEBUG
   m_TextureDrawer.DrawTexture( m_SST[ SST_MRT2 ], Point( 0, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ), false );
   m_TextureDrawer.DrawTexture( m_SST[ SST_SSAOBlur ], Point( 300, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ), true );
   m_TextureDrawer.DrawTexture( m_SST[ SST_MRT1 ], Point( 600, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) );

   m_MainRenderer.LightCulling( pScene );
   m_MainRenderer.CalculateLighting();
   return S_OK;
   }

int OpenGLRenderManager::VSSAOPass( void )
   {
   m_SSAORenderer.OnRender();
   return S_OK;
   }

int OpenGLRenderManager::VMotionBlurPass( void )
   {
   m_MotionBlurRenderer.OnRender();
   return S_OK;
   }

int OpenGLRenderManager::VBloomPass( void )
   {
   return S_OK;
   }

int OpenGLRenderManager::VToneMappingPass( void )
   {
   m_ToneMappingRenderer.OnRender();
   return S_OK;
   }

int OpenGLRenderManager::VPostRender( void ) 
   {
   m_TextureDrawer.VPostRender();

   m_MainRenderer.VPostRender();
   m_SSAORenderer.VPostRender();
   return S_OK;
   }

void OpenGLRenderManager::VShutDown( void ) 
   {
   m_TextureDrawer.VShutdown();
   m_MainRenderer.VShutdown();
   m_SSAORenderer.VShutdown();
   m_ToneMappingRenderer.VShutdown();
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

int OpenGLRenderManager::OnRestoreTextures( void )
   {
   auto screenSize = g_pApp->GetScreenSize();

   // Create all textures at once
   glGenTextures( SST_Num, m_SST );
   for( int i = 0; i < SST_Num; ++i )
      {
      ENG_ASSERT( m_SST[ i ] );
      }
   // Depth texture
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Depth ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   // For depth test
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   // MRT 0
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_MRT0 ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   // MRT 1
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_MRT1 ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   // MRT 2
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_MRT2 ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RG16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_SSAO ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_SSAOBlur ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_Lighting ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_MotionBlur ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

#ifdef _DEBUG
   // Tile debugging
   glBindTexture( GL_TEXTURE_2D, m_SST[ SST_TileDebugging ] );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16, screenSize.x, screenSize.y, 0, GL_RGB, GL_FLOAT, NULL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#endif // _DEBUG

   glBindTexture( GL_TEXTURE_2D, 0 );
   // glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );
   OpenGLRenderManager::CheckError();

   return S_OK;
   }