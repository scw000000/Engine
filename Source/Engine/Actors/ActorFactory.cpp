////////////////////////////////////////////////////////////////////////////////
// Filename: ActorFactory.cpp
////////////////////////////////////////////////////////////////////////////////


///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"
#include "ActorFactory.h"
#include "Actor.h"
#include "ActorComponent.h"

ActorFactory::ActorFactory( void )
   {
   m_lastActorId = INVALID_ACTOR_ID;
   }

// This version of function is different than the source code
// Including function arguments
StrongActorPtr ActorFactory::CreateActor( const char* actorResource )
   {
   // Todo: implement XmlResourceLoader 
   //TiXmlElement *pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( actorResource );
   TiXmlElement *pRoot = NULL;

   // Resource loading failed, return empty pointer
   if( !pRoot )
      {
      ENG_ERROR( "Failed to create actor from resource: " + std::string( actorResource ) );
      return StrongActorPtr();
      }
   
   StrongActorPtr pActor( ENG_NEW Actor( GetNextActorId() ) );
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

   // Actor has been fully created, run the post ini phase
   pActor->PostInit();

   return pActor;
   }

void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
   {
	// Loop through each child element and load the component
	for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	   {
		ComponentId componentId = ActorComponent::GetIdFromName( pNode->Value() );
		StrongActorComponentPtr pComponent = MakeStrongPtr( pActor->GetComponent<ActorComponent>( componentId ) );
		if (pComponent)
		   {
			pComponent->VInit( pNode );
		   }
		else
		   {
			pComponent = CreateComponent( pNode );
			if (pComponent)
			   {
				pActor->AddComponent(pComponent);
				pComponent->SetOwner(pActor);
			   }
		   }
	   }  		
   }

StrongActorComponentPtr ActorFactory::CreateComponent( TiXmlElement* pData )
   {
   std::string name( pData->Value() );

   // Create a component from componentFactory, the Create function take Id as argument
   // Which means it will search its creation function based on id -> id is mapping to creation function
   // typedef BaseClass* (*ObjectCreationFunction)(void); -> called creation function    
   // std::map<IdType, ObjectCreationFunction> m_creationFunctions -> the table of creation functions 
   // ( of course these functions should be registered first )
   // Before that, the id is translated from name to id by using hash string ( GetIdFromName )
   StrongActorComponentPtr pComponent( m_componentFactory.Create( ActorComponent::GetIdFromName( name.c_str() ) ) );
   if( !pComponent )
      {
      ENG_ERROR( "Cannot create componet due to lossing creation function: " + std::string( name ) );
      return StrongActorComponentPtr();
      }
   
   if( !pComponent->VInit( pData ) )
      {
      ENG_ERROR( "Component failed to initialize: " + std::string( name ) );
      return StrongActorComponentPtr();
      }
   return pComponent;
   }