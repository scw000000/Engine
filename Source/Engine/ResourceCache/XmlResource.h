#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: XmlResource.h
////////////////////////////////////////////////////////////////////////////////

#include "ResourceCache.h"
#include <tinyxml.h>

class XmlResourceExtraData : public IResourceExtraData
   {
   public:
      virtual std::string VToString() { return "XmlResourceExtraData"; }
      void ParseXml( char* pRawBuffer );
      TiXmlElement* GetRoot(void) { return m_xmlDocument.RootElement(); }

   private:
      TiXmlDocument m_xmlDocument;  
   };

class XmlResourceLoader : public IResourceLoader
   {
   public:
      virtual bool VUseRawFile() override { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() override { return true; }
      virtual unsigned int VGetLoadedResourceSize ( char *rawBuffer, unsigned int rawSize ) override { return rawSize; }
      virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override;
      virtual std::string VGetPattern() override { return "*.xml"; }
      virtual bool VUsePreAllocate( void ) override { return true; }
      // convenience function
      static TiXmlElement* LoadAndReturnRootXmlElement( const char* resourceString );
   };