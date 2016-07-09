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

#define MAX_COLLISION_TYPE_NUM 16

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
      CollisionId typeId = m_CollisionNameToIdMap[ typeName ];
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
            CollisionId otherTypeId = m_CollisionNameToIdMap[ otherTypeName ];
            typeMask |= otherTypeId;
            }
         }
      m_CollisionIdToMaskMap[ typeId ] = typeMask;
      }
   return true;
   }

CollisionId CollisionTable::GetIdFromName( const std::string& typeName ) const
   {
   auto strToIdIt = m_CollisionNameToIdMap.find( typeName );
   if( strToIdIt == m_CollisionNameToIdMap.end() )
      {
      ENG_ERROR( "Collision type is not exist" );
      return 0;
      }
   return strToIdIt->second;
   }

std::string CollisionTable::GetNameFromId( CollisionId id ) const
   {
   auto findIt = m_CollisionIdToNameMap.find( id );
   if( findIt == m_CollisionIdToNameMap.end() )
      {
      ENG_WARNING( "Collision not found" );
      return "unknown";
      }
   return findIt->second;
   }

bool CollisionTable::isCollidable( CollisionId id1, CollisionId id2 ) const
   {
   auto idToMapIt1 = m_CollisionIdToMaskMap.find( id1 );
   ENG_ASSERT( idToMapIt1 != m_CollisionIdToMaskMap.end() );

   auto idToMapIt2 = m_CollisionIdToMaskMap.find( id2 );
   ENG_ASSERT( idToMapIt2 != m_CollisionIdToMaskMap.end() );

   return ( ( id1 & idToMapIt2->second ) && ( id2 && idToMapIt1->second ) );
   }

CollisionMask CollisionTable::GetCollisionMask( CollisionId id ) const
   {
   auto colMapIt = m_CollisionIdToMaskMap.find( id );
   if( colMapIt == m_CollisionIdToMaskMap.end() )
      {
      return 0;
      }
   return colMapIt->second;
   }

bool CollisionTable::AddCollisionType( const std::string& typeName )
   {
   auto strToIdIt = m_CollisionNameToIdMap.find( typeName );
   if( strToIdIt != m_CollisionNameToIdMap.end() )
      {
      return true;
      }
   if( m_CurrMaxBit >= MAX_COLLISION_TYPE_NUM )
      {
      ENG_WARNING( "Too many collision types; table is full" );
      return false;
      }
   m_CurrMaxBit++;
   m_CollisionNameToIdMap[ typeName ] = BIT( m_CurrMaxBit );
   m_CollisionIdToNameMap[ BIT( m_CurrMaxBit ) ] = typeName;
   return true;
   }