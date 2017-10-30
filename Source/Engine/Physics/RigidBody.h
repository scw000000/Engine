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

class RigidBody;

class ICollider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;
      public:

      void SetMass( float mass ) { m_Mass = mass; }
      void SetLocalCentroid( const Vec3& newCentriod ) { m_LocalCentroid = newCentriod; }
      void SetInertia( const Mat3x3& newInertia ) { m_Inertia = newInertia; }
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const = 0;

   private:
      float m_Mass;
      Vec3 m_LocalCentroid;
      Mat3x3 m_Inertia;
      Transform m_Transform;
   };

class SphereCollider : public ICollider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;
   friend class PESphereColliderAttributes;
      public:
      SphereCollider( float radius ) : m_Radius( radius ) {}
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const override;
      private:
   float m_Radius;

   };

class BoxCollider : public ICollider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;
   friend class PEBoxColliderAttributes;
   public:
      BoxCollider( const Vec3& dimension ) : m_Dimension(dimension) { }
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const override;
      private:
      Vec3 m_Dimension;

   };

class RigidBody{
   friend class PEPhysics;
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
