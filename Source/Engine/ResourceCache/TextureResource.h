#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: TextureResource.h
////////////////////////////////////////////////////////////////////////////////

#include "ResourceCache.h"

// 
//  class GLTextureResourceExtraData				- not described in the book, see D3DTextureResourceExtraData11
//
class GLTextureResourceExtraData : public IResourceExtraData
   {
	friend class TextureResourceLoader;

   public:
	   GLTextureResourceExtraData();
	   virtual ~GLTextureResourceExtraData() {  //SAFE_RELEASE(m_pTexture); 
         }
	   virtual std::string VToString() { return "GLTextureResourceExtraData"; }

	  // LPDIRECT3DTEXTURE9 const GetTexture() { return m_pTexture; }

   protected:
	//   LPDIRECT3DTEXTURE9		m_pTexture;			
      GLuint m_TextureId;
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
