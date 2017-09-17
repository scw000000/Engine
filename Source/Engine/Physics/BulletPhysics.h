/*!
 * \file BulletPhysics.h
 * \date 2017/09/16 21:54
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
#include "Physics.h"

class BulletPhysics : public IGamePhysics
   {
   friend class IGamePhysics;
   friend class RayCastManager;
      public:

      virtual ~BulletPhysics();
      // Initialization and Maintenance of the Physics World
      virtual bool VInitialize() override;
      virtual void VSyncVisibleScene() override;
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
      virtual void VDestroyRenderCompDelegate( IEventPtr pEvent ) override;
      virtual void VSetSimulation( bool isOn ) override { m_IsSimulating = isOn; };

      protected:
      BulletPhysics( void );				// [mrmike] This was changed post-press to add event registration!
      void LoadXml();
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

      bool m_IsSimulating;
   };
