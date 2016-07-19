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

void XMLHelper::SetAttribute( TiXmlElement* pNode, const char* attr, float value )
   {
   pNode->SetAttribute( attr, ToStr( value ).c_str() );
   }

void XMLHelper::SetAttribute( TiXmlElement* pNode, const char* attr, bool value )
   {
   pNode->SetAttribute( attr, value? "yes":"no" );
   }

void XMLHelper::SetAttribute( TiXmlElement* pNode, Vec3 value )
   {
   SetAttribute( pNode, "x", value.x );
   SetAttribute( pNode, "y", value.y );
   SetAttribute( pNode, "z", value.z );
   }

void XMLHelper::GenerateOverride( TiXmlElement* pRetNode, TiXmlElement* pResNode )
   {
   if( !pRetNode && !pResNode )
      {
      return;
      }
   ENG_ASSERT( pRetNode && pResNode );
   ENG_ASSERT( !std::strcmp( pRetNode->Value(), pResNode->Value() ) );
   ENG_ASSERT( GetChildNodeNum( pRetNode ) == GetChildNodeNum( pResNode ) );
   ENG_ASSERT( GetAttributerNum( pRetNode ) == GetAttributerNum( pResNode ) );

   auto retAttr = pRetNode->FirstAttribute();
   auto resAttr = pResNode->FirstAttribute();

   while( retAttr && resAttr )
      {
      ENG_ASSERT( !std::strcmp( retAttr->Name(), resAttr->Name() ) );
      if( !std::strcmp( retAttr->Value(), resAttr->Value() ) )
         {
         std::string removeName = retAttr->Name();
         retAttr = retAttr->Next();
         resAttr = resAttr->Next();
         pRetNode->RemoveAttribute( removeName.c_str() );
         continue;
         }

      retAttr = retAttr->Next();
      resAttr = resAttr->Next();
      }

   TiXmlElement* retChild = pRetNode->FirstChildElement();
   TiXmlElement* resChild = pResNode->FirstChildElement();
   for( ; retChild && resChild; retChild = retChild->NextSiblingElement(), resChild = resChild->NextSiblingElement() )
      {
      GenerateOverride( retChild, resChild );
      }
   }

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

int XMLHelper::GetChildNodeNum( TiXmlElement* pData )
   {
   int ret = 0;
   for( TiXmlElement* child = pData->FirstChildElement(); child; child = child->NextSiblingElement() )
      {
      ret++;
      }
   return ret;
   }

int XMLHelper::GetAttributerNum( TiXmlElement* pData )
   {
   
   int ret = 0;
   for( auto attr = pData->FirstAttribute(); attr; attr = attr->Next() )
      {
      ret++;
      }
   return ret;
   }