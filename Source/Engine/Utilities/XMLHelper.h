#pragma once
/*!
 * \file XMLParser.h
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

class XMLHelper
   {
   public:
      static void WriteXMLToFile( const char* fileName, TiXmlElement* pData );
      static std::string WriteXMLToString( TiXmlElement* pData );
   };
