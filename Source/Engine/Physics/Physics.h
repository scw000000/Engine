#pragma once
/*!
 * \file Physics.h
 * \date 2016/05/07 10:43
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
#include "btBulletCollisionCommon.h"
#include "BulletPhysicsAttributes.h"

class btVector3;
class btTransform;
class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class BulletDebugDrawer;

extern btVector3 Vec3_to_btVector3( Vec3 const & vec3 );

extern Vec3 btVector3_to_Vec3( btVector3 const & btvec );

extern btTransform Transform_to_btTransform( Transform const & trans );

extern Transform btTransform_to_Transform( btTransform const & trans );

struct MaterialData 
   {
   float m_Restitution;
   float m_Friction;

   MaterialData( float restitution, float friction )
      {
      m_Restitution = restitution;
      m_Friction = friction;
      }

   MaterialData( const MaterialData& other )
      {
      m_Restitution = other.m_Restitution;
      m_Friction = other.m_Friction;
      }
   };

struct RenderCompMotionState : public btMotionState 
   {
   Transform m_Transform;

   RenderCompMotionState( Transform const & transform )
      : m_Transform( transform )
      {
      }

   // btMotionState interface:  Bullet calls these
   virtual void getWorldTransform( btTransform& worldTrans ) const override
      {
      worldTrans = Transform_to_btTransform( m_Transform );
      }

   virtual void setWorldTransform( const btTransform& worldTrans ) override
      {
      m_Transform = btTransform_to_Transform( worldTrans );
      }
   };


class IGamePhysics : public ENG_Noncopyable
   {
   public:
      template< typename T > static void RegisterImplementation( void );
      static IGamePhysics& GetSingleton( void );
      static void Shutdown( void );
      // Initialization and Maintenance of the Physics World
      virtual bool VInitialize() = 0;
      virtual void VSyncVisibleScene() = 0;
      virtual void VSyncRigidBodyToRenderComponent( StrongRenderComponentPtr pRenderComp ) = 0;
      virtual void VOnUpdate( const float deltaSeconds ) = 0;

      // Initialization of Physics Objects
      virtual void VAddSphere( float radius, StrongRenderComponentPtr pRenderComp ) = 0;
      virtual void VAddBox( const Vec3& dimensions, StrongRenderComponentPtr pRenderComp ) = 0;
      virtual void VAddPointCloud( Vec3 *verts, int numPoints, StrongRenderComponentPtr pRenderComp ) = 0;
      virtual void VRemoveRenderComponent( StrongRenderComponentPtr pRenderComp ) = 0;

      // Debugging
      virtual void VRenderDiagnostics() = 0;

      // Physics world modifiers
      virtual void VCreateTrigger( WeakActorPtr pGameActor, const Vec3 &pos, const float dim ) = 0;
      virtual void VApplyForce( const Vec3 &dir, float newtons, ActorId actorId ) = 0;
      virtual void VApplyTorque( const Vec3 &dir, float newtons, ActorId actorId ) = 0;
      virtual bool VKinematicMove( const Transform &trans, ActorId actorId ) = 0;

      // Physics actor states
      virtual void VRotateY( ActorId actorId, float angleRadians, float time ) = 0;
      virtual float VGetOrientationY( ActorId actorId ) = 0;
      virtual void VStopActor( ActorId actorId ) = 0;
      virtual Vec3 VGetVelocity( ActorId actorId ) = 0;
      virtual void VSetVelocity( ActorId actorId, const Vec3& vel ) = 0;
      virtual Vec3 VGetAngularVelocity( ActorId actorId ) = 0;
      virtual void VSetAngularVelocity( ActorId actorId, const Vec3& vel ) = 0;
      virtual void VTranslate( ActorId actorId, const Vec3& vec ) = 0;

      virtual void VSetTransform( const ActorId id, const Transform& trans ) = 0;
      virtual Transform VGetTransform( ActorId actorId ) = 0;

      virtual void VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp ) = 0;

      virtual void VDestroyRenderCompDelegate( IEventPtr pEvent ) = 0;

      virtual ~IGamePhysics( void ) {};
      
      virtual void VSetSimulation( bool isOn ) = 0;

   protected:
      IGamePhysics( void );
   };

template< typename T > void IGamePhysics::RegisterImplementation( void )
   {
   if( !std::is_base_of< IGamePhysics, T >::value )
      {
      ENG_ERROR( "Invalid implementation register" );
      return;
      }
   ENG_NEW T;
   //static T physicsImp;
   }
