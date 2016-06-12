/*!
 * \file Actor.cpp
 * \date 2016/05/22 10:55
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
#include "Actor.h"
#include "ActorComponent.h"
#include "ActorFactory.h"

Actor::Actor( ActorId id )
   {
   m_Id = id;
   m_Type = "Unknown";

	// [mrmike] added post press - this is an editor helper
	m_Resource = "Unknown";
   m_LastActorComponentId = INVALID_ACTOR_COMPONENT_ID;
   }

// The actor should call Destroy() before calling destructor
Actor::~Actor( void )
   {
   ENG_LOG( "Actor", std::string( "Destroying Actor " ) + ToStr( m_Id ) );
   ENG_ASSERT( m_Components.empty() );  // [rez] if this assert fires, the actor was destroyed without calling Actor::Destroy()
   }

bool Actor::Init( TiXmlElement* pData )
   {
   ENG_LOG("Actor", std::string( "Initializing Actor " ) + ToStr( m_Id ) );

	m_Type = pData->Attribute( "type" );
	m_Resource = pData->Attribute( "resource" );
   return true;
   }

void Actor::PostInit( void )
   {
   for (ActorComponents::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
      {
      it->second->VPostInit();
      }
   }

// Once these pointers are deleted from map, it will delete itself automatically
void Actor::Destroy( void )
   {
   // Call map::clear
   for( auto comp : m_Components )
      {
      comp.second->Destory();
      }
   m_Components.clear();
   }

void Actor::Update( unsigned long deltaMs)
   {
   for (ActorComponents::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
      {
      it->second->VUpdate(deltaMs);
      }
   }

TiXmlElement* Actor::GenerateXML( void )
   {
   // Actor element
   TiXmlElement* pActorElement = ENG_NEW TiXmlElement( "Actor" );
   pActorElement->SetAttribute( "type", m_Type.c_str() );
	pActorElement->SetAttribute( "resource", m_Resource.c_str() );

   // components
   for ( auto it = m_Components.begin(); it != m_Components.end(); ++it )
      {
      StrongActorComponentPtr pComponent = it->second;
      TiXmlElement* pComponentElement = pComponent->VGenerateXML();
      pActorElement->LinkEndChild( pComponentElement );
      }

   return pActorElement;
   }


void Actor::AddComponent( StrongActorComponentPtr pComponent )
   {
   ENG_ASSERT( GetNextActorCompnentId() != INVALID_ACTOR_COMPONENT_ID );

   std::pair<ActorComponents::iterator, bool> success = m_Components.insert( std::make_pair( m_LastActorComponentId, pComponent ) );
   ENG_ASSERT( success.second );
   }