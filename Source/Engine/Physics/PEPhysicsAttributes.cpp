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

shared_ptr<ICollider> PESphereColliderAttributes::VCreateCollider( StrongRenderComponentPtr pRenderComp )
   {
   auto pCollider = shared_ptr<ICollider>( ENG_NEW SphereCollider(
      pRenderComp->VGetTransformPtr()->GetScale().x * m_Radius) );
   pCollider->SetInertia( Mat3x3::g_Identity );
   pCollider->SetMass( 1.f );
   return pCollider;
   }

shared_ptr<ICollider> PEBoxColliderAttributes::VCreateCollider( StrongRenderComponentPtr pRenderComp )
   {
   // The input should be half size of the box
   auto pCollider = shared_ptr<ICollider>( ENG_NEW BoxCollider(
      pRenderComp->VGetTransformPtr()->GetScale() * m_Dimension * 0.5f ) );
   pCollider->SetInertia( Mat3x3::g_Identity );
   pCollider->SetMass( 1.f );
   return pCollider;
   }

PEPhysicsAttributes::PEPhysicsAttributes( void ) // : m_TransLateFactor( Vec3::g_Identity ), m_RotateFactor( Vec3::g_Identity )
   {
   m_IsActive = true;
   m_CollisionId = 1; // Must be selectable by ray test in PE physics
   m_Acceleration = 0.f;
   m_MaxVelocity = 0.f;

   m_AngularAcceleration = 0;
   m_MaxAngularVelocity = 0;

 //  m_Shape = "unknown";
//   m_Density = "empty";
 //  m_Material = "None";

//   m_pRigidBody = NULL;
   m_IsLinkedToPhysicsWorld = false;
   m_CollisionFlags = 0;
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
         pShapeNode->QueryFloatAttribute( "density", &newColliderAttribute->m_Density );

         if( pShapeNode->Attribute( "material" ) )
            {
            newColliderAttribute->m_Material = pShapeNode->Attribute( "material" );
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
         pShapeNode->QueryFloatAttribute( "density", &newColliderAttribute->m_Density );

         if( pShapeNode->Attribute( "material" ) )
            {
            newColliderAttribute->m_Material = pShapeNode->Attribute( "material" );
            }
         }
     }
  
   if( !retAttr || ( retAttr && !retAttr->VInit( pData ) ) ) // not supported shape or Init failed
      {
      return shared_ptr< IPhysicsAttributes >();
      }
   return retAttr;
   }

bool PEPhysicsAttributes::VInit( TiXmlElement* pData )
   {
   TiXmlElement* pMovement = pData->FirstChildElement( "Movement" );
   if( pMovement )
      {
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
   m_pRigidBody->SetWorldTransform( transform );;
 //  btTransform newTransform = Transform_to_btTransform( transform );
   // m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
//   m_pRigidBody->setWorldTransform( newTransform );

//   RenderCompMotionState * const renderCompMotionState = static_cast< RenderCompMotionState* >( m_pRigidBody->getMotionState() );
 //  renderCompMotionState->setWorldTransform( newTransform );
   VSetScale( transform.GetScale() );
   //m_pRigidBody->activate();
   }

void PEPhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT(pRenderComp);
   this->m_pRigidBody = shared_ptr<RigidBody>( ENG_NEW RigidBody() );
   for( auto& pColliderAttr : this->m_ColliderAttributes )
      {
      auto pNewCollider = pColliderAttr->VCreateCollider( pRenderComp );
      this->m_pRigidBody->AddCollider( pNewCollider );
      pNewCollider->SetRigidBody(m_pRigidBody );
      }

   PEPhysics::GetSingleton().VAddRigidBody( pRenderComp, this->m_pRigidBody );
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

PESpherePhysicsAttributes::PESpherePhysicsAttributes( void )
   {
   m_Radius = 1.f;
   }

bool PESpherePhysicsAttributes::VDelegateInit( TiXmlElement* pData )
   {
   std::string shapeName = pData->Attribute( "shape" );
   if( shapeName.compare( "sphere" ) )
      {
      return false;
      }
   if( pData->QueryFloatAttribute( "radius", &m_Radius ) != TIXML_SUCCESS || m_Radius <= 0.f )
      {
      m_Radius = 1.f;
      return false;
      }
   return true;
   }

void PESpherePhysicsAttributes::VDelegateGenerateXML( TiXmlElement* pParent ) const
   {
   TiXmlElement* pRoot = ENG_NEW TiXmlElement( "ShapeData" );
   pParent->LinkEndChild( pRoot );
   pRoot->SetAttribute( "shape", "sphere" );
   XMLHelper::SetAttribute( pRoot, "radius", m_Radius );
   }

void PESpherePhysicsAttributes::VSetScale( const Vec3& scale )
   {
//   auto pShpereShape = dynamic_cast< btSphereShape* >( m_pRigidBody->getCollisionShape() );
//   ENG_ASSERT( pShpereShape );
//   pShpereShape->setUnscaledRadius( m_Radius * scale.x );
   }

void PESpherePhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   IGamePhysics::GetSingleton().VAddSphere( pRenderComp->VGetTransformPtr()->GetScale().x * m_Radius,
                                            pRenderComp );
   }

PEBoxPhysicsAttributes::PEBoxPhysicsAttributes( void ) : m_Size( Vec3::g_Identity )
   {

   }

bool PEBoxPhysicsAttributes::VDelegateInit( TiXmlElement* pData )
   {
   std::string shapeName = pData->Attribute( "shape" );
   if( shapeName.compare( "box" ) )
      {
      return false;
      }
   m_Size.Init( pData );
   if( m_Size.x <= 0.f || m_Size.y <= 0.f || m_Size.z <= 0.f )
      {
      m_Size = Vec3::g_Identity;
      return false;
      }
   return true;
   }

void PEBoxPhysicsAttributes::VDelegateGenerateXML( TiXmlElement* pParent ) const
   {
   TiXmlElement* pRoot = ENG_NEW TiXmlElement( "ShapeData" );
   pParent->LinkEndChild( pRoot );
   pRoot->SetAttribute( "shape", "box" );
   XMLHelper::SetAttribute( pRoot, m_Size );
   }

void PEBoxPhysicsAttributes::VSetScale( const Vec3& scale )
   {
//   auto pBoxShape = dynamic_cast< btBoxShape* >( m_pRigidBody->getCollisionShape() );
//   ENG_ASSERT( pBoxShape );
//s   pBoxShape->setImplicitShapeDimensions( Vec3_to_btVector3( m_Size * scale ) );
   }

void PEBoxPhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   IGamePhysics::GetSingleton().VAddBox( pRenderComp->VGetTransformPtr()->GetScale() * m_Size,
                                         pRenderComp );
   }