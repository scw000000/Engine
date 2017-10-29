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

void RigidBody::MoveForOneTimeStep( float deltaSecond )
   {
   m_LinearVelocity += m_InverseMass * ( m_Force * deltaSecond );
   m_AngularVelocity += m_GlobalInverseInertia * ( m_Torque * deltaSecond );

   m_Force = Vec3::g_Zero;
   m_AngularVelocity = Vec3::g_Zero;

   // Update transform first
   m_Transform.AddToWorldPosition( m_LinearVelocity * deltaSecond );
   m_GlobalCentroid = TransformToGlobal( m_LocalCentroid, true );

   const float angle = m_AngularVelocity.Length() * deltaSecond;
   m_AngularVelocity.Normalize();
   Quaternion q; 
   q.BuildAxisDeg( m_AngularVelocity, angle );
   m_Transform.AddToWorldRotation( q );

   // UpdateOrientation();
   // UpdatePositionFromGlobalCentroid();
   }

void RigidBody::UpdateOrientation( void )
   {

   }

void RigidBody::UpdateRigidBodyInfo( void )
   {
   if( !m_Colliders.size() )
      {
      ENG_WARNING( "Collider doesn't exist" );
      }
 
   m_Mass = 0.f;
   m_LocalCentroid = Vec3::g_Zero;
   for( auto& pCollider : m_Colliders )
      {
      m_Mass += pCollider->m_Mass;
      m_LocalCentroid += pCollider->m_Mass * pCollider->m_Transform.GetToWorld().Xform( m_LocalCentroid, 1.f );
      }

   m_InverseMass = 1.f / m_Mass;

   m_LocalCentroid *= m_InverseMass;
   m_GlobalCentroid = TransformToGlobal( m_LocalCentroid, true );

   m_LocalInertia = Mat3x3::g_Zero;
   for( auto& pCollider : m_Colliders )
      {
      const Vec3 d = m_LocalCentroid - pCollider->m_Transform.GetToWorld().Xform( pCollider->m_LocalCentroid );

      // https://en.wikipedia.org/wiki/Parallel_axis_theorem#Moment_of_inertia_matrix
      m_LocalInertia += pCollider->m_Inertia + pCollider->m_Mass * ( d.Dot( d ) * Mat3x3::g_Identity - d.OuterProduct( d ) );
      }

   // compute inverse inertia tensor
   m_LocalInverseInertia = m_LocalInertia.Inverse();
   }

void RigidBody::AddCollider( shared_ptr<Collider> collider )
   {
   if( !collider )
      {
      ENG_WARNING("Collider is already existing");
      }
   m_Colliders.push_back( collider );

   }

Vec3 RigidBody::TransformToGlobal( const Vec3 &localVec, bool isPoint ) const
   {
   return m_Transform.GetToWorld().Xform( localVec, isPoint ? 1.f: 0.f );
   }

Vec3 RigidBody::TransformToLocal( const Vec3 &globalVec, bool isPoint )
   {
   return m_Transform.GetFromWorld().Xform( globalVec, isPoint ? 1.f : 0.f );
   }

void RigidBody::ApplyForce( const Vec3& force, const Vec3& globalPosition )
   {
 
   }

void RigidBody::UpdateGlobalInertia( void )
   {
   m_GlobalInertia = Mat3x3( m_Transform.GetToWorld() ) * m_LocalInertia * Mat3x3( m_Transform.GetFromWorld() );
   }

void RigidBody::SetWorldTransform( const Transform& transform )
   {
   m_Transform.SetPosition( transform.GetToWorldPosition() );
   m_Transform.SetRotMatrix( transform.GetToWorld() );
   }