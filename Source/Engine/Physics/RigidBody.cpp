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

void RigidBody::UpdateVelocity( float deltaSecond )
   {
   m_LinearVelocity += m_InverseMass * ( m_Force * deltaSecond );
   m_AngularVelocity += m_GlobalInverseInertia * ( m_Torque * deltaSecond );

   m_Force = Vec3::g_Zero;
   m_Torque = Vec3::g_Zero;
   }

void RigidBody::MoveForOneTimeStep( float deltaSecond )
   {
   //m_Force = Vec3( 0.f, 0.001f, 0.f );
   // m_Force = Vec3( 0.f, 0.1f, 0.f );
   if( dynamic_pointer_cast< SphereCollider >( m_Colliders[0] ) )
      {
     // m_LinearVelocity = Vec3::g_Zero;
    //  m_Force = Vec3( 0.f, 0.1f, 0.f );
      }

   // Update transform first
   m_Transform.AddToWorldPosition( m_LinearVelocity * deltaSecond );
   m_GlobalCentroid = TransformToGlobal( m_LocalCentroid, true );

   const float angle = m_AngularVelocity.Length() * deltaSecond;
   if( angle != 0.0f )
      {
      m_AngularVelocity.Normalize();
      Quaternion q;
      q.BuildAxisDeg( m_AngularVelocity, angle );
      m_Transform.AddToWorldRotation( q );
      }

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

   if(m_Mass > 0.f )
      {
      m_InverseMass = 1.f / m_Mass;
      }
   else
      {
      m_InverseMass = 0.f;
      }
   
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

void RigidBody::AddCollider( shared_ptr<ICollider> collider )
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

void RigidBody::ApplyForceAt( const Vec3& force, const Vec3& globalPosition )
   {
   
   }

void RigidBody::ApplyForce( const Vec3& force )
   {
   m_Force += force;
   }

void RigidBody::UpdateGlobalInertia( void )
   {
   m_GlobalInertia = Mat3x3( m_Transform.GetToWorld() ) * m_LocalInertia * Mat3x3( m_Transform.GetFromWorld() );
   }

void RigidBody::SetWorldTransform( const Transform& transform )
   {
   m_Transform.SetRotMatrix( transform.GetToWorld() );
   m_Transform.SetPosition( transform.GetToWorldPosition() );
   }