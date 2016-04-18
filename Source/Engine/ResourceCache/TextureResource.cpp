////////////////////////////////////////////////////////////////////////////////
// Filename: TextureResource.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "TextureResource.h"
#include "SDL_image.h"


class BmpResourceLoader : public TextureResourceLoader
   {
   public:
	   virtual std::string VGetPattern() { return "*.bmp"; }
   };

shared_ptr<IResourceLoader> CreateBmpResourceLoader()
   {
	return shared_ptr<IResourceLoader>( ENG_NEW BmpResourceLoader() );
   }

//
// class JpgResourceLoader					- creates an interface with the Resource cache to load JPG files
//
class JpgResourceLoader : public TextureResourceLoader
   {
public:
	virtual std::string VGetPattern() { return "*.jpg"; }
   };

shared_ptr<IResourceLoader> CreateJPGResourceLoader()
   {
	return shared_ptr<IResourceLoader>( ENG_NEW JpgResourceLoader() );
   }

GLTextureResourceExtraData::GLTextureResourceExtraData() : m_pSurface(NULL) 
   {	

   }


unsigned int TextureResourceLoader::VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
   {
	// This will keep the resource cache from allocating memory for the texture, so SDL_IMAGE can manage it on it's own.
   // This value should be reset in VLoadResource by calling SetSize
	return 0;
   }

//
// TextureResourceLoader::VLoadResource				- Chapter 14, page 492
//
bool TextureResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
 	Renderer renderer = EngineApp::GetRendererImpl();
	if ( renderer == Renderer::Renderer_OpenGL )
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
      shared_ptr<GLTextureResourceExtraData> extra = shared_ptr<GLTextureResourceExtraData>( ENG_NEW GLTextureResourceExtraData() );
      extra->m_pSurface = p_Surface;
      handle->SetExtra( extra );
      handle->SetSize( extra->m_pSurface->w * extra->m_pSurface->h * extra->m_pSurface->format->BytesPerPixel );
	   }

	return false;
   }