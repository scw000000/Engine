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

class XMLParser
   {
   public:
      //For reading attribute 
      static bool ReadBoolean( TiXmlElement* pData, const char* attribute, bool* pReadVar );
      static bool ReadFloat( TiXmlElement* pData, const char* attribute, float* pReadVar );
      static bool ReadInteger( TiXmlElement* pData, const char* attribute, int* pReadVar );
      static bool ReadString( TiXmlElement* pData, const char* attribute, std::string* pReadVar );

      //For reading nodes
      static bool ReadVector3( TiXmlElement* pData, Vec3* pReadVar );
      static bool ReadTransform( TiXmlElement* pData, Transform* pReadVar );
      static bool ReadColor( TiXmlElement* pData, Color* pReadVar );
   };

extern bool WriteXMLToFile( const char* fileName, TiXmlElement* pRoot );