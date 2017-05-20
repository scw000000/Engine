/*!
 * \file Physics.cpp
 * \date 2016/05/07 10:44
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
#include "Physics.h"

#include "btBulletDynamicsCommon.h"
#include "PhysicsDebugDrawer.h"
#include "CollisionTable.h"
#include "Raycast.h"

#include "..\ResourceCache\XmlResource.h"
#include "..\Event\Events.h"
#include "..\Event\PhysicsEvents.h"
#include "..\Event\EventManager.h"

IGamePhysics* g_pGamePhysics = NULL;

/////////////////////////////////////////////////////////////////////////////
// helpers for conversion to and from Bullet's data types
btVector3 Vec3_to_btVector3( Vec3 const & vec3 )
   {
   return btVector3( vec3.x, vec3.y, vec3.z );
   }

Vec3 btVector3_to_Vec3( btVector3 const & btvec )
   {
   return Vec3( btvec.x( ), btvec.y( ), btvec.z( ) );
   }

btTransform Transform_to_btTransform( Transform const & trans )
   {
   // convert from Mat4x4 (GameCode) to btTransform (Bullet)
   btMatrix3x3 bulletRotation;
   Mat4x4 rot = trans.GetRotationMatrix();

   for( int row = 0; row < 3; ++row )
      {
  //    Vec3 vec = rot.GetRow( row );
      memcpy( &bulletRotation[row][0], &rot[row][0], sizeof( Vec3 ) );
      }
  // return btTransform( btQuaternion( quat.x, quat.y, quat.z, quat.w ), Vec3_to_btVector3( trans.GetToWorldPosition() ) );
   return btTransform( bulletRotation, Vec3_to_btVector3( trans.GetToWorldPosition() ) );
   }

static Transform btTransform_to_Transform( btTransform const & trans )
   {
   Mat4x4 ret = Mat4x4::g_Identity;

   // convert from btTransform (Bullet) to Mat4x4 (GameCode)
   btMatrix3x3 const & bulletRotation = trans.getBasis( );
   btVector3 const & bulletPosition = trans.getOrigin( );

   for( int row = 0; row < 3; ++row )
      {
      //    Vec3 vec = rot.GetRow( row );
      memcpy( &ret[row][0], &bulletRotation[row][0], sizeof( Vec3 ) );
      }

   ret.SetToWorldPosition( btVector3_to_Vec3( bulletPosition ) );
   //// copy rotation matrix
   //for( int row = 0; row < 3; ++row )
   //   for( int column = 0; column < 3; ++column )
   //      ret[row][column] = bulletRotation[column][row];
   //// note the reversed indexing (row/column vs. column/row)
   ////  this is because Mat4x4s are row-major matrices and
   ////  btMatrix3x3 are column-major.  This reversed indexing
   ////  implicitly transposes (flips along the diagonal) 
   ////  the matrix when it is copied.

   //// copy position
   //for( int column = 0; column < 3; ++column )
   //   ret[3][column] = bulletPosition[column];

   return ret;
   }

IGamePhysics::IGamePhysics( void )
   {
   ENG_ASSERT( !g_pGamePhysics );
   g_pGamePhysics = this;
   }

IGamePhysics& IGamePhysics::GetSingleton( void )
   {
   return *g_pGamePhysics;
   }

void IGamePhysics::Shutdown( void )
   {
   SAFE_DELETE( g_pGamePhysics );
   }

BulletPhysics::BulletPhysics()
   {
   static RayCastManager s_RayCastManager( this );
   m_DynamicsWorld = NULL;
   IEventManager::GetSingleton()->VAddListener( fastdelegate::MakeDelegate( this, &BulletPhysics::VDestroyRenderCompDelegate), Event_Destory_Render_Component::s_EventType );
   m_IsSimulating = false;
   // [mrmike] This was changed post-press to add event registration!
   REGISTER_EVENT( EvtData_PhysTrigger_Enter );
   REGISTER_EVENT( EvtData_PhysTrigger_Leave );
   REGISTER_SCRIPT_EVENT( EvtData_PhysCollision );
   REGISTER_EVENT( EvtData_PhysSeparation );
   }


/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::~BulletPhysics				- Chapter 17, page 596
//
BulletPhysics::~BulletPhysics( )
   {
   // delete any physics objects which are still in the world

   // iterate backwards because removing the last object doesn't affect the
   //  other objects stored in a vector-type array
   for( int ii = m_DynamicsWorld->getNumCollisionObjects( ) - 1; ii >= 0; --ii )
      {
      btCollisionObject * const obj = m_DynamicsWorld->getCollisionObjectArray( )[ii];

      RemoveCollisionObject( obj );
      }

   m_RigidBodyToRenderComp.clear( );

   SAFE_DELETE( m_DebugDrawer );
   SAFE_DELETE( m_DynamicsWorld );
   SAFE_DELETE( m_Solver );
   SAFE_DELETE( m_Broadphase );
   SAFE_DELETE( m_Dispatcher );
   SAFE_DELETE( m_CollisionConfig );
   }


/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::LoadXml						- not described in the book
//
//    Loads the physics materials from an XML file
//
void BulletPhysics::LoadXml( )
   {
   // Load the physics config file and grab the root XML node
   Resource physicsRes( "config\\Physics.xml", g_pApp->m_EngineOptions.GetIsUsingDevDirectory() );
   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( physicsRes );
   ENG_ASSERT( pRoot );

   // load all materials
   TiXmlElement* pParentNode = pRoot->FirstChildElement( "PhysicsMaterials" );
   ENG_ASSERT( pParentNode );
   for( TiXmlElement* pNode = pParentNode->FirstChildElement( ); pNode; pNode = pNode->NextSiblingElement( ) )
      {
      double restitution = 0;
      double friction = 0;
      pNode->Attribute( "restitution", &restitution );
      pNode->Attribute( "friction", &friction );
      m_MaterialTable.insert( std::make_pair( pNode->Value( ), MaterialData( ( float ) restitution, ( float ) friction ) ) );
      }

   // load all densities
   pParentNode = pRoot->FirstChildElement( "DensityTable" );
   ENG_ASSERT( pParentNode );
   for( TiXmlElement* pNode = pParentNode->FirstChildElement( ); pNode; pNode = pNode->NextSiblingElement( ) )
      {
      // Insert ( substance name, substance density )
      m_DensityTable.insert( std::make_pair( pNode->Value( ), ( float ) atof( pNode->FirstChild( )->Value( ) ) ) );
      }
   pParentNode = pRoot->FirstChildElement( "CollisionTable" );
   ENG_ASSERT( pParentNode );
   
   ENG_ASSERT( CollisionTable::GetSingleton().Init( pParentNode ) );

   }

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VInitialize					- Chapter 17, page 594
//
bool BulletPhysics::VInitialize( )
   {
   LoadXml( );

   // this controls how Bullet does internal memory management during the collision pass
   m_CollisionConfig = ENG_NEW btDefaultCollisionConfiguration( );

   // this manages how Bullet detects precise collisions between pairs of objects
   m_Dispatcher = ENG_NEW btCollisionDispatcher( m_CollisionConfig );

   // Bullet uses this to quickly (imprecisely) detect collisions between objects.
   //   Once a possible collision passes the broad phase, it will be passed to the
   //   slower but more precise narrow-phase collision detection (btCollisionDispatcher).
   m_Broadphase = ENG_NEW btDbvtBroadphase( );

   // Manages constraints which apply forces to the physics simulation.  Used
   //  for e.g. springs, motors.  We don't use any constraints right now.
   m_Solver = new btSequentialImpulseConstraintSolver;

   // This is the main Bullet interface point.  Pass in all these components to customize its behavior.
   m_DynamicsWorld = new btDiscreteDynamicsWorld( m_Dispatcher,
                                                      m_Broadphase,
                                                      m_Solver,
                                                      m_CollisionConfig );

   m_DebugDrawer = ENG_NEW BulletDebugDrawer;
   m_DebugDrawer->ReadOptions( );

   if( !m_CollisionConfig || !m_Dispatcher || !m_Broadphase ||
       !m_Solver || !m_DynamicsWorld || !m_DebugDrawer )
      {
      ENG_ERROR( "BulletPhysics::VInitialize failed!" );
      return false;
      }

   m_DynamicsWorld->setDebugDrawer( m_DebugDrawer );


   // and set the internal tick callback to our own method "BulletInternalTickCallback"
   m_DynamicsWorld->setInternalTickCallback( BulletInternalTickCallback );
   m_DynamicsWorld->setWorldUserInfo( this );

   return true;
   }

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VOnUpdate						- Chapter 17, page 596
//
void BulletPhysics::VOnUpdate( float const deltaSeconds )
   {
   // Bullet uses an internal fixed timestep (default 1/60th of a second)
   //   We pass in 4 as a max number of sub steps.  Bullet will run the simulation
   //   in increments of the fixed timestep until "deltaSeconds" amount of time has
   //   passed, but will only run a maximum of 4 steps this way.
   if( m_IsSimulating )
      {
      m_DynamicsWorld->stepSimulation( deltaSeconds, 4 );
      }
   }

/////////////////////////////////////////////////////////////////////////////
// BulletPhysics::VSyncVisibleScene				- Chapter 17, page 598
//
void BulletPhysics::VSyncVisibleScene( )
   {
   // Keep physics & graphics in sync

   // check all the existing actor's bodies for changes. 
   //  If there is a change, send the appropriate event for the game system.

   for( auto renderCompIt : m_RenderCompToRigidBody )
      {
      StrongRenderComponentPtr pRenderComp = renderCompIt.first;
      ENG_ASSERT( pRenderComp );

      // get the MotionState.  this object is updated by Bullet.
      // it's safe to cast the btMotionState to ActorMotionState, because all the bodies in m_ActorIdToRigidBody
      //   were created through AddShape()
      RenderCompMotionState const * const renderCompMotionState = static_cast< RenderCompMotionState* >( renderCompIt.second->getMotionState() );
      ENG_ASSERT( renderCompMotionState );

      TransformPtr pComponentTransform = pRenderComp->VGetTransformPtr();
      if( pComponentTransform->GetToWorld() != renderCompMotionState->m_Transform.GetToWorld() )
         {
         Vec3 scale = pComponentTransform->GetScale();
         // Bullet has moved the actor's physics object.  Sync the transform and inform the game an actor has moved
         *pComponentTransform = renderCompMotionState->m_Transform;
         pComponentTransform->SetScale( scale );
         /*shared_ptr<Event_Move_Actor> pEvent( ENG_NEW Event_Move_Actor( id, actorMotionState->m_Transform.GetToWorld() ) );
         IEventManager::GetSingleton()->VQueueEvent( pEvent );*/
         }
      }
   }

void BulletPhysics::VSyncRigidBodyToRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   // get the MotionState.  this object is updated by Bullet.
   // it's safe to cast the btMotionState to ActorMotionState, because all the bodies in m_ActorIdToRigidBody
   //   were created through AddShape()
   ENG_ASSERT( pRenderComp );
   auto renderCompToBodyIt = m_RenderCompToRigidBody.find( pRenderComp );
   ENG_ASSERT( renderCompToBodyIt != m_RenderCompToRigidBody.end() );
   TransformPtr pComponentTransform = pRenderComp->VGetTransformPtr();
   btTransform newTransform = Transform_to_btTransform( *pComponentTransform );

   renderCompToBodyIt->second->setWorldTransform( newTransform );

   RenderCompMotionState * const renderCompMotionState = static_cast< RenderCompMotionState* >( renderCompToBodyIt->second->getMotionState() );
   renderCompMotionState->setWorldTransform( newTransform );
   }

void BulletPhysics::AddShape( StrongRenderComponentPtr pRenderComp, 
                              btCollisionShape* shape, 
                              float mass )
   {
   ENG_ASSERT( pRenderComp );

   auto pPhysicsAttr = pRenderComp->VGetPhysicsAttributes();
   // lookup the material
   MaterialData material( LookupMaterialData( pPhysicsAttr->VGetMaterial() ) );

   // localInertia defines how the object's mass is distributed
   btVector3 localInertia( 0.f, 0.f, 0.f );
   if( mass > 0.f )
      shape->calculateLocalInertia( mass, localInertia );

   Transform transform = Transform::g_Identity;
   transform = *pRenderComp->VGetTransformPtr();

   // set the initial transform of the body from the actor
   RenderCompMotionState * const myMotionState = ENG_NEW RenderCompMotionState( transform );

   btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, shape, localInertia );
   
   // set up the materal properties
   rbInfo.m_restitution = material.m_Restitution;
   rbInfo.m_friction = material.m_Friction;
   btRigidBody * const body = new btRigidBody( rbInfo );
   body->setCollisionFlags( pPhysicsAttr->VGetCollisionFlags() );
   if( !pPhysicsAttr->VGetIsActive() )
      {
      body->setActivationState( 0 );
      }
   body->setLinearFactor( Vec3_to_btVector3( pPhysicsAttr->VGetTranslateFactor() ) );
   body->setAngularFactor( Vec3_to_btVector3( pPhysicsAttr->VGetRotationFactor() ) );

   m_DynamicsWorld->addRigidBody( body, pPhysicsAttr->VGetCollisionId(), CollisionTable::GetSingleton().GetCollisionMask( pPhysicsAttr->VGetCollisionId() ) );

   // add it to the collection to be checked for changes in VSyncVisibleScene
   m_RenderCompToRigidBody[ pRenderComp ] = body;
   m_RigidBodyToRenderComp[ body ] = pRenderComp;

   VLinkRenderCompAttribute( pRenderComp );
   }

void BulletPhysics::RemoveCollisionObject( btCollisionObject * const removeMe )
   {
   // first remove the object from the physics sim
   m_DynamicsWorld->removeCollisionObject( removeMe );

   // then remove the pointer from the ongoing contacts list
   for( CollisionPairs::iterator it = m_PrevTickCollisionPairs.begin( );
        it != m_PrevTickCollisionPairs.end( ); )
      {
      CollisionPairs::iterator next = it; // jump to next pair in case iterator is erased
      ++next;

      if( it->first == removeMe || it->second == removeMe )
         {
         SendCollisionPairRemoveEvent( it->first, it->second );
         m_PrevTickCollisionPairs.erase( it );
         }

      it = next;
      }

   // if the object is a RigidBody (all of ours are RigidBodies, but it's good to be safe)
   if( btRigidBody * const body = btRigidBody::upcast( removeMe ) )
      {
      // delete the components of the object
      delete body->getMotionState( );
      delete body->getCollisionShape( );
      delete body->getUserPointer( );

      for( int ii = body->getNumConstraintRefs( ) - 1; ii >= 0; --ii )
         {
         btTypedConstraint * const constraint = body->getConstraintRef( ii );
         m_DynamicsWorld->removeConstraint( constraint );
         delete constraint;
         }
      }

   delete removeMe;
   }

btRigidBody* BulletPhysics::FindBulletRigidBody( StrongRenderComponentPtr pRenderComp ) const
   {
   RenderCompToRigidBodyMap::const_iterator found = m_RenderCompToRigidBody.find( pRenderComp );
   if( found != m_RenderCompToRigidBody.end() )
      return found->second;

   return NULL;
   }

StrongRenderComponentPtr BulletPhysics::FindRenderComponent( btRigidBody const * pbody ) const
   {
   RigidBodyToRenderCompMap::const_iterator found = m_RigidBodyToRenderComp.find( pbody );
   if( found != m_RigidBodyToRenderComp.end() )
      return found->second;

   return StrongRenderComponentPtr( );
   }

void BulletPhysics::VAddSphere( float radius, 
                                StrongRenderComponentPtr pRenderComp )
   {
   if( !pRenderComp )
      return;  // FUTURE WORK - Add a call to the error log here

   // create the collision body, which specifies the shape of the object
   btSphereShape * const collisionShape = new btSphereShape( radius );
   auto pPhysicsAttr = pRenderComp->VGetPhysicsAttributes();
   // calculate absolute mass from specificGravity
   float specificGravity = LookupSpecificGravity( pPhysicsAttr->VGetDensity() );
   float const volume = ( 4.f / 3.f ) * ENG_PI * radius * radius * radius;
   btScalar const mass = volume * specificGravity;

   AddShape( pRenderComp, /*initialTransform,*/ collisionShape, mass);
   }

void BulletPhysics::VAddBox( const Vec3& dimensions, StrongRenderComponentPtr pRenderComp )
   {
   if( !pRenderComp )
      return;  // FUTURE WORK: Add a call to the error log here

   // create the collision body, which specifies the shape of the object
   btBoxShape * const boxShape = new btBoxShape( Vec3_to_btVector3( dimensions ) );
   auto pPhysicsAttr = pRenderComp->VGetPhysicsAttributes();
   // calculate absolute mass from specificGravity
   float specificGravity = LookupSpecificGravity( pPhysicsAttr->VGetDensity() );
   float const volume = dimensions.x * dimensions.y * dimensions.z;
   btScalar const mass = volume * specificGravity;

   AddShape( pRenderComp,/* initialTransform,*/ boxShape, mass);
   }

void BulletPhysics::VAddPointCloud( Vec3 *verts, 
                                    int numPoints, 
                                    StrongRenderComponentPtr pRenderComp )
   {
   if( !pRenderComp )
      return;  // FUTURE WORK: Add a call to the error log here

   btConvexHullShape * const shape = new btConvexHullShape( );

   // add the points to the shape one at a time
   for( int ii = 0; ii < numPoints; ++ii )
      shape->addPoint( Vec3_to_btVector3( verts[ii] ) );

   // approximate absolute mass using bounding box
   btVector3 aabbMin( 0, 0, 0 ), aabbMax( 0, 0, 0 );
   shape->getAabb( btTransform::getIdentity( ), aabbMin, aabbMax );

   btVector3 const aabbExtents = aabbMax - aabbMin;

   auto pPhysicsAttr = pRenderComp->VGetPhysicsAttributes();
   float specificGravity = LookupSpecificGravity( pPhysicsAttr->VGetDensity() );
   float const volume = aabbExtents.x( ) * aabbExtents.y( ) * aabbExtents.z( ); // approximate the volume as bounding box
   btScalar const mass = volume * specificGravity;

   AddShape( pRenderComp, shape, mass);
   }

void BulletPhysics::VRemoveRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT( pRenderComp );
   if( btRigidBody * const body = FindBulletRigidBody( pRenderComp ) )
      {
      // destroy the body and all its components
      RemoveCollisionObject( body );
      m_RenderCompToRigidBody.erase( pRenderComp );
      m_RigidBodyToRenderComp.erase( body );
      }
   }

void BulletPhysics::VRenderDiagnostics( void )
   {
   m_DynamicsWorld->debugDrawWorld( );
   }


void BulletPhysics::VCreateTrigger( WeakActorPtr pActor, const Vec3 &pos, const float dim )
   {
   StrongActorPtr pStrongActor = pActor.lock();
   if( !pStrongActor )
      return;  // FUTURE WORK: Add a call to the error log here

   // create the collision body, which specifies the shape of the object
   btBoxShape * const boxShape = new btBoxShape( Vec3_to_btVector3( Vec3( dim, dim, dim ) ) );

   // triggers are immovable.  0 mass signals this to Bullet.
   btScalar const mass = 0;

   // set the initial position of the body from the actor
   Transform triggerTrans = Transform::g_Identity;
   triggerTrans.SetPosition( pos );
   RenderCompMotionState * const myMotionState = ENG_NEW RenderCompMotionState( triggerTrans );

   btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, boxShape, btVector3( 0, 0, 0 ) );
   btRigidBody * const body = new btRigidBody( rbInfo );

   m_DynamicsWorld->addRigidBody( body );

   // a trigger is just a box that doesn't collide with anything.  That's what "CF_NO_CONTACT_RESPONSE" indicates.
   body->setCollisionFlags( body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE );
   StrongRenderComponentPtr pStrongRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   m_RenderCompToRigidBody[ pStrongRenderComp ] = body;
   m_RigidBodyToRenderComp[ body ] = pStrongRenderComp;
   }

void BulletPhysics::VApplyForce( const Vec3 &dir, float newtons, ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast<IRenderComponent>( pStrongActor->GetComponent( 0 ).lock() );
   if( btRigidBody * const body = FindBulletRigidBody( pRenderComp ) )
      {
      body->setActivationState( DISABLE_DEACTIVATION );

      btVector3 const force( dir.x * newtons,
                             dir.y * newtons,
                             dir.z * newtons );

      body->applyCentralImpulse( force );
      }
   }

void BulletPhysics::VApplyTorque( const Vec3 &dir, float newtons, ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   if( btRigidBody * const body = FindBulletRigidBody( pRenderComp ) )
      {
      body->setActivationState( DISABLE_DEACTIVATION );

      btVector3 const torque( dir.x * newtons,
                              dir.y * newtons,
                              dir.z * newtons );

      body->applyTorqueImpulse( torque );
      }
   }

bool BulletPhysics::VKinematicMove( const Transform &trans, ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   if( btRigidBody * const body = FindBulletRigidBody( pRenderComp ) )
      {
      body->setActivationState( DISABLE_DEACTIVATION );

      // warp the body to the new position
      body->setWorldTransform( Transform_to_btTransform( trans ) );
      return true;
      }

   return false;
   }

Transform BulletPhysics::VGetTransform( ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );

   const btTransform& actorTransform = pRigidBody->getCenterOfMassTransform( );
   return btTransform_to_Transform( actorTransform );
   }

void BulletPhysics::VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp )
   {
   auto pPhtsicsAttr = pRenderComp->VGetPhysicsAttributes();
   auto bulletPhyAttr = dynamic_pointer_cast< BulletPhysicsAttributes >( pPhtsicsAttr );
   ENG_ASSERT( bulletPhyAttr );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   bulletPhyAttr->VSetRigidBody( pRigidBody );
   bulletPhyAttr->VSetIsLinkedToPhysicsWorld( true );
   }

void BulletPhysics::VDestroyRenderCompDelegate( IEventPtr pEvent )
   {
   shared_ptr<Event_Destory_Render_Component> pDerivedEvent = dynamic_pointer_cast<Event_Destory_Render_Component>( pEvent );
   VRemoveRenderComponent( pDerivedEvent->GetStrongRenderComp() );
   }

void BulletPhysics::VSetTransform( ActorId actorId, const Transform& trans )
   {
   VKinematicMove( trans, actorId );
   }

void BulletPhysics::VRotateY( ActorId const actorId, float const deltaAngleRadians, float const time )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );

   // create a transform to represent the additional turning this frame
   btTransform angleTransform;
   angleTransform.setIdentity( );
   angleTransform.getBasis( ).setEulerYPR( 0, deltaAngleRadians, 0 ); // rotation about body Y-axis

   // concatenate the transform onto the body's transform
   pRigidBody->setCenterOfMassTransform( pRigidBody->getCenterOfMassTransform( ) * angleTransform );
   }

float BulletPhysics::VGetOrientationY( ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );

   const btTransform& actorTransform = pRigidBody->getCenterOfMassTransform( );
   btMatrix3x3 actorRotationMat( actorTransform.getBasis( ) );  // should be just the rotation information

   btVector3 startingVec( 0, 0, 1 );
   btVector3 endingVec = actorRotationMat * startingVec; // transform the vector

   endingVec.setY( 0 );  // project it onto XZ plane, cause we only care about rotation on the XZ plane

   float const endingVecLength = endingVec.length( );
   if( endingVecLength < 0.001 )
      {
      // gimbal lock (orientation is straight up or down)
      return 0;
      }

   else
      {
      btVector3 cross = startingVec.cross( endingVec );
      float sign = cross.getY( ) > 0 ? 1.0f : -1.0f;
      return ( acosf( startingVec.dot( endingVec ) / endingVecLength ) * sign );
      }

   return FLT_MAX;  // fail...
   }

void BulletPhysics::VStopActor( ActorId actorId )
   {
   VSetVelocity( actorId, Vec3( 0.f, 0.f, 0.f ) );
   }

Vec3 BulletPhysics::VGetVelocity( ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody* pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      return Vec3( );
   btVector3 btVel = pRigidBody->getLinearVelocity( );
   return btVector3_to_Vec3( btVel );
   }

/////////////////////////////////////////////////////////////////////////////
void BulletPhysics::VSetVelocity( ActorId actorId, const Vec3& vel )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      return;
   btVector3 btVel = Vec3_to_btVector3( vel );
   pRigidBody->setLinearVelocity( btVel );
   }

/////////////////////////////////////////////////////////////////////////////
Vec3 BulletPhysics::VGetAngularVelocity( ActorId actorId )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody* pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      return Vec3( );
   btVector3 btVel = pRigidBody->getAngularVelocity( );
   return btVector3_to_Vec3( btVel );
   }

/////////////////////////////////////////////////////////////////////////////
void BulletPhysics::VSetAngularVelocity( ActorId actorId, const Vec3& vel )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      return;
   btVector3 btVel = Vec3_to_btVector3( vel );
   pRigidBody->setAngularVelocity( btVel );
   }

/////////////////////////////////////////////////////////////////////////////
void BulletPhysics::VTranslate( ActorId actorId, const Vec3& vec )
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   btRigidBody * pRigidBody = FindBulletRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   btVector3 btVec = Vec3_to_btVector3( vec );
   pRigidBody->translate( btVec );
   }

void BulletPhysics::BulletInternalTickCallback( btDynamicsWorld * const world, btScalar const timeStep )
   {
   ENG_ASSERT( world );

   ENG_ASSERT( world->getWorldUserInfo( ) );
   BulletPhysics * const bulletPhysics = static_cast< BulletPhysics* >( world->getWorldUserInfo( ) );

   CollisionPairs currentTickCollisionPairs;

   // look at all existing contacts
   btDispatcher * const dispatcher = world->getDispatcher( );
   for( int manifoldIdx = 0; manifoldIdx < dispatcher->getNumManifolds( ); ++manifoldIdx )
      {
      // get the "manifold", which is the set of data corresponding to a contact point
      //   between two physics objects
      btPersistentManifold const * const manifold = dispatcher->getManifoldByIndexInternal( manifoldIdx );
      ENG_ASSERT( manifold );

      // get the two bodies used in the manifold.  Bullet stores them as void*, so we must cast
      //  them back to btRigidBody*s.  Manipulating void* pointers is usually a bad
      //  idea, but we have to work with the environment that we're given.  We know this
      //  is safe because we only ever add btRigidBodys to the simulation
      btRigidBody const * const body0 = static_cast< btRigidBody const * >( manifold->getBody0( ) );
      btRigidBody const * const body1 = static_cast< btRigidBody const * >( manifold->getBody1( ) );

      // always create the pair in a predictable order to prevent duplicated pair
      bool const swapped = body0 > body1;

      btRigidBody const * const sortedBodyA = swapped ? body1 : body0;
      btRigidBody const * const sortedBodyB = swapped ? body0 : body1;

      CollisionPair const thisPair = std::make_pair( sortedBodyA, sortedBodyB );
      currentTickCollisionPairs.insert( thisPair );

      if( bulletPhysics->m_PrevTickCollisionPairs.find( thisPair ) == bulletPhysics->m_PrevTickCollisionPairs.end( ) )
         {
         // this is a new contact, which wasn't in our list before.  send an event to the game.
         bulletPhysics->SendCollisionPairAddEvent( manifold, body0, body1 );
         }
      }

   CollisionPairs removedCollisionPairs;

   // use the STL set difference function to find collision pairs that existed during the previous tick but not any more
   std::set_difference( bulletPhysics->m_PrevTickCollisionPairs.begin( ), bulletPhysics->m_PrevTickCollisionPairs.end( ),
                        currentTickCollisionPairs.begin( ), currentTickCollisionPairs.end( ),
                        std::inserter( removedCollisionPairs, removedCollisionPairs.begin( ) ) );

   for( CollisionPairs::const_iterator it = removedCollisionPairs.begin( ),
        end = removedCollisionPairs.end( ); it != end; ++it )
      {
      btRigidBody const * const body0 = it->first;
      btRigidBody const * const body1 = it->second;

      bulletPhysics->SendCollisionPairRemoveEvent( body0, body1 );
      }

   // the current tick becomes the previous tick.  this is the way of all things.
   bulletPhysics->m_PrevTickCollisionPairs = currentTickCollisionPairs;
   }

//////////////////////////////////////////////////////////////////////////////////////////
void BulletPhysics::SendCollisionPairAddEvent( btPersistentManifold const * manifold, btRigidBody const * const body0, btRigidBody const * const body1 )
   {
   if( body0->getUserPointer( ) || body1->getUserPointer( ) )
      {
      // only triggers have non-NULL userPointers

      // figure out which actor is the trigger
      btRigidBody const * triggerBody, *otherBody;

      if( body0->getUserPointer( ) )
         {
         triggerBody = body0;
         otherBody = body1;
         }
      else
         {
         otherBody = body0;
         triggerBody = body1;
         }

      // send the trigger event.
      int const triggerId = *static_cast< int* >( triggerBody->getUserPointer( ) );
      StrongRenderComponentPtr pStrongRenderComp = FindRenderComponent( otherBody );
      ENG_ASSERT( pStrongRenderComp );
      shared_ptr<EvtData_PhysTrigger_Enter> pEvent( ENG_NEW EvtData_PhysTrigger_Enter( triggerId, pStrongRenderComp->VGetOwner().lock()->GetId() ) );
      IEventManager::GetSingleton( )->VQueueEvent( pEvent );
      }
   else
      {
      StrongRenderComponentPtr pRenderComp0 = FindRenderComponent( body0 );
      StrongRenderComponentPtr pRenderComp1 = FindRenderComponent( body1 );
      ActorId id0 = pRenderComp0->VGetOwner().lock()->GetId();
      ActorId id1 = pRenderComp1->VGetOwner().lock()->GetId();
      if( id0 == INVALID_ACTOR_ID || id1 == INVALID_ACTOR_ID )
         {
         // something is colliding with a non-actor.  we currently don't send events for that
         return;
         }

      // this pair of colliding objects is new.  send a collision-begun event
      Vec3List collisionPoints;
      Vec3 sumNormalForce;
      Vec3 sumFrictionForce;

      for( int pointIdx = 0; pointIdx < manifold->getNumContacts( ); ++pointIdx )
         {
         btManifoldPoint const & point = manifold->getContactPoint( pointIdx );

         collisionPoints.push_back( btVector3_to_Vec3( point.getPositionWorldOnB( ) ) );

         sumNormalForce += btVector3_to_Vec3( point.m_combinedRestitution * point.m_normalWorldOnB );
         sumFrictionForce += btVector3_to_Vec3( point.m_combinedFriction * point.m_lateralFrictionDir1 );
         }

      // send the event for the game
      shared_ptr<EvtData_PhysCollision> pEvent( ENG_NEW EvtData_PhysCollision( id0, id1, sumNormalForce, sumFrictionForce, collisionPoints ) );
      IEventManager::GetSingleton( )->VQueueEvent( pEvent );
      }
   }

//////////////////////////////////////////////////////////////////////////////////////////
void BulletPhysics::SendCollisionPairRemoveEvent( btRigidBody const * const body0, btRigidBody const * const body1 )
   {
   if( body0->getUserPointer( ) || body1->getUserPointer( ) )
      {
      // figure out which actor is the trigger
      btRigidBody const * triggerBody, *otherBody;

      if( body0->getUserPointer( ) )
         {
         triggerBody = body0;
         otherBody = body1;
         }
      else
         {
         otherBody = body0;
         triggerBody = body1;
         }

      // send the trigger event.
      int const triggerId = *static_cast< int* >( triggerBody->getUserPointer() ); 
      StrongRenderComponentPtr pStrongRenderComp = FindRenderComponent( otherBody );
      ENG_ASSERT( pStrongRenderComp );
      shared_ptr<EvtData_PhysTrigger_Leave> pEvent( ENG_NEW EvtData_PhysTrigger_Leave( triggerId, pStrongRenderComp->VGetOwner().lock()->GetId() ) );
      IEventManager::GetSingleton( )->VQueueEvent( pEvent );
      }
   else
      {
      StrongRenderComponentPtr pRenderComp0 = FindRenderComponent( body0 );
      StrongRenderComponentPtr pRenderComp1 = FindRenderComponent( body1 );
      ActorId id0 = pRenderComp0->VGetOwner().lock()->GetId();
      ActorId id1 = pRenderComp1->VGetOwner().lock()->GetId();

      if( id0 == INVALID_ACTOR_ID || id1 == INVALID_ACTOR_ID )
         {
         // collision is ending between some object(s) that don't have actors.  we don't send events for that.
         return;
         }

      shared_ptr<EvtData_PhysSeparation> pEvent( ENG_NEW EvtData_PhysSeparation( id0, id1 ) );
      IEventManager::GetSingleton( )->VQueueEvent( pEvent );
      }
   }

float BulletPhysics::LookupSpecificGravity( const std::string& densityStr )
   {
   float density = 0;
   auto densityIt = m_DensityTable.find( densityStr );
   if( densityIt != m_DensityTable.end( ) )
      density = densityIt->second;
   // else: dump error

   return density;
   }

MaterialData BulletPhysics::LookupMaterialData( const std::string& materialStr )
   {
   auto materialIt = m_MaterialTable.find( materialStr );
   if( materialIt != m_MaterialTable.end( ) )
      return materialIt->second;
   else
      return MaterialData( 0, 0 );
   }


/////////////////////////////////////////////////////////////////////////////
//
// CreateGamePhysics 
//   The free function that creates an object that implements the IGamePhysics interface.
//
//IGamePhysics *CreateGamePhysics( )
//   {
//   std::auto_ptr<IGamePhysics> gamePhysics;
//   gamePhysics.reset( ENG_NEW BulletPhysics );
//
//   if( gamePhysics.get( ) && !gamePhysics->VInitialize( ) )
//      {
//      // physics failed to initialize.  delete it.
//      gamePhysics.reset( );
//      }
//
//   return gamePhysics.release( );
//   }
//
//IGamePhysics *CreateNullPhysics( )
//   {
//   std::auto_ptr<IGamePhysics> gamePhysics;
//   gamePhysics.reset( ENG_NEW NullPhysics );
//   if( gamePhysics.get( ) && !gamePhysics->VInitialize( ) )
//      {
//      // physics failed to initialize.  delete it.
//      gamePhysics.reset( );
//      }
//
//   return gamePhysics.release( );
//   }
