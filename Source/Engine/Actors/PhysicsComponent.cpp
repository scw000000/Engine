/*!
 * \file PhysicsComponent.cpp
 * \date 2016/07/08 9:51
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
#include "PhysicsComponent.h"
#include "..\Physics\PhysicsAttributes.h"

const ComponentRegisterId BaseActorComponent<PhysicsComponent>::s_ComponentId = 0x84646be0;
const std::string BaseActorComponent<PhysicsComponent>::s_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent( void )
   {
   }

shared_ptr<SceneNode> PhysicsComponent::VCreateSceneNode( void )
   {
   return shared_ptr<SceneNode>( ENG_NEW SceneNode( m_pOwner->GetId(), this, RenderPass::RenderPass_Actor, m_pTransform ) );
   }

bool PhysicsComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pPhysicsNode = pData->FirstChildElement( "PhysicsAttributes" );
   if( !pPhysicsNode ) 
      {
      return false;
      }
   TiXmlElement* pShapeNode = pPhysicsNode->FirstChildElement( "ShapeData" );
   if( pShapeNode && !m_pPhysicsAttributes ) // first time init
      {
      std::string shape = pShapeNode->FirstAttribute()->Value();
      if( !shape.compare( "" )  )
         {
         return false;
         }
      m_pPhysicsAttributes = BulletPhysicsAttributes::GetInstanceFromShape( shape );
      }
   m_pPhysicsAttributes->Vinit( pPhysicsNode );

   if( m_pPhysicsAttributes )
      {
      m_pPhysicsAttributes->VSetTransform( *m_pTransform );
      }

   return true;
   }

void PhysicsComponent::VDelegatePostInit( void )
   {
   m_pPhysicsAttributes->VAddRigidBody( VGetSelfStrongRenderCompPtr() );

   }

void PhysicsComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement )
   {
   pBaseElement->LinkEndChild( ENG_NEW TiXmlElement( "PhysicsAttributes" ) );
   }

void PhysicsComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {

   }
