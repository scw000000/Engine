#pragma once
/*!
 * \file XmlResource.h
 * \date 2016/05/01 10:01
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief get Xml node by using tinyXml library
 *
 *  
 *
 * \note
 */

#include "ResourceCache.h"
#include <tinyxml.h>

class XmlResourceExtraData : public IResourceExtraData
   {
   public:
      //virtual std::string VToString() { return "XmlResourceExtraData"; }
      void ParseXml( char* pRawBuffer );
      TiXmlElement* GetRoot(void) { return m_xmlDocument.RootElement(); }

   private:
      TiXmlDocument m_xmlDocument;  
   };

class XmlResourceLoader : public ResourceLoader
   {
   public:
      XmlResourceLoader( void );
      virtual bool VUseRawFile() override { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() override { return true; }
      virtual unsigned int VGetLoadedResourceSize ( char *rawBuffer, unsigned int rawSize ) override { return rawSize; }
      virtual int VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override;
      virtual bool VUsePreAllocate( void ) override { return true; }
      // convenience function
      static TiXmlElement* LoadAndReturnRootXmlElement( const Resource& resource );
   };