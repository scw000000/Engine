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
#include "..\ResourceCache\XmlResource.h"


ActorFactory::ActorFactory( void )
   {
   m_LastActorId = INVALID_ACTOR_ID;
   }

// This version of function is different than the source code
// Including function arguments LATER: modify it and let it consistant
StrongActorPtr ActorFactory::CreateActor( const Resource& classRes, const Resource* pOverridesRes, TransformPtr pInitialTransform /*= NULL*/, ActorId serversActorId /*= INVALID_ACTOR_ID */ )
   {  
   TiXmlElement* pActorClassNode = XmlResourceLoader::LoadAndReturnRootXmlElement( classRes );
   
   if( !pActorClassNode )
      {
      ENG_ERROR( "Failed to read actor class resource: " + classRes.m_Name );
      return StrongActorPtr();
      }

   ActorId nextActorId = GetNextActorId();
   if( nextActorId == INVALID_ACTOR_ID )
      {
      ENG_ERROR( "Actor ID generation failed: " + classRes.m_Name );
      }

   TiXmlElement* pActorClassDataNode = pActorClassNode->FirstChildElement( "Data" );
   StrongActorPtr pActor( ENG_NEW Actor( nextActorId ) );
   if( !pActor->Init( pActorClassDataNode ) )
      {
      ENG_ERROR( "Failed to initialize actor " + classRes.m_Name );
      }
   
   TiXmlElement* pFirstResComponentNode = pActorClassDataNode->NextSiblingElement();
   for( TiXmlElement* pNode = pFirstResComponentNode; pNode; pNode = pNode->NextSiblingElement() )
      {
      
      StrongActorComponentPtr pComponent = BuildComponentTree( pNode, pActor, StrongActorComponentPtr() );

      if( pComponent )
         {
         pComponent->SetOwner( pActor );
         }
         else
         {
         return StrongActorPtr();
         }
      }

   if( pOverridesRes )
      {
      TiXmlElement* pOverridesNode = XmlResourceLoader::LoadAndReturnRootXmlElement( *pOverridesRes );
      pActor->m_pActorInstanceResource.reset( ENG_NEW Resource( pOverridesRes->m_Name ) );
      // Resource loading failed, return empty pointer
      if( !pOverridesNode )
         {
         ENG_ERROR( "Failed to read actor overrides resource: " + pOverridesRes->m_Name );
         return StrongActorPtr();
         }
      if( !ModifyActor( pActor, pOverridesNode ) )
         {
         return StrongActorPtr();
         }
      }

  if( pInitialTransform && pActor->m_Components.size() )
      {
      StrongRenderComponentPtr pRenderComponent = dynamic_pointer_cast< IRenderComponent >( pActor->m_Components[ 1 ] );
      if( pRenderComponent )
         {
         pRenderComponent->VSetTransform( *pInitialTransform );
         }
      }

   // Actor has been fully created, run the post init phase
   pActor->PostInit();

   return pActor;
   }

bool ActorFactory::ModifyActor( StrongActorPtr pActor, TiXmlElement* overrides )
   {
   TiXmlElement* pOverridesComponentNode = overrides->FirstChildElement( "Data" )->NextSiblingElement();

   for( auto it = pActor->m_Components.begin(); it != pActor->m_Components.end(); ++it )
      {
      StrongActorComponentPtr pComponent = it->second;
      StrongActorComponentPtr pParentComponent = pComponent->VGetParentComponent().lock();
      if( !pParentComponent )
         {
         if( !ModifyActorComponent( pComponent, pOverridesComponentNode ) )
            {
            return false;
            }
         pOverridesComponentNode = pOverridesComponentNode->NextSiblingElement();
         }

      }

   return true;

	//for ( TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
	//   {
	//	ComponentId componentId = ActorComponentFactory::GetIdFromName( pNode->Value() );
	////	StrongActorComponentPtr pComponent = MakeStrongPtr( pActor->GetComponent<IActorComponent>( componentId ) );
	//	if (pComponent)
	//	   {
	//		pComponent->VInit( pNode );
	//	   }
	//	else // this should not happen, actor override file should only contains existing components
	//	   {
 //        /*pComponent = CreateComponent( pNode );
 //        if (pComponent)
 //        {
 //        pActor->AddComponent(pComponent);
 //        pComponent->SetOwner(pActor);
 //        }*/
 //        return false;
	//	   }
	//   }  		
   return true;
   }

StrongActorComponentPtr ActorFactory::CreateComponent( const std::string& componentName, TiXmlElement* pData )
   {
   auto pComponent = ActorComponentFactory::CreateComponent( componentName, pData );

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

StrongActorComponentPtr ActorFactory::BuildComponentTree( TiXmlElement* pData, StrongActorPtr pActor, StrongActorComponentPtr pParent )
   {
   // Build current level component first
   TiXmlElement* pDataNode = pData->FirstChildElement( "Data" );
   ENG_ASSERT( pDataNode );
   StrongActorComponentPtr pCurrLevelComponent( CreateComponent( pData->Value(), pDataNode ) );

   if( pCurrLevelComponent )
      {
      pActor->AddComponent( pCurrLevelComponent );
      pCurrLevelComponent->SetActorCompoenetId( pActor->m_LastActorComponentId );
      pCurrLevelComponent->SetOwner( pActor );
      pCurrLevelComponent->SetParentComponent( pParent );
      }
   else
      {
      return StrongActorComponentPtr();
      }

   // Build the children components for current level component
   for( TiXmlElement* pNode = pData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
      {
      if( !std::strcmp( pNode->Value(), "Data" ) )
         {
         
         }
      else
         {
         StrongActorComponentPtr pChildComponent = BuildComponentTree( pNode, pActor, pCurrLevelComponent );
         if( !pChildComponent )
            {
            return StrongActorComponentPtr();
            }
         pCurrLevelComponent->AddChildComponent( pChildComponent );
         }
      }
   return pCurrLevelComponent;
   }

bool ActorFactory::ModifyActorComponent( StrongActorComponentPtr pActorComponent, TiXmlElement* pOverrides )
   {
   TiXmlElement* pDataNode = pOverrides->FirstChildElement( "Data" );
   if( !pActorComponent->VInit( pDataNode ) )
      {
      return false;
      }
   auto childComponents = pActorComponent->VGetChildComponents();
   TiXmlElement* pChildComponentNode = pDataNode->NextSiblingElement();
   for( auto actorCompIt : childComponents )
      {
      StrongActorComponentPtr pChildComponent = actorCompIt.lock();
      if( !pChildComponent || !pChildComponentNode )
         {
         return false;
         }
      ModifyActorComponent( pChildComponent, pChildComponentNode );
      pChildComponentNode = pChildComponentNode->NextSiblingElement();
      }
   return true;
   }