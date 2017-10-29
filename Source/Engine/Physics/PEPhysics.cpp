/*!
 * \file PEPhysics.cpp
 * \date 2017/09/16 22:12
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "PEPhysics.h"
#include "RigidBody.h"
#include "PEPhysicsAttributes.h"
#include "..\Graphics\BasicGeometry.h"

PEPhysics::~PEPhysics( void )
   {
   m_RenderCompToRigidBody.clear();
   m_RigidBodyToRenderComp.clear();
   }

PEPhysics::PEPhysics( void )
   {
   m_IsSimulating = false;

   }

bool PEPhysics::VInitialize()
   {
   return true;
   }

void PEPhysics::VSyncVisibleScene()
   {

   }
void PEPhysics::VSyncRigidBodyToRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT( pRenderComp );
   auto renderCompToBodyIt = m_RenderCompToRigidBody.find( pRenderComp );
   ENG_ASSERT( renderCompToBodyIt != m_RenderCompToRigidBody.end() );
   TransformPtr pComponentTransform = pRenderComp->VGetTransformPtr();

   renderCompToBodyIt->second->SetWorldTransform( *pComponentTransform );
   }

void PEPhysics::VOnUpdate( const float deltaSeconds )
   {

   }

// Initialization of Physics Objects
void PEPhysics::VAddSphere( float radius, StrongRenderComponentPtr pRenderComp )
   {

   }

void PEPhysics::VAddBox( const Vec3& dimensions, StrongRenderComponentPtr pRenderComp )
   {
  
   }

void PEPhysics::VAddPointCloud( Vec3 *verts,
                             int numPoints,
                             StrongRenderComponentPtr pRenderComp )
   {
 
   }

void PEPhysics::VRemoveRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT( pRenderComp );
   if( auto pBody = FindRigidBody( pRenderComp ) )
      {
      // destroy the body and all its components
      // RemoveCollisionObject( pBody );
      m_RenderCompToRigidBody.erase( pRenderComp );
      m_RigidBodyToRenderComp.erase( pBody );
      }
   }

void PEPhysics::VRenderDiagnostics( void )
   {
   auto pScene = g_pApp->m_pEngineLogic->m_pWrold;
   auto pv = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView();
   for(auto& mapPair : m_RigidBodyToRenderComp)
      {
      auto m = mapPair.first->m_Transform.GetToWorld();
      for(auto& collider : mapPair.first->m_Colliders ){
         SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Red, m * pv );
         }
      }
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Red, pv );
   } 

void PEPhysics::VCreateTrigger( WeakActorPtr pActor, const Vec3 &pos, const float dim ) 
   {

   }

void PEPhysics::VApplyForce( const Vec3 &dir, float newtons, ActorId actorId ) 
   {

   }

void PEPhysics::VApplyTorque( const Vec3 &dir, float newtons, ActorId actorId ) 
   {

   }

bool PEPhysics::VKinematicMove( const Transform &trans, ActorId actorId ) 
   {
   return true; 
   }

void PEPhysics::VRotateY( ActorId actorId, float angleRadians, float time )
   {
   }

float PEPhysics::VGetOrientationY( ActorId actorId ) 
   {
   return 0.f; 
   }

void PEPhysics::VStopActor( ActorId actorId )
   {

   }

Vec3 PEPhysics::VGetVelocity( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Vec3();
      }
   return pRigidBody->m_LinearVelocity;
   }

void PEPhysics::VSetVelocity( ActorId actorId, const Vec3& vel )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->m_LinearVelocity = vel;
   }

Vec3 PEPhysics::VGetAngularVelocity( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Vec3();
      }
   return pRigidBody->m_AngularVelocity;
   }

void PEPhysics::VSetAngularVelocity( ActorId actorId, const Vec3& vel )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->m_AngularVelocity = vel;
   }

void PEPhysics::VTranslate( ActorId actorId, const Vec3& vec )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }

   }

void PEPhysics::VSetTransform( const ActorId actorId, const Transform &trans )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->SetWorldTransform( trans );
   }

Transform PEPhysics::VGetTransform( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Transform();
      }
   return pRigidBody->m_Transform;
   }

void PEPhysics::VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp ) 
   {
   auto pPhtsicsAttr = pRenderComp->VGetPhysicsAttributes();
   auto pPhyAttr = dynamic_pointer_cast< PEPhysicsAttributes >( pPhtsicsAttr );
   ENG_ASSERT( pPhyAttr );
   shared_ptr<RigidBody> pRigidBody = FindRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   pPhyAttr->VSetRigidBody( pRigidBody );
   pPhyAttr->VSetIsLinkedToPhysicsWorld( true );
   }

void PEPhysics::VDestroyRenderCompDelegate( IEventPtr pEvent ) 
   {

   }

shared_ptr<RigidBody> PEPhysics::FindRigidBody( StrongRenderComponentPtr pRenderComp ) const
   {
   auto pIterater = m_RenderCompToRigidBody.find( pRenderComp );
   if( pIterater != m_RenderCompToRigidBody.end() )
      {
      return pIterater->second;
      }
   return NULL;
   }

shared_ptr<RigidBody> PEPhysics::FindRigidBody( ActorId actorId ) const
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   return FindRigidBody( pRenderComp );
   }

StrongRenderComponentPtr PEPhysics::FindRenderComponent( shared_ptr<RigidBody> pRigidBody ) const
   {
   auto pIterater = m_RigidBodyToRenderComp.find( pRigidBody );
   if( pIterater != m_RigidBodyToRenderComp.end() )
      return pIterater->second;

   return StrongRenderComponentPtr();
   }

void PEPhysics::AddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<RigidBody> pRB )
   {
   if( m_RenderCompToRigidBody.find( pRenderComp ) != m_RenderCompToRigidBody.end()
       || m_RigidBodyToRenderComp.find( pRB ) != m_RigidBodyToRenderComp.end() )
      {
      ENG_ERROR("This render comp is already registed!");
      }

   m_RenderCompToRigidBody[ pRenderComp ] = pRB;
   m_RigidBodyToRenderComp[ pRB ] = pRenderComp;
   }