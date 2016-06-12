#pragma once
/*!
 * \file Actor.h
 * \date 2016/05/22 16:34
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

#include "RenderComponent.h"

typedef std::string ActorType; 

class Actor
   {
   friend class ActorFactory;
 //  typedef std::map< ComponentId, StrongActorComponentPtr > ActorComponents;
   typedef std::map< ActorComponentId, StrongActorComponentPtr > ActorComponents;

   public:
      explicit Actor( ActorId id );
      ~Actor( void );

      bool Init( TiXmlElement* pData );
      void PostInit( void );
      // To break out the circular reference strongPtr chain of Actor & actorComponent
      void Destroy( void );
      void Update( unsigned long deltaMs );
      TiXmlElement* GenerateXML( void );

      ActorId GetId( void ) const { return m_Id; }
      TransformPtr GetTransformPtr( void ) { return m_pRootComponent.lock()->m_pTransform; }
      //template< class ComponenType > weak_ptr<ComponenType> GetComponent( ComponentId id );
      //template< class ComponentType > weak_ptr<ComponentType> GetComponent( const char* name );

   public:
      std::weak_ptr< RenderComponent > m_pRootComponent;

   private:
      void AddComponent( StrongActorComponentPtr pComponent );
      ActorComponentId GetNextActorCompnentId( void ) { ++m_LastActorComponentId; return m_LastActorComponentId; }

   private:
      ActorId m_Id;
      ActorType m_Type;
      ActorComponents m_Components;
      ActorComponentId m_LastActorComponentId;
      std::string m_Resource; // actor XML file name
   };

//template< class ComponenType > weak_ptr<ComponenType> Actor::GetComponent( ComponentId id )
//   {
//   ActorComponents::iterator findIt = m_Components.find( id );
//   if( findIt != m_Components.end() )
//      {
//      // Get component ptr as class actor component
//      // We need to down cast it to specific actor component class
//      StrongActorComponentPtr pBase( findIt->second );
//      shared_ptr< ComponenType > pSub( std::static_pointer_cast< ComponenType >( pBase ) );
//      // transfer it to weak ptr
//      weak_ptr< ComponenType > pWeakSub( pSub );
//      return pWeakSub;
//      }
//   else
//      {
//      return weak_ptr< ComponenType>();
//      }
//   }

//template< class ComponentType > weak_ptr<ComponentType> Actor::GetComponent( const char* name )
//   {
//   ComponentId id = ActorComponent::GetIdFromName( name );
//   return GetComponent<ComponentType>( id );
//   }