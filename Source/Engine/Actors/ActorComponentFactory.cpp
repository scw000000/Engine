/*!
 * \file ActorComponentFactory.cpp
 * \date 2016/05/22 9:30
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
#include "ActorComponentFactory.h"

ActorComponentFactory& ActorComponentFactory::GetSingleton( void )
   {
   static ActorComponentFactory factory;
   return factory;
   }

StrongActorComponentPtr ActorComponentFactory::CreateComponent( ComponentId compId, TiXmlElement* pData )
   {
   StrongActorComponentPtr pComponent( GetSingleton().m_CompFactory.Create( compId ) );
   ENG_ASSERT( pComponent );
   if( !pComponent->VInit( pData ) )
      {
      return StrongActorComponentPtr();
      }
   return pComponent;
   }

StrongActorComponentPtr ActorComponentFactory::CreateComponent( const std::string& name, TiXmlElement* pData )
   {
   auto& map = GetSingleton().m_NameToIdMap;
   auto findResult = map.find( name );
   if( findResult == map.end() )
      {
      return StrongActorComponentPtr();
      }
   return GetSingleton().CreateComponent( findResult->second, pData );
   }

ComponentId ActorComponentFactory::GetIdFromName( const std::string& name ) 
   { 
   auto& map = GetSingleton( ).m_NameToIdMap;
   auto findResult = map.find( name );
   if( findResult == map.end( ) )
      {
      return 0;
      }
   return findResult->second;
   }

ActorComponentFactory::ActorComponentFactory( void )
   {

   }