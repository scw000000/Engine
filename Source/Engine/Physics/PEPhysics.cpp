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
#include "..\Graphics\BasicGeometry.h"

PEPhysics::~PEPhysics( void )
   {

   }

PEPhysics::PEPhysics( void )
   {

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
 
   }

void PEPhysics::VRenderDiagnostics( void )
   {
   auto pScene = g_pApp->m_pEngineLogic->m_pWrold;
   auto v = pScene->GetCamera()->GetView();
   auto p = pScene->GetCamera()->GetProjection();
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Box, g_Red, p * v );
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
   return Vec3();
   }

void PEPhysics::VSetVelocity( ActorId actorId, const Vec3& vel )
   {

   }

Vec3 PEPhysics::VGetAngularVelocity( ActorId actorId ) 
   {
   return Vec3();
   }

void PEPhysics::VSetAngularVelocity( ActorId actorId, const Vec3& vel )
   {
   }

void PEPhysics::VTranslate( ActorId actorId, const Vec3& vec )
   {

   }

void PEPhysics::VSetTransform( const ActorId id, const Transform &trans )
   {

   }

Transform PEPhysics::VGetTransform( ActorId actorId ) 
   {
   return Transform();
   }

void PEPhysics::VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp ) 
   {

   }

void PEPhysics::VDestroyRenderCompDelegate( IEventPtr pEvent ) 
   {

   }