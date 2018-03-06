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

RigidBody::RigidBody() : m_TransLateFactor( Vec3::g_Identity ), m_RotateFactor( Vec3::g_Identity )
   {
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
   //if( dynamic_pointer_cast< SphereCollider >( m_Colliders[0] ) )
   //   {
   //   // m_LinearVelocity = Vec3::g_Zero;
   //   // m_Force = Vec3( 0.f, 1.f, 1.f );
   //  // UpdateVelocity( deltaSecond );
   //   }
   VApplyFactor();
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
   if( m_RotateFactor == Vec3::g_Zero )
      {
      m_GlobalInverseInertia = Mat3x3::g_Zero;
      }
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

   if(m_Mass > 0.f && m_TransLateFactor != Vec3::g_Zero)
      {
      m_InverseMass = 1.f / m_Mass;
      }
   else
      {
      m_Mass = std::numeric_limits<float>::max();
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


void RigidBody::VSetTranslateFactor( const Vec3& factor )
   {
   m_TransLateFactor = factor;
   }

void RigidBody::VSetRotateFactor( const Vec3& factor )
   {
   m_RotateFactor = factor;
   }

void RigidBody::VApplyFactor()
   {
   m_LinearVelocity *= m_TransLateFactor;
   m_AngularVelocity *= m_RotateFactor;
   }

AABB RigidBody::VGetAABB() const 
   {
   auto toWorld = m_Transform.GetToWorld();
   auto translation = toWorld.GetToWorldPosition();
   const float maxFloat = std::numeric_limits<float>::max();
   Vec3 minPos( maxFloat, maxFloat, maxFloat );
   Vec3 maxPos( -maxFloat, -maxFloat, -maxFloat );
   // recompute AABB from rotated AABB
   for( auto& pCollider : m_Colliders )
      {
      // this AABB is in RB local space, need to convert it to world space
      auto colliderAABB = pCollider->VGetAABB();
      Vec3 tempMin = translation;
      Vec3 tempMax = translation;
      Vec3 localMin = colliderAABB.GetMin();
      Vec3 localMax = colliderAABB.GetMax();
      for( int i = 0; i < 3; ++i )
         {
         for( int j = 0; j < 3; ++j )
            {
            float e = toWorld[ i ][ j ] * localMin[ j ];
            float f = toWorld[ i ][ j ] * localMax[ j ];
            if( e < f )
               {
               tempMin[ j ] += e;
               tempMax[ j ] += f;
               }
            else
               {
               tempMin[ j ] += f;
               tempMax[ j ] += e;
               }
            }
         }
      minPos.x = std::min( minPos.x, tempMin.x );
      minPos.y = std::min( minPos.y, tempMin.y );
      minPos.z = std::min( minPos.z, tempMin.z );

      maxPos.x = std::max( maxPos.x, tempMax.x );
      maxPos.y = std::max( maxPos.y, tempMax.y );
      maxPos.z = std::max( maxPos.z, tempMax.z );
      }


   return AABB(minPos, maxPos);
   }