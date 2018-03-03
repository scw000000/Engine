/*!
 * \file RigidBody.cpp
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

#include "EngineStd.h"
#include "RigidBody.h"
#include "Colliders.h"
#include "..\Graphics\BasicGeometry.h"

RigidBody::RigidBody()
   {
   m_PositionLock[ 0 ] = false;
   m_PositionLock[ 1 ] = false;
   m_PositionLock[ 2 ] = false;

   m_RotationLock[ 0 ] = false;
   m_RotationLock[ 1 ] = false;
   m_RotationLock[ 2 ] = false;
   }

void RigidBody::VUpdateVelocity( float deltaSecond )
   {
   m_LinearVelocity += m_InverseMass * ( m_Force * deltaSecond );
   m_AngularVelocity += m_GlobalInverseInertia * ( m_Torque * deltaSecond );

   m_Force = Vec3::g_Zero;
   m_Torque = Vec3::g_Zero;
   }

void RigidBody::VMoveForOneTimeStep( float deltaSecond )
   {
   //m_Force = Vec3( 0.f, 0.001f, 0.f );
   // m_Force = Vec3( 0.f, 0.1f, 0.f );
   if( dynamic_pointer_cast< SphereCollider >( m_Colliders[0] ) )
      {
      // m_LinearVelocity = Vec3::g_Zero;
      // m_Force = Vec3( 0.f, 1.f, 1.f );
     // UpdateVelocity( deltaSecond );
      }
   VApplyLock();
   // Update transform first
   m_Transform.AddToWorldPosition( m_LinearVelocity * deltaSecond );
   m_GlobalCentroid = VTransformToGlobal( m_LocalCentroid, true );
 //  m_LocalCentroid = VTransformToLocal( m_GlobalCentroid, true );
   
   const float angle = m_AngularVelocity.Length() * deltaSecond;
   if( angle != 0.0f )
      {
      m_AngularVelocity.Normalize();
      Quaternion q;
      q.BuildAxisDeg( m_AngularVelocity, angle );
      auto origPos = m_Transform.GetToWorldPosition();
      m_Transform.SetPosition( Vec3::g_Zero );
      m_Transform.AddToWorldRotation( q );
      m_Transform.SetPosition( origPos );
      // m_Transform.AddFromWorldRotation( q );
      }

   auto rot = m_Transform.GetQuaternion();
   auto rotMat33 = rot.GetRotationMatrix33();
   auto invRotMat33 = rotMat33.Transpose();
   m_GlobalInverseInertia = rotMat33 * m_LocalInverseInertia * invRotMat33;
   // UpdateOrientation();
   // UpdatePositionFromGlobalCentroid();
   }

void RigidBody::VUpdateOrientation( void )
   {

   }

void RigidBody::VUpdateRigidBodyInfo( void )
   {
   if( !m_Colliders.size() )
      {
      ENG_WARNING( "Collider doesn't exist" );
      }
 
   m_Mass = 0.f;
   m_LocalCentroid = Vec3::g_Zero;
   for( auto& pCollider : m_Colliders )
      {
      float colliderMass = pCollider->VGetMass();
      m_Mass += colliderMass;
      m_LocalCentroid += colliderMass * pCollider->VGetRigidBodySpaceCentroid();
      }

   if(m_Mass > 0.f )
      {
      m_InverseMass = 1.f / m_Mass;
      }
   else
      {
      m_InverseMass = 0.f;
      }
   
   m_LocalCentroid *= m_InverseMass;
   m_GlobalCentroid = VTransformToGlobal( m_LocalCentroid, true );

   m_LocalInertia = Mat3x3::g_Zero;
   for( auto& pCollider : m_Colliders )
      {
      const Vec3 d = m_LocalCentroid - pCollider->VGetRigidBodySpaceCentroid();

      // https://en.wikipedia.org/wiki/Parallel_axis_theorem#Moment_of_inertia_matrix
      m_LocalInertia += pCollider->VGetInertia() + pCollider->VGetMass() * ( d.Dot( d ) * Mat3x3::g_Identity - d.OuterProduct( d ) );
      }

   // compute inverse inertia tensor
   m_LocalInverseInertia = m_LocalInertia.Inverse();
   }

void RigidBody::VAddCollider( shared_ptr<ICollider> collider )
   {
   if( !collider )
      {
      ENG_WARNING("Collider is is empty");
      }
   m_Colliders.push_back( collider );
   }

Vec3 RigidBody::VTransformToGlobal( const Vec3 &localVec, bool isPoint ) const
   {
   return m_Transform.GetToWorld().Xform( localVec, isPoint ? 1.f: 0.f );
   }

Vec3 RigidBody::VTransformToLocal( const Vec3 &globalVec, bool isPoint )
   {
   return m_Transform.GetFromWorld().Xform( globalVec, isPoint ? 1.f : 0.f );
   }

void RigidBody::VApplyForceAt( const Vec3& force, const Vec3& globalPosition )
   {
   
   }

void RigidBody::VApplyForce( const Vec3& force )
   {
   m_Force += force;
   }

void RigidBody::VUpdateGlobalInertia( void )
   {
   m_GlobalInertia = Mat3x3( m_Transform.GetToWorld() ) * m_LocalInertia * Mat3x3( m_Transform.GetFromWorld() );
   }

void RigidBody::VSetWorldTransform( const Transform& transform )
   {
   m_Transform.SetRotMatrix( transform.GetToWorld() );
   m_Transform.SetPosition( transform.GetToWorldPosition() );
   }

void RigidBody::VApplyLock()
   {
   m_LinearVelocity.x = m_PositionLock[ 0 ] ? 0.f : m_LinearVelocity.x;
   m_LinearVelocity.y = m_PositionLock[ 1 ] ? 0.f : m_LinearVelocity.y;
   m_LinearVelocity.z = m_PositionLock[ 2 ] ? 0.f : m_LinearVelocity.z;

   m_AngularVelocity.x = m_RotationLock[ 0 ] ? 0.f : m_AngularVelocity.x;
   m_AngularVelocity.y = m_RotationLock[ 1 ] ? 0.f : m_AngularVelocity.y;
   m_AngularVelocity.z = m_RotationLock[ 2 ] ? 0.f : m_AngularVelocity.z;
   }

void RigidBody::VSetPositionLock( bool xLock, bool yLock, bool zLock )
   {
   m_PositionLock[ 0 ] = xLock;
   m_PositionLock[ 1 ] = yLock;
   m_PositionLock[ 2 ] = zLock;
   }

void RigidBody::VSetRotationLock( bool xLock, bool yLock, bool zLock )
   {
   m_RotationLock[ 0 ] = xLock;
   m_RotationLock[ 1 ] = yLock;
   m_RotationLock[ 2 ] = zLock;
   }