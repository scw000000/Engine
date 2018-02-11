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

class Collider : public ICollider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;

   public:
      virtual float VGetMass( void ) const override { return m_Mass; };
      virtual void VSetMass( float mass ) override { m_Mass = mass; }
      
      virtual void VSetLocalCentroid( const Vec3& newCentriod ) override { m_LocalCentroid = newCentriod; }
      virtual void VSetInertia( const Mat3x3& newInertia ) override { m_Inertia = newInertia; }
      virtual weak_ptr<IRigidBody> VGetRigidBody( void ) const override { return m_pRigidBody; }
      virtual void VSetRigidBody( weak_ptr<IRigidBody> pRigidBody ) override { m_pRigidBody = pRigidBody; }
      virtual Vec3 VGetRigidBodySpaceCentroid( void ) const override
         { return m_Transform.GetToWorld().Xform( m_LocalCentroid, 1.f ); };
      virtual Mat3x3 VGetInertia( void ) const override { return m_Inertia; }
   protected:
      weak_ptr<IRigidBody> m_pRigidBody;
      float m_Mass;
      Vec3 m_LocalCentroid;
      Mat3x3 m_Inertia;
      Transform m_Transform;
   };

class SphereCollider : public Collider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;
   friend class PESphereColliderAttributes;
      public:
      SphereCollider( float radius ) : m_Radius( radius ) {}
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const override;
      virtual Vec3 VSupportMapping( const Vec3& direction ) override;
      private:
      float m_Radius;

   };

class BoxCollider : public Collider
   {
   friend class RigidBody;
   friend class PEPhysics;
   friend class PEPhysicsAttributes;
   friend class PEBoxColliderAttributes;
      public:
      BoxCollider( const Vec3& halfSize ) : m_HalfSize( halfSize ) {}
      virtual void VRenderShape( const Mat4x4& m, const Mat4x4& vp ) const override;
      virtual Vec3 VSupportMapping( const Vec3& direction ) override;
      private:
      Vec3 m_HalfSize;

   };