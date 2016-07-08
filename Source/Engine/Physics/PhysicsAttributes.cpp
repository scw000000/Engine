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

shared_ptr< IPhysicsAttributes > BulletPhysicsAttributes::GetInstanceFromShape( const std::string& shape )
   {
   if( !shape.compare( "sphere" ) )
      {
      return shared_ptr< IPhysicsAttributes >( ENG_NEW BulletSpherePhysicsAttributes() );
      }
   else if( !shape.compare( "box" ) )
      {
      return shared_ptr< IPhysicsAttributes >( ENG_NEW BulletBoxPhysicsAttributes() );
      }
   else
      {
      return shared_ptr< IPhysicsAttributes >();
      }
   }

bool BulletPhysicsAttributes::Vinit( TiXmlElement* pData )
   {
   // shape

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
      }

   TiXmlElement* pShape = pData->FirstChildElement( "ShapeData" );
   if( pShape )
      {
      m_Shape = pShape->Attribute( "shape" );
      }

   // density
   TiXmlElement* pDensity = pData->FirstChildElement( "Density" );
   if( pDensity )
      m_Density = pDensity->GetText();

   // material
   TiXmlElement* pMaterial = pData->FirstChildElement( "PhysicsMaterial" );
   if( pMaterial )
      m_Material = pMaterial->GetText();

   // material
   TiXmlElement* pCollisionGroup = pData->FirstChildElement( "CollisionGroup" );
   if( pCollisionGroup )
      m_CollisionId = CollisionTable::GetSingleton().GetIdFromName( pCollisionGroup->GetText() );


   TiXmlElement* pCollisionFlags = pData->FirstChildElement( "CollisionFlags" );
   bool flagAttr = false;
   if( pCollisionFlags )
      {
      if( pCollisionFlags->QueryBoolAttribute( "staticobject", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_STATIC_OBJECT;
         }
      if( pCollisionFlags->QueryBoolAttribute( "kinematicobject", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
         }
      if( pCollisionFlags->QueryBoolAttribute( "nocontactresponse", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
         }
      if( pCollisionFlags->QueryBoolAttribute( "custommaterialcallback", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
         }
      if( pCollisionFlags->QueryBoolAttribute( "characterobject", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_CHARACTER_OBJECT;
         }
      if( pCollisionFlags->QueryBoolAttribute( "disablervisualizeobject", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
         }
      if( pCollisionFlags->QueryBoolAttribute( "disablespucollisionprocessing", &flagAttr ) == TIXML_SUCCESS && flagAttr )
         {
         m_CollisionFlags |= btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING;
         }
      }
   return VDelegateInit( pData );
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


   VDelegateGenerateXML( pRootNode );
   return pRootNode;
   }

BulletSpherePhysicsAttributes::BulletSpherePhysicsAttributes( void )
   {
   m_Radius = 1.f;
   }

bool BulletSpherePhysicsAttributes::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pShapeData = pData->FirstChildElement( "ShapeData" );
   if( !pShapeData )
      {
      return false;
      }
   if( pShapeData->QueryFloatAttribute( "radius", &m_Radius ) != TIXML_SUCCESS || m_Radius <= 0.f )
      {
      m_Radius = 1.f;
      return false;
      }
   return true;
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
   TiXmlElement* pShapeData = pData->FirstChildElement( "ShapeData" );
   if( !pShapeData )
      {
      return false;
      }
   m_Size.Init( pShapeData );
   if( m_Size.x <= 0.f || m_Size.y <= 0.f || m_Size.z <= 0.f )
      {
      m_Size = Vec3::g_Identity;
      return false;
      }
   return true;
   }

void BulletBoxPhysicsAttributes::VSetScale( const Vec3& scale )
   {
   ENG_LOG( "Test", ToStr( scale ) );
   auto pBoxShape = dynamic_cast< btBoxShape* >( m_pRigidBody->getCollisionShape() );
   ENG_ASSERT( pBoxShape );
   pBoxShape->setImplicitShapeDimensions( Vec3_to_btVector3( m_Size * scale ) );   
   }

void BulletBoxPhysicsAttributes::VAddRigidBody( StrongRenderComponentPtr pRenderComp )
   {
   IGamePhysics::GetSingleton().VAddBox( pRenderComp->VGetTransformPtr()->GetScale() * m_Size,
                                         pRenderComp);
   }