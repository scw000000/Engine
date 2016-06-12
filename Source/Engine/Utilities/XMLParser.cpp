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
#include "XMLParser.h"

bool XMLParser::ReadBoolean( TiXmlElement* pData, const char* attribute, bool* pReadVar )
   {
   if( !pData )
      {
      return false;
      }
   return TIXML_SUCCESS == pData->QueryBoolAttribute( attribute, pReadVar );
   }

bool XMLParser::ReadFloat( TiXmlElement* pData, const char* attribute, float* pReadVar )
   {
   if( !pData )
      {
      return false;
      }
   return TIXML_SUCCESS == pData->QueryFloatAttribute( attribute, pReadVar );
   }

bool XMLParser::ReadInteger( TiXmlElement* pData, const char* attribute, int* pReadVar )
   {
   if( !pData )
      {
      return false;
      }
   return TIXML_SUCCESS == pData->QueryIntAttribute( attribute, pReadVar );
   }

bool XMLParser::ReadString( TiXmlElement* pData, const char* attribute, std::string* pReadVar )
   {
   if( !pData || !pData->Attribute( attribute ))
      {
      return false;
      }
   *pReadVar = pData->Attribute( attribute );
   return true;
   }

bool XMLParser::ReadVector3( TiXmlElement* pData, Vec3* pReadVar )
   {
   float x = 0;
   float y = 0;
   float z = 0;
   if( !ReadFloat( pData, "x", &x ) )
      {
      return false;
      }
   if( !ReadFloat( pData, "y", &y ) )
      {
      return false;
      }
   if( !ReadFloat( pData, "z", &z ) )
      {
      return false;
      }
   *pReadVar = Vec3( x, y, z );
   return true;
   }

bool XMLParser::ReadTransform( TiXmlElement* pData, Transform* pReadVar )
   {
   Vec3 pitchYawRoll;
   Vec3 position;
   Vec3 scale;

   if( ReadVector3( pData->FirstChildElement( "PitchYawRoll" ), &pitchYawRoll ) )
      {
      pReadVar->SetRotation( pitchYawRoll );
      }

   if( ReadVector3( pData->FirstChildElement( "Position" ), &position ) )
      {
      pReadVar->SetPosition( position );
      }

   if( ReadVector3( pData->FirstChildElement( "Scale" ), &scale ) )
      {
      pReadVar->SetScale( scale );
      }

   return true;
   }

bool XMLParser::ReadColor( TiXmlElement* pData, Color* pReadVar )
   {
   float r;
   float g;
   float b;
   float a;

   if( !ReadFloat( pData, "r", &r ) )
      {
      return false;
      }
   if( !ReadFloat( pData, "g", &g ) )
      {
      return false;
      }
   if( !ReadFloat( pData, "b", &b ) )
      {
      return false;
      }
   if( !ReadFloat( pData, "a", &a ) )
      {
      return false;
      }

   *pReadVar = Color( r, g, b, a );
   return true;
   }

bool WriteXMLToFile( const char* fileName, TiXmlElement* pRoot )
   {
   TiXmlDocument doc;
   doc.LinkEndChild( pRoot );
   doc.SaveFile( fileName );
   doc.Clear();
   return true;
   }

