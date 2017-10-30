/*!
 * \file RigidBody.h
 * \date 2017/10/28 8:14
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
 
// typedef std::vector<>

class ICollider;

class RigidBody{
   friend class PEPhysics;
   friend class CollisionDetector;
   public:
      void MoveForOneTimeStep( float deltaSecond );

      void UpdateOrientation( void );
      void UpdateRigidBodyInfo( void );

      void AddCollider( shared_ptr<ICollider> collider );

      Vec3 TransformToGlobal( const Vec3 &localVec, bool isPoint ) const;
      Vec3 TransformToLocal( const Vec3 &globalVec, bool isPoint );

      void ApplyForce( const Vec3& force, const Vec3& globalPosition );
      void UpdateGlobalInertia( void );

      void SetWorldTransform( const Transform& transform );

   private:
      // This transform is between local and global
      Transform m_Transform;

      Vec3 m_LocalCentroid;
      Vec3 m_GlobalCentroid;

      float m_Mass;
      float m_InverseMass;

      Mat3x3 m_LocalInertia;
      Mat3x3 m_LocalInverseInertia;
      Mat3x3 m_GlobalInertia;
      Mat3x3 m_GlobalInverseInertia;

      Vec3 m_Force;
      Vec3 m_Torque;

      // Expressed in world space
      Vec3 m_LinearVelocity;
      Vec3 m_AngularVelocity;
      
      std::vector< shared_ptr<ICollider> > m_Colliders;
      float gravityScale;
   };
