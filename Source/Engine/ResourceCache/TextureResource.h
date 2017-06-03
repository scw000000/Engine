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


class TextureResourceLoader : public ResourceLoader
{
public:
   TextureResourceLoader( void );
	virtual bool VUseRawFile() const override { return false; }
   virtual bool VDiscardRawBufferAfterLoad() const override { return true; }
	virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize );
   virtual int VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle );
   virtual bool VUsePreAllocate( void ) const override { return false; }
   static SDL_Surface* LoadAndReturnSurface( shared_ptr< Resource > pResource );
};
