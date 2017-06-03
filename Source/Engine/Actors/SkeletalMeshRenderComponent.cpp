/*!
 * \file SkeletalMeshRenderComponent.cpp
 * \date 2016/07/24 14:32
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
#include "SkeletalMeshRenderComponent.h"
#include "..\Graphics\SceneNode\SkeletalMeshSceneNode.h"
#include "..\Physics\PhysicsAttributes.h"
#include "..\ResourceCache\MeshResource.h"

const ComponentRegisterId BaseActorComponent<SkeletalMeshRenderComponent>::s_ComponentId = 0x8da594fd;
const std::string BaseActorComponent<SkeletalMeshRenderComponent>::s_Name = "SkeletalMeshRenderComponent";

void SkeletalMeshRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pMaterial.reset();
   m_pSkeletalMeshResource.reset();
   m_pAnimScriptResource.reset();
   }

SkeletalMeshRenderComponent::SkeletalMeshRenderComponent( void ) : m_pSkeletalMeshResource( ENG_NEW Resource( "" ) ), m_pAnimScriptResource( ENG_NEW Resource( "" ) ), m_pMaterial( ENG_NEW Material )
   {
   m_pPhysicsAttributes.reset( ENG_NEW BulletSpherePhysicsAttributes );
   }

// This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
shared_ptr<SceneNode> SkeletalMeshRenderComponent::VCreateSceneNode( void )
   {
   shared_ptr< SceneNode > pSKMeshSceneNode( ENG_NEW SkeletalMeshSceneNode( m_pOwner->GetId(), this, m_pSkeletalMeshResource, m_pAnimScriptResource, m_pMaterial, RenderGroup::RenderGroup_Actor, m_pTransform ) );

   return pSKMeshSceneNode;
   }

bool SkeletalMeshRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }

   // Set mesh file path
   m_pSkeletalMeshResource->VInit( pData->FirstChildElement( "SkeletalMesh" ) );

   m_pAnimScriptResource->VInit( pData->FirstChildElement( "AnimationScript" ) );

   m_pMaterial->Init( pData->FirstChildElement( "Material" ) );

   TiXmlElement* pPhysicsAttributes = pData->FirstChildElement( "PhysicsAttributes" );
   if( pPhysicsAttributes && m_pPhysicsAttributes )
      {
      m_pPhysicsAttributes->VInit( pPhysicsAttributes );
      }

   m_pPhysicsAttributes->VSetTransform( *m_pTransform );

   return true;
   }

void SkeletalMeshRenderComponent::VDelegatePostInit( void )
   {
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( m_pSkeletalMeshResource );
   auto pMesh = pAiScene->mMeshes[ 0 ];

   auto pShpereAttr = static_pointer_cast< BulletSpherePhysicsAttributes >( m_pPhysicsAttributes );
   pShpereAttr->SetRadius( this->m_pSceneNode->VGetProperties().GetRadius() );
   m_pPhysicsAttributes->VAddRigidBody( VGetSelfStrongRenderCompPtr() );
   }

void SkeletalMeshRenderComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement )
   {
   TiXmlElement* pMesh = m_pSkeletalMeshResource->VGenerateXML();
   pMesh->SetValue( "SkeletalMesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pAnim = m_pAnimScriptResource->VGenerateXML();
   pAnim->SetValue( "AnimationScript" );
   pBaseElement->LinkEndChild( pAnim );

   TiXmlElement* pMaterial = m_pMaterial->GenerateXML();
   pBaseElement->LinkEndChild( pMaterial );
   }

void SkeletalMeshRenderComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {
   TiXmlElement* pMesh = m_pSkeletalMeshResource->VGenerateOverridesXML( pResourceNode->FirstChildElement( "SkeletalMesh" ) );
   pMesh->SetValue( "SkeletalMesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pMaterial = m_pMaterial->GenerateOverridesXML( pResourceNode->FirstChildElement( "Material" ) );
   pBaseElement->LinkEndChild( pMaterial );
   }