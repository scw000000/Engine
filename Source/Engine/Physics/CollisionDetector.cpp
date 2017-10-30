/*!
 * \file CollisionDetector.cpp
 * \date 2017/10/29 21:18
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
#include "CollisionDetector.h"
#include "RigidBody.h"
#include "Colliders.h"

const float g_Esplion = 0.0001f;
const float g_EsplionSqr = g_Esplion * g_Esplion;

bool CollisionDetector::CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, Manifold& manifold )
   {
   Manifold maniFold;
   bool hasCollision = false;
   for(auto& pColliderA : pRigidBodyA->m_Colliders)
      {
      for( auto& pColliderB : pRigidBodyB->m_Colliders )
         {
         // Not sure how to generate manifold for composite RB
         if( CollisionDetection( pColliderA, pColliderB, manifold ) )
            {
            hasCollision = true;
            }
         }
      }
   return hasCollision;
   }

// GJK & EPA algorithm
bool CollisionDetector::CollisionDetection( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Manifold& manifold )
   {
   Simplex simplex;
   if( !GJK( pColliderA, pColliderB, simplex ) )
      {
      // No need for EPA
      return false;
      }
   EPA( pColliderA, pColliderB, simplex, manifold );

   return true;
   }

void UpdateSimplex( Simplex& simplex )
   {
   if( simplex.m_Size < 2 )
      {
      return;
      }

   auto lastPoint = simplex.Last();

   if( simplex.m_Size == 2 )
      {
      }
   else if( simplex.m_Size == 3 )
      {
      }
   else 
      {
      }
   }

bool ContainsOrigin( Simplex& simplex, Vec3& direction )
   {
   if( simplex.m_Size == 1 )
      {
      auto point = simplex.m_Vertice[ 0 ].m_PointCSO;
      direction = -point;
      if( point.Dot( point ) < g_EsplionSqr )
         {
         return true;
         }
      direction.Normalize();
      return false;
      }
   else if( simplex.m_Size == 2 )
      {

      }
   else if( simplex.m_Size == 3 )
      {

      }
   else
      {

      }
   return false;
   }

bool GJK( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex )
   {
   // Make up a 1D simplex
   auto supportPoint = GetCSOSupportPoint( pColliderA, pColliderB, g_Forward );
   simplex.Push( supportPoint );
   auto direction = -supportPoint.m_PointCSO;
   while(true)
      {
      // Find next support point
      supportPoint = GetCSOSupportPoint( pColliderA, pColliderB, direction );
      // Validate if it passes through the origin
      if( supportPoint.m_PointCSO.Dot( direction )  < 0.f )
         {
         return false;
         }
      simplex.Push( supportPoint );
      // update simplex
      UpdateSimplex( simplex );
      // if contains origin, return true, and update direction as well
      if( ContainsOrigin( simplex, direction ) )
         {
         return true;
         }
      }
   return false;
   }

void EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Simplex& simplex, Manifold& manifold )
   {
   // Blow up the simplex to tetrahedron
   if( simplex.m_Size < 4 )
      {
      }


   }

SupportPoint GetCSOSupportPoint( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Vec3& direction )
   {
   // The input is in world space, must convert it to local space then convert it back
   auto pRigidBodyA = pColliderA->GetRigidBody().lock();
   auto pRigidBodyB = pColliderB->GetRigidBody().lock();
   ENG_ASSERT( pRigidBodyA && pRigidBodyB );


   Vec3 pA( pColliderA->VSupportMapping( pRigidBodyA->TransformToLocal( direction, false ) ) );
   Vec3 pB( pColliderB->VSupportMapping( pRigidBodyB->TransformToLocal( -direction, false ) ) );

   pA = pRigidBodyA->TransformToGlobal( pA, true );
   pB = pRigidBodyA->TransformToGlobal( pB, true );

   return SupportPoint( pA - pB, pA, pB );
   }