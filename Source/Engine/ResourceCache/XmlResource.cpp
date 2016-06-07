////////////////////////////////////////////////////////////////////////////////
// Filename: XmlResource.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "XmlResource.h"

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
   {
   m_xmlDocument.Parse( pRawBuffer );
   }

shared_ptr<IResourceLoader> CreateXmlResourceLoader()
   {
   return shared_ptr<IResourceLoader>( ENG_NEW XmlResourceLoader() );
   }

XmlResourceLoader::XmlResourceLoader( void ) : ResourceLoader( std::vector< std::string >( 1, "*.xml" ) )
   {
   
   }

bool XmlResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   if ( rawSize <= 0 )
      {
      return false;
      }
        
    shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>( ENG_NEW XmlResourceExtraData() );
    pExtraData->ParseXml( rawBuffer );

    handle->SetExtraData( shared_ptr<XmlResourceExtraData>( pExtraData) );

    return true;
   }

// The resource handle will be pushed into m_lruResHandleList until engine exist or swapped out
TiXmlElement* XmlResourceLoader::LoadAndReturnRootXmlElement( Resource* pResource )
   {
   shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( pResource );  // this actually loads the XML file from the zip file
   ENG_ASSERT( pResourceHandle );
   shared_ptr<XmlResourceExtraData> pExtraData = static_pointer_cast< XmlResourceExtraData >( pResourceHandle->GetExtraData() );
   return pExtraData->GetRoot();
   }