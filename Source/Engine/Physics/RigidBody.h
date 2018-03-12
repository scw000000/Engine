#pragma once
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

class RigidBody : public IRigidBody
   {
   friend class PEPhysics;
   friend class CollisionDetector;
   friend class RigidBodySolver;
   friend class AABB;
   friend struct Manifold;
      public:
      RigidBody();
      virtual void VUpdateVelocity( float deltaSecond ) override;
      virtual void VMoveForOneTimeStep( float deltaSecond ) override;

      virtual void VUpdateOrientation( void ) override;
      virtual void VUpdateRigidBodyInfo( void ) override;

      virtual void VAddCollider( shared_ptr<ICollider> collider ) override;

      virtual Vec3 VTransformToGlobal( const Vec3 &localVec, bool isPoint ) const override;
      virtual Vec3 VTransformToLocal( const Vec3 &globalVec, bool isPoint ) override;

      virtual void VApplyForceAt( const Vec3& force, const Vec3& globalPosition ) override;
      virtual void VApplyForce( const Vec3& force ) override;
      virtual void VUpdateGlobalInertia( void ) override;

      virtual void VSetWorldTransform( const Transform& transform ) override;
      virtual void VApplyFactor() override;
      virtual void VSetTranslateFactor( const Vec3& factor ) override;
      virtual void VSetRotateFactor( const Vec3& factor ) override;
      virtual void VSetGravityScale( float gravScale ) override { m_GravityScale = gravScale; };
      virtual AABB VGetAABB() const override;
      virtual Vec3 VGetGlobalCentroid( void ) const override { return m_GlobalCentroid; }
      void SetMaterialData( MaterialData& data ) { m_Material = data; };
      float GetRestitution( void ) const { return m_Material.m_Restitution; }
      float GetFriction( void ) const { return m_Material.m_Friction; }

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
      float m_GravityScale;
      Vec3 m_TransLateFactor;
      Vec3 m_RotateFactor;
      std::vector< shared_ptr<ICollider> > m_Colliders;
      MaterialData m_Material;
   };

//class RigidBody{
//   friend class PEPhysics;
//   friend class CollisionDetector;
//   friend class RigidBodySolver;
//   public:
//   void UpdateVelocity( float deltaSecond );
//      void MoveForOneTimeStep( float deltaSecond );
//
//      void UpdateOrientation( void );
//      void UpdateRigidBodyInfo( void );
//
//      void AddCollider( shared_ptr<ICollider> collider );
//
//      Vec3 TransformToGlobal( const Vec3 &localVec, bool isPoint ) const;
//      Vec3 TransformToLocal( const Vec3 &globalVec, bool isPoint );
//
//      void ApplyForceAt( const Vec3& force, const Vec3& globalPosition );
//      void ApplyForce( const Vec3& force );
//      void UpdateGlobalInertia( void );
//
//      void SetWorldTransform( const Transform& transform );
//
//   private:
//       This transform is between local and global
//      Transform m_Transform;
//
//      Vec3 m_LocalCentroid;
//      Vec3 m_GlobalCentroid;
//
//      float m_Mass;
//      float m_InverseMass;
//
//      Mat3x3 m_LocalInertia;
//      Mat3x3 m_LocalInverseInertia;
//      Mat3x3 m_GlobalInertia;
//      Mat3x3 m_GlobalInverseInertia;
//
//      Vec3 m_Force;
//      Vec3 m_Torque;
//
//       Expressed in world space
//      Vec3 m_LinearVelocity;
//      Vec3 m_AngularVelocity;
//      
//      std::vector< shared_ptr<ICollider> > m_Colliders;
//      float m_GravityScale;
//   };
