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
#include "..\Physics\PEPhysicsAttributes.h"
#include "..\Physics\PhysicsAttributes.h"

const ComponentRegisterId BaseActorComponent<PhysicsComponent>::s_ComponentId = 0x84646be0;
const std::string BaseActorComponent<PhysicsComponent>::s_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent( void )
   {
   }

shared_ptr<SceneNode> PhysicsComponent::VCreateSceneNode( void )
   {
   return shared_ptr<SceneNode>( ENG_NEW SceneNode( m_pOwner->GetId(), this, RenderGroup::RenderGroup_Actor, m_pTransform ) );
   }

bool PhysicsComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pPhysicsNode = pData->FirstChildElement( "PhysicsAttributes" );
   if( !pPhysicsNode ) 
      {
      return false;
      }

   if( !m_pPhysicsAttributes ) // first time init
      {
      m_pPhysicsAttributes = PEPhysicsAttributes::GetInstanceFromShape( pPhysicsNode );
      if( !m_pPhysicsAttributes )
         {
         return false;
         }
      }

   m_pPhysicsAttributes->VSetTransform( *m_pTransform );

   return true;
   }

void PhysicsComponent::VDelegatePostInit( void )
   {
   m_pPhysicsAttributes->VAddRigidBody( VGetSelfStrongRenderCompPtr(), m_pPhysicsAttributes );

   }

void PhysicsComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement )
   {
   pBaseElement->LinkEndChild( m_pPhysicsAttributes->VGenerateXML() );
   }

void PhysicsComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {
   pBaseElement->LinkEndChild( m_pPhysicsAttributes->VGenerateOverridesXML( pResourceNode->FirstChildElement( "PhysicsAttributes" ) ) );
   }
