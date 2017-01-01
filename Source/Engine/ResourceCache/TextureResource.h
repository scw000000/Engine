#pragma once
/*!
 * \file TextureResource.h
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

#include "ResourceCache.h"

// 
// TODO: check if openGL need to resotre smaple state
class SDLTextureResourceExtraData : public IResourceExtraData
   {
	friend class TextureResourceLoader;

   public:
	   SDLTextureResourceExtraData();
	   virtual ~SDLTextureResourceExtraData( void ) { SDL_FreeSurface( m_pSurface ); }
	   //virtual std::string VToString() override { return "SDLTextureResourceExtraData";  }
      void* GetTexture( void ) { return m_pSurface->pixels; };
      
   public:
      SDL_Surface* m_pSurface;
   };


// 
//  class TextureResourceLoader				- Chapter 14, page 492
//
class TextureResourceLoader : public ResourceLoader
{
public:
   TextureResourceLoader( void );
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize );
   virtual int VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle );
   virtual bool VUsePreAllocate( void ) override { return false; }
   static SDL_Surface* LoadAndReturnSurface( const Resource& resource );
};
