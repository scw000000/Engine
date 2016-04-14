////////////////////////////////////////////////////////////////////////////////
// Filename: TextureResource.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "TextureResource.h"


//
// class DdsResourceLoader					- creates an interface with the Resource cache to load DDS files
//
class DdsResourceLoader : public TextureResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.dds"; }
};

shared_ptr<IResourceLoader> CreateDDSResourceLoader()
   {
	return shared_ptr<IResourceLoader>( ENG_NEW DdsResourceLoader() );
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

GLTextureResourceExtraData::GLTextureResourceExtraData()
//: m_pTexture(NULL) 
   {	
   }


unsigned int TextureResourceLoader::VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
   {
	// This will keep the resource cache from allocating memory for the texture, so DirectX can manage it on it's own.
	return 0;
   }

//
// TextureResourceLoader::VLoadResource				- Chapter 14, page 492
//
bool TextureResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
   {
	Renderer renderer = EngineApp::GetRendererImpl();
	if ( renderer == Renderer::Renderer_OpenGL )
	   {
		shared_ptr<GLTextureResourceExtraData> extra = shared_ptr<GLTextureResourceExtraData>( ENG_NEW GLTextureResourceExtraData() );

		if ( FAILED ( D3DXCreateTextureFromFileInMemory( DXUTGetD3D9Device(), rawBuffer, rawSize, &extra->m_pTexture ) ) )
         {
			return false;
         }
		else 
		   {
			handle->SetExtra( shared_ptr<GLTextureResourceExtraData>( extra ) );
			return true;
		   }
	   }


	ENG_ASSERT( 0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
   }