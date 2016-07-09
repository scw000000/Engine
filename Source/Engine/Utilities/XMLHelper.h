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
      static void SetAttribute( TiXmlElement* pNode, const char* attr, float value );
      static void SetAttribute( TiXmlElement* pNode, const char* attr, bool value );
      static void SetAttribute( TiXmlElement* pNode, Vec3 value );
      static void GenerateOverride( TiXmlElement* pRetNode, TiXmlElement* pResNode );
      static void WriteXMLToFile( const char* fileName, TiXmlElement* pData );
      static std::string WriteXMLToString( TiXmlElement* pData );
   
   private:
      static int GetChildNodeNum( TiXmlElement* pData );
      static int GetAttributerNum( TiXmlElement* pData );
   };
