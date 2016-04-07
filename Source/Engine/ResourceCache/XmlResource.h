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
      virtual bool VUseRawFile() { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() { return true; }
      virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) { return rawSize; }
      virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle );
      virtual std::string VGetPattern() { return "*.xml"; }

      // convenience function
      static TiXmlElement* LoadAndReturnRootXmlElement( const char* resourceString );
   };