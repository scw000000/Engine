/*!
 * \file TextureResource.cpp
 * \date 2016/06/25 17:39
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */
#include "EngineStd.h"
#include "TextureResource.h"
#include "SDL_image.h"

const char *TEXTURE_LOADER_PATTERNS[] = { "*.bmp", "*.jpg", "*.png", "*.tga", "*.tif", "*.tiff" };

SDLTextureResourceExtraData::SDLTextureResourceExtraData() : m_pSurface(NULL) 
   {	

   }

TextureResourceLoader::TextureResourceLoader( void ) : ResourceLoader( std::vector< std::string >( TEXTURE_LOADER_PATTERNS, std::end( TEXTURE_LOADER_PATTERNS ) ) )
   {
   
   }

unsigned int TextureResourceLoader::VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
   {
	// This will keep the resource cache from allocating memory for the texture, so SDL_IMAGE can manage it on it's own.
   // This value should be reset in VLoadResource by calling SetSize
	return 0;
   }

int TextureResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   SDL_RWops* p_RWops = SDL_RWFromMem( rawBuffer, rawSize );
   if( !p_RWops )
      {
      ENG_ERROR( SDL_GetError() );
      return false;
      }
   SDL_Surface* p_Surface = IMG_Load_RW( p_RWops, 0 );
   if( SDL_RWclose( p_RWops ) )
      {
      ENG_WARNING( SDL_GetError() );
      }
   if( !p_Surface )
      {
      ENG_ERROR( SDL_GetError() );
      return false;
      }
   shared_ptr<SDLTextureResourceExtraData> extra = shared_ptr<SDLTextureResourceExtraData>( ENG_NEW SDLTextureResourceExtraData() );
   extra->m_pSurface = p_Surface;
   handle->SetExtraData( extra );
   handle->SetSize( extra->m_pSurface->w * extra->m_pSurface->h * extra->m_pSurface->format->BytesPerPixel );

	return S_OK;
   }

SDL_Surface* TextureResourceLoader::LoadAndReturnSurface( const Resource& resource )
   {
   shared_ptr<ResHandle> pTextureResHandle = g_pApp->m_pResCache->GetHandle( resource );
   shared_ptr< SDLTextureResourceExtraData > pTextureExtra = static_pointer_cast< SDLTextureResourceExtraData >( pTextureResHandle->GetExtraData() );
   return pTextureExtra->m_pSurface;
   }