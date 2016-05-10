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
   float m_restitution;
   float m_friction;

   MaterialData( float restitution, float friction )
      {
      m_restitution = restitution;
      m_friction = friction;
      }

   MaterialData( const MaterialData& other )
      {
      m_restitution = other.m_restitution;
      m_friction = other.m_friction;
      }
   };

struct ActorMotionState : public btMotionState 
   {
   Transform m_Transform;

   ActorMotionState( Transform const & transform )
      : m_Transform( transform )
      {
      }

   // btMotionState interface:  Bullet calls these
   virtual void getWorldTransform( btTransform& worldTrans ) const
      {
      worldTrans = Transform_to_btTransform( m_Transform );
      }

   virtual void setWorldTransform( const btTransform& worldTrans )
      {
      m_Transform = btTransform_to_Transform( worldTrans );
      }
   };


class BulletPhysics : public IGamePhysics, ENG_Noncopyable 
   {
   public:
      BulletPhysics( );				// [mrmike] This was changed post-press to add event registration!
      virtual ~BulletPhysics( );

      // Initialiazation and Maintenance of the Physics World
      virtual bool VInitialize( ) override;
      virtual void VSyncVisibleScene( ) override;
      virtual void VOnUpdate( const float deltaSeconds ) override;

      // Initialization of Physics Objects
      virtual void VAddSphere( float radius, WeakActorPtr pActor, const std::string& densityStr, const std::string& physicsMaterial ) override;
      virtual void VAddBox( const Vec3& dimensions, WeakActorPtr pActor, const std::string& densityStr, const std::string& physicsMaterial ) override;

      /**
       * @brief declare a group of vertice as a rigid body
       *
       * @param  verts Vec3 * verts
       * @param  numPoints int numPoints
       * @param  pActor WeakActorPtr pActor
       * @param  densityStr const std::string & densityStr
       * @param  physicsMaterial const std::string & physicsMaterial
       * @return void
       */
       virtual void VAddPointCloud( Vec3 *verts, int numPoints, WeakActorPtr pActor, const std::string& densityStr, const std::string& physicsMaterial ) override;
      virtual void VRemoveActor( ActorId id ) override;

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
      virtual void VApplyForce( const Vec3 &dir, float newtons, ActorId aid ) override;
      virtual void VApplyTorque( const Vec3 &dir, float newtons, ActorId aid ) override;
      virtual bool VKinematicMove( const Transform &trans, ActorId aid ) override;

      virtual void VRotateY( ActorId actorId, float angleRadians, float time );
      virtual float VGetOrientationY( ActorId actorId );
      virtual void VStopActor( ActorId actorId );
      virtual Vec3 VGetVelocity( ActorId actorId );
      virtual void VSetVelocity( ActorId actorId, const Vec3& vel );
      virtual Vec3 VGetAngularVelocity( ActorId actorId );
      virtual void VSetAngularVelocity( ActorId actorId, const Vec3& vel );
      virtual void VTranslate( ActorId actorId, const Vec3& vec );

      virtual void VSetTransform( const ActorId id, const Transform &trans );

      virtual Transform VGetTransform( const ActorId id );

   private:
      void LoadXml( );
      float LookupSpecificGravity( const std::string& densityStr );
      MaterialData LookupMaterialData( const std::string& materialStr );

      btRigidBody * FindBulletRigidBody( ActorId id ) const;

      ActorId FindActorID( btRigidBody const * ) const;


      // helpers for sending events relating to collision pairs
      void SendCollisionPairAddEvent( btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1 );
      void SendCollisionPairRemoveEvent( btRigidBody const * body0, btRigidBody const * body1 );

      // common functionality used by VAddSphere, VAddBox, etc
      void AddShape( StrongActorPtr pActor, btCollisionShape* shape, float mass, const std::string& physicsMaterial );

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
      typedef std::map<std::string, MaterialData> MaterialTable;
      DensityTable m_densityTable;
      MaterialTable m_materialTable;

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
      typedef std::map<ActorId, btRigidBody*> ActorIDToBulletRigidBodyMap;
      ActorIDToBulletRigidBodyMap m_ActorIdToRigidBody;

      // also keep a map to get the actor id from the btRigidBody*
      typedef std::map<btRigidBody const *, ActorId> BulletRigidBodyToActorIDMap;
      BulletRigidBodyToActorIDMap m_RigidBodyToActorId;
   };
