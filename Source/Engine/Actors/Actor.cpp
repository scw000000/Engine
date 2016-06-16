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

Actor::Actor( ActorId id ) : m_ActorResource( "Unknown" )
   {
   m_Id = id;
   m_Type = "Unknown";

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
   ENG_LOG( "Actor", std::string( "Initializing Actor " ) + ToStr( m_Id ) );

	m_Type = pData->Attribute( "type" );
   m_ActorResource.Init( pData->FirstChildElement( "Resource" ) );
   unsigned int rootComponentId = 0;
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

TiXmlElement* Actor::BuildComponentXML( StrongActorComponentPtr pComponent )
   {
   ENG_ASSERT( pComponent );
   TiXmlElement* pCurrLevelNode = ENG_NEW TiXmlElement( pComponent->VGetName().c_str() );
   TiXmlElement* pCurrLevelNodeData = pComponent->VGenerateXML();
   pCurrLevelNodeData->SetValue( "Data" );
   pCurrLevelNode->LinkEndChild( pCurrLevelNodeData );
   const ChildComponents& childComponents = pComponent->GetChildComponents();
   for( auto pChildComponent : childComponents )
      {
      auto pStrongChildComponent = pChildComponent.lock();
      if( pStrongChildComponent )
         {
         pCurrLevelNode->LinkEndChild( BuildComponentXML( pStrongChildComponent ) );
         }
      }
   return pCurrLevelNode;
   }

TiXmlElement* Actor::GenerateXML( void )
   {
   // Actor element
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Actor" );
   TiXmlElement* pDataNode = ENG_NEW TiXmlElement( "Data" );
   pDataNode->SetAttribute( "type", m_Type.c_str() );
   pDataNode->LinkEndChild( m_ActorResource.GenerateXML() );
   pRetNode->LinkEndChild( pDataNode );

   // components
   for ( auto it = m_Components.begin(); it != m_Components.end(); ++it )
      {
      StrongActorComponentPtr pComponent = it->second;
      StrongActorComponentPtr pParentComponent = pComponent->GetParentComponent().lock();
      if( !pParentComponent )
         {
         pRetNode->LinkEndChild( BuildComponentXML( pComponent ) );
         }
      
      }

   return pRetNode;
   }

TiXmlElement* Actor::BuildOverridesXML( StrongActorComponentPtr pComponent, TiXmlElement* pResouce )
   {
   ENG_ASSERT( pComponent );
   TiXmlElement* pCurrLevelNode = ENG_NEW TiXmlElement( pComponent->VGetName().c_str() );
   TiXmlElement* pResDataNode = pResouce->FirstChildElement( "Data" );
   TiXmlElement* pCurrLevelNodeData = pComponent->VGenerateOverridesXML( pResDataNode );
   pCurrLevelNodeData->SetValue( "Data" );
   pCurrLevelNode->LinkEndChild( pCurrLevelNodeData );

   TiXmlElement* pResChildComponentNode = pResDataNode->NextSiblingElement();
   const ChildComponents& childComponents = pComponent->GetChildComponents();

   for( auto pChildComponent : childComponents )
      {
      auto pStrongChildComponent = pChildComponent.lock();
      if( pStrongChildComponent )
         {
         pCurrLevelNode->LinkEndChild( BuildOverridesXML( pStrongChildComponent, pResChildComponentNode ) );
         pResChildComponentNode = pResChildComponentNode->NextSiblingElement();
         }
      }
   return pCurrLevelNode;
   }

TiXmlElement* Actor::GenerateOverridesXML( TiXmlElement* pResouce )
   {
   // Actor element
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Actor" );

   //Type and resource should not be changed in overrides file, so no need to write it
   
   TiXmlElement* pResComponentNode = pResouce->FirstChildElement( "Data" )->NextSiblingElement();
  
   // components
   for( auto it = m_Components.begin(); it != m_Components.end(); ++it )
      {
      StrongActorComponentPtr pComponent = it->second;
      StrongActorComponentPtr pParentComponent = pComponent->GetParentComponent().lock();
      if( !pParentComponent )
         {
         TiXmlElement* pComponentNode = BuildOverridesXML( pComponent, pResComponentNode );
         pRetNode->LinkEndChild( pComponentNode );
         pResComponentNode = pResComponentNode->NextSiblingElement();
         }

      }

   return pRetNode;
   }

TransformPtr Actor::GetTransformPtr( void )
   {
   weak_ptr<IActorComponent> pComponent = GetComponent( 1 );
   ENG_ASSERT( !pComponent.expired() );
   shared_ptr<IRenderComponent> pRootComponent = dynamic_pointer_cast< IRenderComponent >( pComponent.lock() );
   ENG_ASSERT( pRootComponent );
   return pRootComponent->VGetTransform();
   }

void Actor::AddComponent( StrongActorComponentPtr pComponent )
   {
   ENG_ASSERT( GetNextActorCompnentId() != INVALID_ACTOR_COMPONENT_ID );

   std::pair<ActorComponents::iterator, bool> success = m_Components.insert( std::make_pair( m_LastActorComponentId, pComponent ) );
   ENG_ASSERT( success.second );
   }

weak_ptr<IActorComponent> Actor::GetComponent( ActorComponentId id )
   {
   ActorComponents::iterator findIt = m_Components.find( id );
   if( findIt != m_Components.end() )
      {
      // Get component ptr as class actor component
      // We need to down cast it to specific actor component class
      StrongActorComponentPtr pStrong( findIt->second );
      weak_ptr< IActorComponent > pWeak( pStrong );
      return pWeak;
      }
   else
      {
      return weak_ptr< IActorComponent>();
      }
   }