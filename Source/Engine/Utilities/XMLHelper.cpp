/*!
 * \file XMLParser.cpp
 * \date 2016/06/12 11:56
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

#include "EngineStd.h"
#include "XMLHelper.h"

void XMLHelper::WriteXMLToFile( const char* fileName, TiXmlElement* pData )
   {
   TiXmlDocument doc;
   doc.LinkEndChild( pData );
   doc.SaveFile( fileName );
   }

std::string XMLHelper::WriteXMLToString( TiXmlElement* pData )
   {
   TiXmlDocument doc;
   doc.LinkEndChild( pData );
   TiXmlPrinter printer;
   doc.Accept( &printer );
   return printer.CStr();
   }
