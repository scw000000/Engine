/*!
 * \file MeshRenderComponent.cpp
 * \date 2016/07/24 14:03
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
#include "MeshRenderComponent.h"
#include "..\Graphics\SceneNode\MeshSceneNode.h"
#include "..\Physics\PhysicsAttributes.h"
#include "..\ResourceCache\MeshResource.h"

const ComponentRegisterId BaseActorComponent<MeshRenderComponent>::s_ComponentId = 0x7a02ca99;
const std::string BaseActorComponent<MeshRenderComponent>::s_Name = "MeshRenderComponent";

void MeshRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pMaterial.reset();
   m_pMeshResource.reset();
   }

MeshRenderComponent::MeshRenderComponent( void ) : m_pMeshResource( ENG_NEW Resource( "" ) ), m_pMaterial( ENG_NEW Material )
   {
   m_pPhysicsAttributes.reset( ENG_NEW BulletSpherePhysicsAttributes );
   }

// This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
shared_ptr<SceneNode> MeshRenderComponent::VCreateSceneNode( void )
   {
   auto pScene = MeshResourceLoader::LoadAndReturnScene( *m_pMeshResource );
   shared_ptr< SceneNode > pMeshRootNode( ENG_NEW SceneNode( m_pOwner->GetId(), this, RenderGroup_Actor, m_pTransform ) );

   std::queue< std::pair< shared_ptr< SceneNode >, aiNode* > > nodeQueue;
   nodeQueue.push( std::pair< shared_ptr< SceneNode >, aiNode* >( pMeshRootNode, pScene->mRootNode ) );

   std::vector< MaterialPtr > pMaterials;
   pMaterials.reserve( pScene->mNumMeshes );
   std::string filePath = m_pMeshResource->GetPath();
   for( unsigned int i = 0; i < pScene->mNumMeshes; ++i )
      {
      pMaterials.push_back( MaterialPtr( ENG_NEW Material( pScene, i, filePath ) ) );
      if( pMaterials[ i ]->m_DiffuseTextureRes.m_Name.size() == 0 )
         {
         pMaterials[ i ]->m_DiffuseTextureRes = m_pMaterial->m_DiffuseTextureRes;
         }
      }
   while( nodeQueue.size() )
      {
      auto nodePair = nodeQueue.front();
      auto pSceneNode = nodePair.first;
      auto pAiNode = nodePair.second;
      nodeQueue.pop();
      for( unsigned int i = 0; i < pAiNode->mNumChildren; ++i )
         {
         auto pAiChildNode = pAiNode->mChildren[ i ];
         shared_ptr< SceneNode > pChildSceneNode( ENG_NEW SceneNode( m_pOwner->GetId(), this, RenderGroup_Actor, TransformPtr( ENG_NEW Transform( pAiChildNode->mTransformation ) ) ) );
         pSceneNode->VAddChild( pChildSceneNode );
         pChildSceneNode->VSetParentNode( pSceneNode.get() );
         nodeQueue.push( std::pair< shared_ptr< SceneNode >, aiNode* >( pChildSceneNode, pAiChildNode ) );
         }
      for( unsigned int i = 0; i < pAiNode->mNumMeshes; ++i )
         {
     //    shared_ptr< Material > pChildMaterial( ENG_NEW Material( *m_pMaterial ) );
       //  pChildMaterial->SetMeshIndex( pAiNode->mMeshes[ i ] );0x73B9A832
        // pChildMaterial->SetMaterialIndex( pScene->mMeshes[ pAiNode->mMeshes[ i ] ]->mMaterialIndex );
         shared_ptr< SceneNode > pChildSceneNode( ENG_NEW MeshSceneNode( m_pOwner->GetId(), this, m_pMeshResource, pMaterials[ pAiNode->mMeshes[ i ] ], RenderGroup_Actor, TransformPtr( ENG_NEW Transform() ) ) );
         pChildSceneNode->VSetParentNode( pSceneNode.get() );
         pSceneNode->VAddChild( pChildSceneNode );
         }
      }
      
 //  shared_ptr< SceneNode > pMeshSceneNode( ENG_NEW MeshSceneNode( m_pOwner->GetId(), this, m_pMeshResource, m_pMaterial, RenderGroup::RenderGroup_Actor, m_pTransform ) );
   //// get the transform component
   //shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::s_ComponentId ) );
   //if ( !pTransformComponent )
   //   {
   //   // can't render without a transform
   //   return shared_ptr<SceneNode>();
   //   }

   //WeakBaseRenderComponentPtr wbrcp(this);
   //shared_ptr< SceneNode > pMeshSceneNode( ENG_NEW MeshSceneNode( m_pOwner->GetId(), wbrcp, m_pMeshResource, m_pMaterial, RenderPass::RenderPass_Actor, pTransformComponent->GetTransform() ) );
   //
   return pMeshRootNode;
   }

bool MeshRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }

   // Set mesh file path
   m_pMeshResource->Init( pData->FirstChildElement( "Mesh" ) );

   m_pMaterial->Init( pData->FirstChildElement( "Material" ) );

   TiXmlElement* pPhysicsAttributes = pData->FirstChildElement( "PhysicsAttributes" );
   if( pPhysicsAttributes && m_pPhysicsAttributes )
      {
      m_pPhysicsAttributes->VInit( pPhysicsAttributes );
      }

   m_pPhysicsAttributes->VSetTransform( *m_pTransform );

   return true;
   }

void MeshRenderComponent::VDelegatePostInit( void )
   {
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( *m_pMeshResource );
   auto pMesh = pAiScene->mMeshes[ 0 ];

   auto pShpereAttr = static_pointer_cast< BulletSpherePhysicsAttributes >( m_pPhysicsAttributes );
   pShpereAttr->SetRadius( this->m_pSceneNode->VGetProperties().GetRadius() );
   m_pPhysicsAttributes->VAddRigidBody( VGetSelfStrongRenderCompPtr() );
   }

void MeshRenderComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement )
   {
   TiXmlElement* pMesh = m_pMeshResource->GenerateXML();
   pMesh->SetValue( "Mesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pMaterial = m_pMaterial->GenerateXML();
   pBaseElement->LinkEndChild( pMaterial );
   }

void MeshRenderComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {
   TiXmlElement* pMesh = m_pMeshResource->GenerateOverridesXML( pResourceNode->FirstChildElement( "Mesh" ) );
   pMesh->SetValue( "Mesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pMaterial = m_pMaterial->GenerateOverridesXML( pResourceNode->FirstChildElement( "Material" ) );
   pBaseElement->LinkEndChild( pMaterial );
   }