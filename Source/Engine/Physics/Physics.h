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
#include "PhysicsAttributes.h"

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
      // Initialiazation and Maintenance of the Physics World
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

      virtual ~IGamePhysics( void ) {};
      
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
   static T physicsImp;
   }

class BulletPhysics : public IGamePhysics
   {
   friend class IGamePhysics;
   friend class RayCastManager;
   public:
      
      virtual ~BulletPhysics( );
      // Initialiazation and Maintenance of the Physics World
      virtual bool VInitialize( ) override;
      virtual void VSyncVisibleScene( ) override;
      virtual void VSyncRigidBodyToRenderComponent( StrongRenderComponentPtr pRenderComp ) override;
      virtual void VOnUpdate( const float deltaSeconds ) override;
      
      // Initialization of Physics Objects
      virtual void VAddSphere( float radius, StrongRenderComponentPtr pRenderComp ) override;
      virtual void VAddBox( const Vec3& dimensions, StrongRenderComponentPtr pRenderComp ) override;

      virtual void VAddPointCloud( Vec3 *verts, 
                                   int numPoints, 
                                   StrongRenderComponentPtr pRenderComp ) override;
      virtual void VRemoveRenderComponent( StrongRenderComponentPtr pRenderComp ) override;

      // Debugging
      virtual void VRenderDiagnostics( void ) override;
      
      // Physics world modifiers
      /**
       * @brief 
       * TODO: check if it work correctly, becuase it doesn't set any Userpointer in this method
       * @param  pActor WeakActorPtr pActor
       * @param  pos const Vec3 & pos
       * @param  dim const float dim
       * @return void
       */
       virtual void VCreateTrigger( WeakActorPtr pActor, const Vec3 &pos, const float dim ) override;
      virtual void VApplyForce( const Vec3 &dir, float newtons, ActorId actorId ) override;
      virtual void VApplyTorque( const Vec3 &dir, float newtons, ActorId actorId ) override;
      virtual bool VKinematicMove( const Transform &trans, ActorId actorId ) override;

      virtual void VRotateY( ActorId actorId, float angleRadians, float time );
      virtual float VGetOrientationY( ActorId actorId );
      virtual void VStopActor( ActorId actorId );
      virtual Vec3 VGetVelocity( ActorId actorId );
      virtual void VSetVelocity( ActorId actorId, const Vec3& vel );
      virtual Vec3 VGetAngularVelocity( ActorId actorId );
      virtual void VSetAngularVelocity( ActorId actorId, const Vec3& vel );
      virtual void VTranslate( ActorId actorId, const Vec3& vec );

      virtual void VSetTransform( const ActorId id, const Transform &trans );

      virtual Transform VGetTransform( ActorId actorId );
      virtual void VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp ) override;

   protected:
      BulletPhysics( void );				// [mrmike] This was changed post-press to add event registration!
      void LoadXml( );
      float LookupSpecificGravity( const std::string& densityStr );
      MaterialData LookupMaterialData( const std::string& materialStr );

      btRigidBody * FindBulletRigidBody( StrongRenderComponentPtr pRenderComp ) const;

      StrongRenderComponentPtr FindRenderComponent( btRigidBody const * ) const;


      // helpers for sending events relating to collision pairs
      void SendCollisionPairAddEvent( btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1 );
      void SendCollisionPairRemoveEvent( btRigidBody const * body0, btRigidBody const * body1 );

      // common functionality used by VAddSphere, VAddBox, etc
      void AddShape( StrongRenderComponentPtr pRenderComp, btCollisionShape* shape, float mass );

      // helper for cleaning up objects
      void RemoveCollisionObject( btCollisionObject * removeMe );

      // callback from bullet for each physics time step.  set in VInitialize
      static void BulletInternalTickCallback( btDynamicsWorld * const world, btScalar const timeStep );

   private:

      // use auto pointers to automatically call delete on these objects
      //   during ~BulletPhysics

      // these are all of the objects that Bullet uses to do its work.
      //   see BulletPhysics::VInitialize() for some more info.
      btDynamicsWorld*                 m_DynamicsWorld;
      btBroadphaseInterface*           m_Broadphase;
      btCollisionDispatcher*           m_Dispatcher;
      btConstraintSolver*              m_Solver;
      btDefaultCollisionConfiguration* m_CollisionConfig;
      BulletDebugDrawer*               m_DebugDrawer;

      // tables read from the XML
      typedef std::map<std::string, float> DensityTable; 
      DensityTable m_DensityTable;

      typedef std::map<std::string, MaterialData> MaterialTable;
      MaterialTable m_MaterialTable;

      // data used to store which collision pair (bodies that are touching) need
      //   Collision events sent.  When a new pair of touching bodies are detected,
      //   they are added to m_PrevTickCollisionPairs and an event is sent.
      //   When the pair is no longer detected, they are removed and another event
      //   is sent.
      typedef std::pair< btRigidBody const *, btRigidBody const * > CollisionPair;
      typedef std::set< CollisionPair > CollisionPairs;
      CollisionPairs m_PrevTickCollisionPairs;

      // keep track of the existing rigid bodies:  To check them for updates
      //   to the actors' positions, and to remove them when their lives are over.
      typedef std::unordered_map< shared_ptr< IRenderComponent >, btRigidBody*> RenderCompToRigidBodyMap;
      RenderCompToRigidBodyMap m_RenderCompToRigidBody;

      // also keep a map to get the actor id from the btRigidBody*
      typedef std::map<btRigidBody const *, shared_ptr< IRenderComponent > > RigidBodyToRenderCompMap;
      RigidBodyToRenderCompMap m_RigidBodyToRenderComp;
   };
