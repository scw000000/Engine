#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: TextureResource.h
////////////////////////////////////////////////////////////////////////////////

#include "ResourceCache.h"

// 
//  class GLTextureResourceExtraData				- not described in the book, see D3DTextureResourceExtraData11
// TODO: check if openGL need to resotre smaple state
class GLTextureResourceExtraData : public IResourceExtraData
   {
	friend class TextureResourceLoader;

   public:
	   GLTextureResourceExtraData();
	   virtual ~GLTextureResourceExtraData( void ) { SDL_FreeSurface( m_pSurface ); }
	   virtual std::string VToString() override { return "GLTextureResourceExtraData";  }
      void* GetTexture( void ) { return m_pSurface->pixels; };

   protected:
      SDL_Surface* m_pSurface;
   };


// 
//  class TextureResourceLoader				- Chapter 14, page 492
//
class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize );
	virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle );
};
