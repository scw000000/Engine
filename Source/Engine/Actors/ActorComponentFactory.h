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
      template <typename T> static void RegisterComponentCreation( const std::string& name, ComponentRegisterId compId );
      
      static StrongActorComponentPtr CreateComponent( ComponentRegisterId compId, TiXmlElement* pData );
      static StrongActorComponentPtr CreateComponent( const std::string& name, TiXmlElement* pData );
      static ComponentRegisterId GetIdFromName( const std::string& name );

   private:
      ActorComponentFactory( void );

   private:
      std::map< std::string, ComponentRegisterId > m_NameToIdMap;
      GenericObjectFactory< IActorComponent, ComponentRegisterId> m_CompFactory;
   };

template <typename T> void ActorComponentFactory::RegisterComponentCreation( const std::string& name, ComponentRegisterId compId )
   {
   auto& map = GetSingleton().m_NameToIdMap;
   auto& factory = GetSingleton().m_CompFactory;
   ENG_ASSERT(  map.find( name ) == map.end() && !factory.IsRegistered( compId ) && "The component has been registered before!" );
   map[name] = compId;
   factory.Register<T>( compId );
   }

#define REGISTER_COMPONENT( componentClass ) ActorComponentFactory::RegisterComponentCreation<componentClass>( componentClass::s_Name, componentClass::s_ComponentId )

#define CREATE_COMPONENT_FROM_NAME( name, pData ) ActorComponentFactory::CreateComponent( name, pData )