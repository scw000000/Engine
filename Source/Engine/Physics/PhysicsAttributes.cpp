/*!
 * \file PhysicsAttributes.cpp
 * \date 2016/06/27 21:46
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
#include "PhysicsAttributes.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

BulletPhysicsAttributes::BulletPhysicsAttributes( void ) : m_TransLateFactor( Vec3::g_Identity ), m_RotateFactor( Vec3::g_Identity )
   {
   m_IsActive = true;
   m_CollisionId = 1; // Must be selectable by ray test in bullet physics
   m_Acceleration = 0.f;
   m_MaxVelocity = 0.f;

   m_AngularAcceleration = 0;
   m_MaxAngularVelocity = 0;

   m_Shape = "unknown";
   m_Density = "empty";
   m_Material = "None";

   m_pRigidBody = NULL;
   m_IsLinkedToPhysicsWorld = false;
   m_CollisionFlags = 0;
   }

shared_ptr< IPhysicsAttributes > BulletPhysicsAttributes::GetInstanceFromShape( TiXmlElement* pData )
   {
   TiXmlElement* pShapeNode = pData->FirstChildElement( "ShapeData" );

   if( !pShapeNode ) // first time init
      {
      return shared_ptr< IPhysicsAttributes >();
      }
   shared_ptr< IPhysicsAttributes > retAttr;
   std::string shapeName = pShapeNode->Attribute( "shape" );
   if( !shapeName.compare( "sphere" ) )
      {
       retAttr = shared_ptr< IPhysicsAttributes >( ENG_NEW BulletSpherePhysicsAttributes() );
      }
   else if( !shapeName.compare( "box" ) )
      {
      retAttr = shared_ptr< IPhysicsAttributes >( ENG_NEW BulletBoxPhysicsAttributes() );
      }

   if( !retAttr || ( retAttr && !retAttr->VInit( pData ) ) ) // not supported shape or Init failed
      {
      return shared_ptr< IPhysicsAttributes >();
      }
   return retAttr;
   }

bool BulletPhysicsAttributes::VInit( TiXmlElement* pData )
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
         if( pTemp->Attribute( "density" ) )
            {
            m_Density = pTemp->Attribute( "density" );
            }
         if( pTemp->Attribute( "material" ) )
            {
            m_Material = pTemp->Attribute( "material" );
            }
         if( pTemp->Attribute( "collisiongroup" ) )
            {
            m_CollisionId = CollisionTable::GetSingleton().GetIdFromName( pTemp->Attribute( "collisiongroup" ) );
            }
         }
      }

   TiXmlElement* pShape = pData->FirstChildElement( "ShapeData" );
   if( pShape )
      {
      m_Shape = pShape->Attribute( "shape" );
      if( !VDelegateInit( pShape ) )
         {
         return false;
         }
      }

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

bool BulletPhysicsAttributes::VDelegateInit( TiXmlElement* pData )
   {
   return true;
   }

void BulletPhysicsAttributes::VSetTransform( const Transform& transform )
   {
   if( !m_IsLinkedToPhysicsWorld )
      {
      return;
      }
   btTransform newTransform = Transform_to_btTransform( transform );
  // m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
   m_pRigidBody->setWorldTransform( newTransform );

   RenderCompMotionState * const renderCompMotionState = static_cast< RenderCompMotionState* >( m_pRigidBody->getMotionState() );
   renderCompMotionState->setWorldTransform( newTransform );
   VSetScale( transform.GetScale() );
   //m_pRigidBody->activate();
   }

TiXmlElement* BulletPhysicsAttributes::VGenerateXML( void ) const 
   {
   TiXmlElement* pRootNode = ENG_NEW TiXmlElement( "PhysicsAttributes" );

   TiXmlElement* pMovementNode = ENG_NEW TiXmlElement( "Movement" );
   pRootNode->LinkEndChild( pMovementNode );
   XMLHelper::SetAttribute( pMovementNode, "active", m_IsActive );

   TiXmlElement* pTranslate = m_TransLateFactor.GernerateXML();
   pMovementNode->LinkEndChild( pTranslate );
   pTranslate->SetValue( "Translate" );

   TiXmlElement* pRotate = m_RotateFactor.GernerateXML();
   pMovementNode->LinkEndChild( pRotate );
   pRotate->SetValue( "Rotate" );

   VDelegateGenerateXML( pRootNode );

   TiXmlElement* pMaterial = ENG_NEW TiXmlElement( "Collision" );
   pMovementNode->LinkEndChild( pMaterial );
   pMaterial->SetAttribute( "density", m_Density.c_str() );
   pMaterial->SetAttribute( "material", m_Material.c_str() );
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

TiXmlElement* BulletPhysicsAttributes::VGenerateOverridesXML( TiXmlElement* pResourceNode ) const
   {
   auto pBase = VGenerateXML();
   XMLHelper::GenerateOverride( pBase, pResourceNode ); 
   return pBase;
   }

BulletSpherePhysicsAttributes::BulletSpherePhysicsAttributes( void )
   {
   m_Radius = 1.f;
   }

bool BulletSpherePhysicsAttributes::VDelegateInit( TiXmlElement* pData )
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

void BulletSpherePhysicsAttributes::VDelegateGenerateXML( TiXmlElement* pParent ) const
   {
   TiXmlElement* pRoot = ENG_NEW TiXmlElement( "ShapeData" );
   pParent->LinkEndChild( pRoot );
   pRoot->SetAttribute( "shape", "sphere" );
   XMLHelper::SetAttribute( pRoot, "radius", m_Radius );
   }

void BulletSpherePhysicsAttributes::VSetScale( const Vec3& scale )
   {
   auto pShpereShape = dynamic_cast< btSphereShape* >( m_pRigidBody->getCollisionShape() );
   ENG_ASSERT( pShpereShape );
   pShpereShape->setUnscaledRadius( m_Radius * scale.x );
   }

void BulletSpherePhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   IGamePhysics::GetSingleton().VAddSphere( pRenderComp->VGetTransformPtr()->GetScale().x * m_Radius,
                                            pRenderComp);
   }

BulletBoxPhysicsAttributes::BulletBoxPhysicsAttributes( void ) : m_Size( Vec3::g_Identity )
   {
   
   }

bool BulletBoxPhysicsAttributes::VDelegateInit( TiXmlElement* pData )
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

void BulletBoxPhysicsAttributes::VDelegateGenerateXML( TiXmlElement* pParent ) const
   {
   TiXmlElement* pRoot = ENG_NEW TiXmlElement( "ShapeData" );
   pParent->LinkEndChild( pRoot );
   pRoot->SetAttribute( "shape", "box" );
   XMLHelper::SetAttribute( pRoot, m_Size );
   }

void BulletBoxPhysicsAttributes::VSetScale( const Vec3& scale )
   {
   auto pBoxShape = dynamic_cast< btBoxShape* >( m_pRigidBody->getCollisionShape() );
   ENG_ASSERT( pBoxShape );
   pBoxShape->setImplicitShapeDimensions( Vec3_to_btVector3( m_Size * scale ) );   
   }

void BulletBoxPhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   IGamePhysics::GetSingleton().VAddBox( pRenderComp->VGetTransformPtr()->GetScale() * m_Size,
                                         pRenderComp);
   }