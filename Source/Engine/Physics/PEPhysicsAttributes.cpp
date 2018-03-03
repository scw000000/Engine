/*!
 * \file PEPhysicsAttributes.cpp
 * \date 2017/09/11 13:02
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief s
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "PEPhysics.h"
#include "Colliders.h"
#include "PEPhysicsAttributes.h"
#include "BulletPhysicsAttributes.h"
#include "RigidBody.h"

shared_ptr<ICollider> PESphereColliderAttributes::VCreateCollider( StrongRenderComponentPtr pRenderComp, shared_ptr< IPhysicsAttributes > pPhtsicsAttr )
   {
   float radius = pRenderComp->VGetTransformPtr()->GetScale().x * m_Radius;
   float radiusSq = radius * radius;
   float mass = 4.f * ENG_PI * radiusSq * radius * pPhtsicsAttr->VGetDensity() / 3.f;
   auto pCollider = shared_ptr<ICollider>( ENG_NEW SphereCollider( radius ) );
   pCollider->VSetMass( mass );
   
   float inertia = 0.4f * mass * radiusSq;
   pCollider->VSetInertia( Mat3x3( Vec3( inertia, 0.f, 0.f ), Vec3( 0.f, inertia, 0.f ), Vec3( 0.f, 0.f, inertia ) ) );
   return pCollider;
   }

shared_ptr<ICollider> PEBoxColliderAttributes::VCreateCollider( StrongRenderComponentPtr pRenderComp, shared_ptr< IPhysicsAttributes > pPhtsicsAttr )
   {
   auto dimenstionWS = pRenderComp->VGetTransformPtr()->GetScale() * m_Dimension;
   // The input should be half size of the box
   float mass = dimenstionWS.x * dimenstionWS.y * dimenstionWS.z * pPhtsicsAttr->VGetDensity();
   auto pCollider = shared_ptr<ICollider>( ENG_NEW BoxCollider( dimenstionWS * 0.5f ) );
   pCollider->VSetMass( mass );
   float scalar = mass / 12.f;
   pCollider->VSetInertia( Mat3x3( Vec3( scalar * ( dimenstionWS.y * dimenstionWS.y + dimenstionWS.z * dimenstionWS.z ), 0.f, 0.f )
      , Vec3( 0.f, scalar * ( dimenstionWS.x * dimenstionWS.x + dimenstionWS.z * dimenstionWS.z ), 0.f )
      , Vec3( 0.f, 0.f, scalar * ( dimenstionWS.y * dimenstionWS.y + dimenstionWS.x * dimenstionWS.x ) ) ) );
   return pCollider;
   }

PEPhysicsAttributes::PEPhysicsAttributes( void ) : m_TransLateFactor( Vec3::g_Identity ), m_RotateFactor( Vec3::g_Identity )
   {
   m_IsActive = true;
   m_CollisionId = 1; // Must be selectable by ray test in PE physics
   m_Acceleration = 0.f;
   m_MaxVelocity = 0.f;

   m_AngularAcceleration = 0;
   m_MaxAngularVelocity = 0;

 //  m_Shape = "unknown";
   m_Density = 0.f;
   // m_Material = "None";

//   m_pRigidBody = NULL;
   m_IsLinkedToPhysicsWorld = false;
   m_CollisionFlags = 0;

   m_GravityScale = 1.f;
   }

// This function does not work well when there's a overriding xml file
shared_ptr< IPhysicsAttributes > PEPhysicsAttributes::GetInstanceFromShape( TiXmlElement* pData )
   {
   auto pFirstShapeNode = pData->FirstChildElement( "ShapeData" );
   if( !pFirstShapeNode ) // first time init
      {
      return shared_ptr< IPhysicsAttributes >();
      }

   shared_ptr< PEPhysicsAttributes > retAttr( ENG_NEW PEPhysicsAttributes() );
   if( !retAttr || ( retAttr && !retAttr->VInit( pData ) ) ) // not supported shape or Init failed
      {
      return shared_ptr< IPhysicsAttributes >();
      }

   for( auto pShapeNode = pFirstShapeNode; pShapeNode; pShapeNode = pShapeNode->NextSiblingElement() )
      {
      if( std::string( pShapeNode->Value() ).compare( "ShapeData" ) )
         {
         break;
         }
      auto pShapeAttr = pShapeNode->Attribute( "shape" );
      if( !pShapeAttr )
         {
         return shared_ptr< IPhysicsAttributes >();
         }
      std::string shapeName( pShapeAttr );
      if( !shapeName.compare( "sphere" ) )
         {
         auto newColliderAttribute = shared_ptr<PESphereColliderAttributes>( ENG_NEW PESphereColliderAttributes() );
         retAttr->m_ColliderAttributes.push_back( newColliderAttribute );
         newColliderAttribute->m_ColliderType = ColliderType_Sphere;
         if( pShapeNode->QueryFloatAttribute( "radius", &newColliderAttribute->m_Radius ) != TIXML_SUCCESS
             || newColliderAttribute->m_Radius <= 0.f )
            {
            newColliderAttribute->m_Radius = 1.f;
            return false;
            }
         }
      else if( !shapeName.compare( "box" ) )
         {
         auto newColliderAttribute = shared_ptr<PEBoxColliderAttributes>( ENG_NEW PEBoxColliderAttributes() );
         retAttr->m_ColliderAttributes.push_back( newColliderAttribute );

         newColliderAttribute->m_ColliderType = ColliderType_Box;
         newColliderAttribute->m_Dimension.Init( pShapeNode );
         if( newColliderAttribute->m_Dimension.x <= 0.f || newColliderAttribute->m_Dimension.y <= 0.f || newColliderAttribute->m_Dimension.z <= 0.f )
            {
            newColliderAttribute->m_Dimension = Vec3::g_Identity;
            }
         }
     }
  
   return retAttr;
   }

bool PEPhysicsAttributes::VInit( TiXmlElement* pData )
   {
   TiXmlElement* pMovement = pData->FirstChildElement( "Movement" );
   if( pMovement )
      {
      // Warning: this is not implemented yet
      pMovement->QueryBoolAttribute( "active", &m_IsActive );
      TiXmlElement* pTemp = pMovement->FirstChildElement( "Translate" );
      if( pTemp )
         {
         m_TransLateFactor.Init( pTemp );
         }
      pTemp = pMovement->FirstChildElement( "Rotate" );
      if( pTemp )
         {
         m_RotateFactor.Init( pTemp );
         }
      pTemp = pMovement->FirstChildElement( "Collision" );
      if( pTemp )
         {
         if( pTemp->Attribute( "collisiongroup" ) )
            {
            // Need to implement LoadXml in PEPhysics first
           // m_CollisionId = CollisionTable::GetSingleton().GetIdFromName( pTemp->Attribute( "collisiongroup" ) );
            }
         }
      pTemp = pMovement->FirstChildElement( "GravityScale" );
      if( pTemp )
         {
         pTemp->QueryFloatAttribute( "value", &m_GravityScale );
         }
      
      }

   TiXmlElement* pSubstance = pData->FirstChildElement( "SubstanceData" );
   if( pSubstance )
      {
      if( pSubstance->Attribute( "density" ) )
         {
         std::string densityStr = pSubstance->Attribute( "density" );
         IGamePhysics::GetSingleton().QueryDensity( densityStr, m_Density );
         }

      if( pSubstance->Attribute( "material" ) )
         {
         std::string materialStr = pSubstance->Attribute( "material" );
         IGamePhysics::GetSingleton().QueryMaterialData( materialStr, m_Material );
         // m_Material = pSubstance->Attribute( "material" );
         }
      }
   //TiXmlElement* pShape = pData->FirstChildElement( "ShapeData" );
   //if( pShape )
   //   {
   //   // m_Shape = pShape->Attribute( "shape" );
   //   if( !VDelegateInit( pShape ) )
   //      {
   //      return false;
   //      }
   //   }

   TiXmlElement* pCollisionFlags = pData->FirstChildElement( "CollisionFlags" );
   bool flagAttr = false;
   if( pCollisionFlags )
      {
      TiXmlElement* pFlagSetting = pCollisionFlags->FirstChildElement( "StaticObject" );
      bool setting = false;
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_STATIC_OBJECT;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "KinematicObject" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "NoContactResponse" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "CustomMaterialCallback" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "CharacterObject" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_CHARACTER_OBJECT;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "DisableVisualizeObject" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
         }

      pFlagSetting = pCollisionFlags->FirstChildElement( "DisableSpuCollisionProcessing" );
      if( pFlagSetting && pFlagSetting->QueryBoolAttribute( "enable", &setting ) == TIXML_SUCCESS && setting )
         {
         m_CollisionFlags |= btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING;
         }
      }

   return true;
   }

bool PEPhysicsAttributes::VDelegateInit( TiXmlElement* pData )
   {
   return true;
   }

void PEPhysicsAttributes::VSetTransform( const Transform& transform )
   {
   if( !m_IsLinkedToPhysicsWorld )
      {
      return;
      }
   m_pRigidBody->VSetWorldTransform( transform );;
 //  btTransform newTransform = Transform_to_btTransform( transform );
   // m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
//   m_pRigidBody->setWorldTransform( newTransform );

//   RenderCompMotionState * const renderCompMotionState = static_cast< RenderCompMotionState* >( m_pRigidBody->getMotionState() );
 //  renderCompMotionState->setWorldTransform( newTransform );
   VSetScale( transform.GetScale() );
   //m_pRigidBody->activate();
   }

void PEPhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<IPhysicsAttributes> pPhysicsAttr )
   {
   ENG_ASSERT(pRenderComp);
   m_pRigidBody = shared_ptr<RigidBody>( ENG_NEW RigidBody() );
   for( auto& pColliderAttr : m_ColliderAttributes )
      {
      auto pNewCollider = pColliderAttr->VCreateCollider( pRenderComp, pPhysicsAttr );
      m_pRigidBody->VAddCollider( pNewCollider );
      pNewCollider->VSetRigidBody(m_pRigidBody );
      }
   m_pRigidBody->VSetWorldTransform( *pRenderComp->VGetTransformPtr() );

   m_pRigidBody->VSetTranslateFactor( m_TransLateFactor );
   m_pRigidBody->VSetRotateFactor( m_RotateFactor );
   m_pRigidBody->VSetGravityScale( m_GravityScale );
   PEPhysics::GetSingleton().VAddRigidBody( pRenderComp, m_pRigidBody );
   /*IGamePhysics::GetSingleton().VAddSphere( pRenderComp->VGetTransformPtr()->GetScale().x * m_Radius,
                                            pRenderComp );*/
   }

TiXmlElement* PEPhysicsAttributes::VGenerateXML( void ) const
   {
   TiXmlElement* pRootNode = ENG_NEW TiXmlElement( "PhysicsAttributes" );

   TiXmlElement* pMovementNode = ENG_NEW TiXmlElement( "Movement" );
   pRootNode->LinkEndChild( pMovementNode );
   XMLHelper::SetAttribute( pMovementNode, "active", m_IsActive );

   TiXmlElement* pTranslate = m_TransLateFactor.GenerateXML();
   pMovementNode->LinkEndChild( pTranslate );
   pTranslate->SetValue( "Translate" );

   TiXmlElement* pRotate = m_RotateFactor.GenerateXML();
   pMovementNode->LinkEndChild( pRotate );
   pRotate->SetValue( "Rotate" );

   VDelegateGenerateXML( pRootNode );

   TiXmlElement* pMaterial = ENG_NEW TiXmlElement( "Collision" );
   pMovementNode->LinkEndChild( pMaterial );
   /*pMaterial->SetAttribute( "density", m_Density.c_str() );
   pMaterial->SetAttribute( "material", m_Material.c_str() );*/
   pMaterial->SetAttribute( "collisiongroup", CollisionTable::GetSingleton().GetNameFromId( m_CollisionId ).c_str() );

   TiXmlElement* pCollisionFlags = ENG_NEW TiXmlElement( "CollisionFlags" );
   pRootNode->LinkEndChild( pCollisionFlags );

   TiXmlElement* pFlagSetting = ENG_NEW TiXmlElement( "StaticObject" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_STATIC_OBJECT ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "KinematicObject" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_KINEMATIC_OBJECT ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "NoContactResponse" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_NO_CONTACT_RESPONSE ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "CustomMaterialCallback" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "CharacterObject" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_CHARACTER_OBJECT ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "DisableVisualizeObject" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT ) != 0 );

   pFlagSetting = ENG_NEW TiXmlElement( "DisableSpuCollisionProcessing" );
   pCollisionFlags->LinkEndChild( pFlagSetting );
   XMLHelper::SetAttribute( pFlagSetting, "enable", ( m_CollisionFlags & btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING ) != 0 );

   return pRootNode;
   }
