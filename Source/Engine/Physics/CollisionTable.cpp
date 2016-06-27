/*!
 * \file CollisionTable.cpp
 * \date 2016/06/26 16:53
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
#include "CollisionTable.h"

#define BIT(x) ( 1u << ( x ) )

CollisionTable& CollisionTable::GetSingleton( void )
   {
   static CollisionTable collisionTable;
   return collisionTable;
   }

bool CollisionTable::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   for( auto pType = pData->FirstChildElement(); pType; pType = pType->NextSiblingElement() )
      {
      std::string typeName = pType->Value();
      if( !AddCollisionType( typeName ) )
         {
         return false;
         }
      CollisionId typeId = m_CollisionIdMap[ typeName ];
      CollisionMask typeMask = 0;
      for( auto pOtherType = pType->FirstChildElement(); pOtherType; pOtherType = pOtherType->NextSiblingElement() )
         {
         std::string otherTypeName = pOtherType->Value();
         if( !AddCollisionType( otherTypeName ) )
            {
            return false;
            }
         bool canCollide = false;
         if( pOtherType->QueryBoolAttribute( "collision", &canCollide ) != TIXML_SUCCESS )
            {
            ENG_ERROR( "collision setting attribute is not exist" );
            return false;
            }
         
         if( canCollide )
            {
            CollisionId otherTypeId = m_CollisionIdMap[ otherTypeName ];
            typeMask |= otherTypeId;
            }
         }
      m_CollisionTableMap[ typeId ] = typeMask;
      }
   return true;
   }

CollisionId CollisionTable::GetIdFromName( const std::string& typeName )
   {
   auto strToIdIt = m_CollisionIdMap.find( typeName );
   if( strToIdIt == m_CollisionIdMap.end() )
      {
      ENG_ERROR( "Collision type is not exist" );
      return 0;
      }
   return strToIdIt->second;
   }

bool CollisionTable::isCollidable( CollisionId id1, CollisionId id2 )
   {
   auto idToMapIt1 = m_CollisionTableMap.find( id1 );
   ENG_ASSERT( idToMapIt1 != m_CollisionTableMap.end() );

   auto idToMapIt2 = m_CollisionTableMap.find( id2 );
   ENG_ASSERT( idToMapIt2 != m_CollisionTableMap.end() );

   return ( ( id1 & idToMapIt2->second ) && ( id2 && idToMapIt1->second ) );
   }

bool CollisionTable::AddCollisionType( const std::string& typeName )
   {
   auto strToIdIt = m_CollisionIdMap.find( typeName );
   if( strToIdIt != m_CollisionIdMap.end() )
      {
      return true;
      }
   if( m_CurrMaxBit >= 31 )
      {
      ENG_ERROR( "Too many collision type; table is full" );
      return false;
      }
   m_CurrMaxBit++;
   m_CollisionIdMap[ typeName ] = BIT( m_CurrMaxBit );
   return true;
   }