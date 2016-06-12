/*!
 * \file ActorFactory.cpp
 * \date 2016/05/22 15:36
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
#include "ActorFactory.h"
#include "ActorComponentFactory.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "..\ResourceCache\XmlResource.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "LightRenderComponent.h"
#include "SkyRenderComponent.h"


ActorFactory::ActorFactory( void )
   {
   m_LastActorId = INVALID_ACTOR_ID;

   REGISTER_COMPONENT( TransformComponent );
   REGISTER_COMPONENT( MeshRenderComponent );
   REGISTER_COMPONENT( LightRenderComponent );
   REGISTER_COMPONENT( SkyRenderComponent );

   /*m_ComponentFactory.Register<TransformComponent>( ActorComponent::GetIdFromName( TransformComponent::g_Name) );
   m_ComponentFactory.Register<MeshRenderComponent>( ActorComponent::GetIdFromName( MeshRenderComponent::g_Name) );
   m_ComponentFactory.Register<LightRenderComponent>( ActorComponent::GetIdFromName( LightRenderComponent::g_Name) );
   m_ComponentFactory.Register<SkyRenderComponent>( ActorComponent::GetIdFromName( SkyRenderComponent::g_Name) );*/
   }

// This version of function is different than the source code
// Including function arguments LATER: modify it and let it consistant
StrongActorPtr ActorFactory::CreateActor( const char* actorResource, const char* overridesResource, TransformPtr pInitialTransform, ActorId serversActorId )
   {
   Resource actorRes( actorResource );
   TiXmlElement *pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( &actorRes );

   // Resource loading failed, return empty pointer
   if( !pRoot )
      {
      ENG_ERROR( "Failed to create actor from resource: " + std::string( actorResource ) );
      return StrongActorPtr();
      }
   ActorId nextActorId = GetNextActorId();
   if( nextActorId == INVALID_ACTOR_ID )
      {
      ENG_ERROR( "Actor ID generation failed: " + std::string( actorResource ) );
      }

   StrongActorPtr pActor( ENG_NEW Actor( nextActorId ) );
   if( !pActor->Init( pRoot ) )
      {
      ENG_ERROR( "Failed to initialize actor " + std::string( actorResource ) );
      }
   
   for( TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
      {
      StrongActorComponentPtr pComponent( CreateComponent( pNode ) );

      if( pComponent )
         {
         pActor->AddComponent( pComponent );
         pComponent->SetOwner( pActor );
         }
      else
         {
         return StrongActorPtr();
         }
      }

   
   if( overridesResource )
      {
      Resource overrideRes( overridesResource );
      TiXmlElement *pOverrides = XmlResourceLoader::LoadAndReturnRootXmlElement( &overrideRes );
      ENG_ASSERT( pOverrides && "Failed to load overrides resource" );
      if( ModifyActor( pActor, pOverrides ) )
         {
         return StrongActorPtr();
         }
      }

   // This is a bit of a hack to get the initial transform of the transform component set before the 
   // other components (like PhysicsComponent) read it.
   shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( pActor->GetComponent<TransformComponent>( TransformComponent::s_ComponentId ) );
   if( pInitialTransform && pTransformComponent )
      {
      pTransformComponent->SetTransform( *pInitialTransform );
      }

   // Actor has been fully created, run the post init phase
   pActor->PostInit();

   return pActor;
   }

bool ActorFactory::ModifyActor( StrongActorPtr pActor, TiXmlElement* overrides )
   {
	// Loop through each child element and load the component
	for ( TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
	   {
		ComponentId componentId = ActorComponentFactory::GetIdFromName( pNode->Value() );
		StrongActorComponentPtr pComponent = MakeStrongPtr( pActor->GetComponent<IActorComponent>( componentId ) );
		if (pComponent)
		   {
			pComponent->VInit( pNode );
		   }
		else // this should not happen, actor override file should only contains existing components
		   {
         /*pComponent = CreateComponent( pNode );
         if (pComponent)
         {
         pActor->AddComponent(pComponent);
         pComponent->SetOwner(pActor);
         }*/
         return false;
		   }
	   }  		
   return true;
   }

StrongActorComponentPtr ActorFactory::CreateComponent( TiXmlElement* pData )
   {
   std::string name( pData->Value() );

   auto pComponent = ActorComponentFactory::CreateComponent( name, pData );

   ENG_ASSERT( pComponent );

   // Create a component from componentFactory, the Create function take Id as argument
   // Which means it will search its creation function based on id --> id is mapping to creation function
   // typedef BaseClass* (*ObjectCreationFunction)(void); -> called creation function    
   // std::map<IdType, ObjectCreationFunction> m_creationFunctions -> the table of creation functions 
   // ( of course these functions should be registered first )
   // Before that, the id is translated from name to id by using hash string ( GetIdFromName )
   /*StrongActorComponentPtr pComponent( m_ComponentFactory.Create( ActorComponent::GetIdFromName( name.c_str() ) ) );
   if( !pComponent )
      {
      ENG_ERROR( "Cannot create componet due to lossing creation function: " + std::string( name ) );
      return StrongActorComponentPtr();
      }
   
   if( !pComponent->VInit( pData ) )
      {
      ENG_ERROR( "Component failed to initialize: " + std::string( name ) );
      return StrongActorComponentPtr();
      }*/

   return pComponent;
   }