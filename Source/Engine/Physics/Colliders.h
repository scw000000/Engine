/*!
 * \file Colliders.h
 * \date 2017/10/29 20:09
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
      weak_ptr<RigidBody> GetRigidBody( void ) const { return m_pRigidBody; }
      void SetRigidBody( weak_ptr<RigidBody> pRigidBody ) { m_pRigidBody = pRigidBody; }
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const = 0;
      // The input and output should be in local space
      virtual Vec3 VSupportMapping( const Vec3& direction ) const = 0;
      
      private:
      weak_ptr<RigidBody> m_pRigidBody;
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
      virtual Vec3 VSupportMapping( const Vec3& direction ) const override;
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
      BoxCollider( const Vec3& halfSize ) : m_HalfSize( halfSize ) {}
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const override;
      virtual Vec3 VSupportMapping( const Vec3& direction ) const override;
      private:
      Vec3 m_HalfSize;

   };