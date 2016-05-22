/*!
 * \file ActorComponentFactory.h
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

#include "ActorComponent.h"

class ActorComponentFactory : public ENG_Noncopyable 
   {
   public:
      static ActorComponentFactory& GetSingleton( void );
      template <typename T> static void RegisterComponentCreation( const std::string& name, ComponentId compId );
      
      static StrongActorComponentPtr CreateComponent( ComponentId compId, TiXmlElement* pData );
      static StrongActorComponentPtr CreateComponent( const std::string& name, TiXmlElement* pData );

   private:
      ActorComponentFactory( void );

   private:
     // template <typename T> static void RegisterComponent( ComponentId compId );
      std::map< std::string, ComponentId > m_NameToIdMap;
      GenericObjectFactory< IActorComponent, ComponentId> m_CompFactory;
   };

template <typename T> void ActorComponentFactory::RegisterComponentCreation( const std::string& name, ComponentId compId )
   {
   auto& map = GetSingleton().m_NameToIdMap;
   auto& factory = GetSingleton().m_CompFactory;
   auto findResult = map.find( name );
   ENG_ASSERT( findResult == map.end() && !factory.IsRegistered( compId ) && "The component has been registered before!" )
   map[name] = compId;
   factory.Register<T>( compId );
   }
