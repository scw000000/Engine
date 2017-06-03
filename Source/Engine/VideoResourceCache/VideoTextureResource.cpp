/*!
 * \file VIdeoTextureResource.cpp
 * \date 2017/01/02 10:42
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
#include "VideoTextureResource.h"
#include "..\ResourceCache\TextureResource.h"

//const char* TEXTURE_LOADER_PATTERNS[] = { "*.bmp", "*.jpg", "*.png", "*.tga", "*.tif", "*.tiff" };
const std::vector< std::string > TEXTURE_LOADER_PATTERNS ( { "*.bmp", "*.jpg", "*.png", "*.tga", "*.tif", "*.tiff" } );

VideoTextureResourceExtraData::~VideoTextureResourceExtraData( void )
   {
   glDeleteTextures( 1, &m_TexObj );
   m_TexObj = 0;
   }

VideoTextureResourceLoader::VideoTextureResourceLoader( void ) : VideoResourceLoader( TEXTURE_LOADER_PATTERNS )
   {
   
   }

int VideoTextureResourceLoader::VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle )
   {
   VideoTextureResourceExtraData* pData = ENG_NEW VideoTextureResourceExtraData();
   shared_ptr< IVideoResourceExtraData > pExtraData( pData );

   pData->m_TexObj = 0;
   glGenTextures( 1, &pData->m_TexObj );
   
   glBindTexture( GL_TEXTURE_2D, pData->m_TexObj );
   // No worry for row size that is not evenly divided by 4
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

   auto pSurface = static_pointer_cast< SDLTextureResourceExtraData >( handle->GetExtraData() )->m_pSurface;

   auto pVideoTextureRes = dynamic_pointer_cast< VideoTextureResource >( handle->GetResource() );
   bool isSRGB = false;
   if( pVideoTextureRes )
      {
      isSRGB = pVideoTextureRes->m_bIsSRGB;
      }
   GLint internlFormat = 0;
   GLenum format = GL_RGB;
   GLenum type = GL_UNSIGNED_BYTE;
   //GLenum textureFormat = GL_RGB;
   switch( pSurface->format->format )
      {
      case SDL_PIXELFORMAT_RGB24:
        // textureFormat = GL_RGB;
         if( isSRGB )
            {
            internlFormat = GL_SRGB8;
            }
         else
            {
            internlFormat = GL_RGB8;
            }
         format = GL_RGB;
         type = GL_UNSIGNED_BYTE;
         // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, pSurface->w, pSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, pSurface->pixels );
         break;
      case SDL_PIXELFORMAT_BGR24:
         if( isSRGB )
            {
            internlFormat = GL_SRGB8;
            }
         else
            {
            internlFormat = GL_RGB8;
            }
         format = GL_BGR;
         type = GL_UNSIGNED_BYTE;
         // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, pSurface->w, pSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, pSurface->pixels );
         break;
      case SDL_PIXELFORMAT_RGBA8888:
         if( isSRGB )
            {
            internlFormat = GL_SRGB8_ALPHA8;
            }
         else
            {
            internlFormat = GL_RGBA8;
            }
         format = GL_RGBA;
         type = GL_UNSIGNED_BYTE;
         // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, pSurface->w, pSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSurface->pixels );
         break;
      case SDL_PIXELFORMAT_ARGB8888:
         if( isSRGB )
            {
            internlFormat = GL_SRGB8_ALPHA8;
            }
         else
            {
            internlFormat = GL_RGBA8;
            }
         format = GL_BGRA;
         type = GL_UNSIGNED_INT_8_8_8_8_REV;
         // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, pSurface->w, pSurface->h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pSurface->pixels ); 
            break;
      default:        
         ENG_ASSERT( 0 && "Image format not supported" );
         break;
      };

   glTexImage2D( GL_TEXTURE_2D, 0, internlFormat, pSurface->w, pSurface->h, 0, format, type, pSurface->pixels );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   
   videoHandle->SetExtraData( shared_ptr< IVideoResourceExtraData >( pExtraData ) );
   return S_OK;
   }

GLuint VideoTextureResourceLoader::LoadAndReturnTextureObject( shared_ptr< VideoTextureResource > pResource )
   {
   auto pVideoHandle = g_pApp->m_pVideoResCache->GetHandle( pResource );
   auto pRet = static_pointer_cast< VideoTextureResourceExtraData >( pVideoHandle->GetExtraData() );
   return pRet->m_TexObj;
   }
