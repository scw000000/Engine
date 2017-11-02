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
#include <limits>

const float g_Esplion = 0.0001f;
const float g_EsplionSqr = g_Esplion * g_Esplion;

bool CollisionDetector::CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, Manifold& manifold )
   {
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

void CollisionDetector::UpdateSimplex( Simplex& simplex )
   {
   // No need if it only contains only one vertex
   if( simplex.m_Size < 2 )
      {
      return;
      }

   if( simplex.m_Size == 2 )
      {
      //  impossible  |    region 2   | region 1
      //              a ------------- b (last added point)
      // Test if it's in region 1
      const auto& b = simplex.Last().m_PointCSO;
      // Test BO dot BA
      if( -b.Dot( simplex.m_Vertice[0].m_PointCSO - b ) < 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 1 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 1 ] );
         simplex.m_Size = 1;
         }
      // If not then do nothing
      }
   else if( simplex.m_Size == 3 )
      {
      //
      //     A- -
      //     |     -  - 
      //     |          -  C
      //     |     -  - 
      //     B- -
      //
      // Test vertex C
      const Vec3& a = simplex.m_Vertice[ 0 ].m_PointCSO;
      const Vec3& b = simplex.m_Vertice[ 1 ].m_PointCSO;
      const Vec3& c = simplex.m_Vertice[ 2 ].m_PointCSO;
      float d5 = ( b - a ).Dot( -c );
      float d6 = ( c - a ).Dot( -c );
      if(d6 >= 0.0f && d5 <= d6)
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 2 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 1;
         }
      float d1 = ( b - a ).Dot( -a );
      float d2 = ( c - a ).Dot( -a );

      float vb = d5 * d2 - d1 * d6;
      // Test edge AC
      if( vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f )
         {
         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 2 ];
         simplex.m_Size = 2;
         }
      // Test edge BC
      float d3 = ( b - a ).Dot( -b );
      float d4 = ( c - a ).Dot( -b );
      float va = d3 * d6 - d5*d4;
      if( va <= 0.0f && ( d4 - d3 ) >= 0.0f && ( d5 - d6 ) >= 0.0f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 2 ];
         simplex.m_Size = 2;
         }
      }
   else 
      {
      // First swap the vertice to make it facing correct direction
      Vec3& a = simplex.m_Vertice[ 0 ].m_PointCSO;
      Vec3& b = simplex.m_Vertice[ 1 ].m_PointCSO;
      Vec3& c = simplex.m_Vertice[ 2 ].m_PointCSO;
      Vec3& d = simplex.m_Vertice[ 3 ].m_PointCSO;

      Vec3 ab = b - a;
      Vec3 ac = c - a;
      Vec3 n = ab.Cross( ac );
      if( n.Dot( d - a ) > 0.f )
         {
         std::swap( simplex.m_Vertice[ 1 ], simplex.m_Vertice[ 2 ] );
         ab = b - a;
         ac = c - a;
         }
      // Test vertex D
      Vec3 ad = d - a;
      if( -a.Dot( ab ) <= 0.f
          && -a.Dot( ac ) <= 0.f
          && -a.Dot( ad ) <= 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 1;
         }
      Vec3 nACD = ac.Cross( ad );
      Vec3 nADB = ad.Cross( ab );
      // Test edge AD
      if( -a.Dot( ad ) >= 0.f
          && -d.Dot( -ad ) >= 0.f 
          && -a.Dot( ad.Cross( nADB ) ) >= 0.f
          && -a.Dot( nACD.Cross( ad ) ) >= 0.f )
         {
         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 1 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 2;
         }

      Vec3 bc = c - b;
      Vec3 bd = d - b;
      Vec3 nABC = ab.Cross( ac );
      Vec3 nBDC = bd.Cross( bc );
      // Test edge BD
      if( -b.Dot( bd ) >= 0.f
          && -d.Dot( -bd ) >= 0.f
          && -b.Dot( nADB.Cross( bd ) ) >= 0.f
          && -b.Dot( bd.Cross( nBDC ) ) >= 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 1 ];

         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 3 ];
         // a = b;
         // b = d;
         simplex.m_Size = 2;
         }
      // Test edge CD
      Vec3 cd = d - c;
      if( -c.Dot( cd ) >= 0.f
          && -d.Dot( -cd ) >= 0.f
          &&  -c.Dot( nBDC.Cross( cd ) ) >= 0.f
          && -c.Dot( cd.Cross( nACD ) ) >= 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 2 ];
         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 3 ];
         // a = c;
         // b = d;
         simplex.m_Size = 2;
         }
      // Test triangle ADB
      // If origin and the rest point ( c ) are in different face
      // then it's on the triangle
      if( ( -a.Dot( nADB ) ) * ( ac.Dot( nADB )  ) < 0.f )
         {
         simplex.m_Vertice[ 2 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 2 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         }
      // Test triangle ACD
      if( ( -a.Dot( nACD ) ) * ( ab.Dot( nACD ) ) < 0.f )
         {
         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 1 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         }
      // Test triangle BDC
      if( ( -b.Dot( nBDC ) ) * ( -ab.Dot( nBDC ) ) < 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         }
      }
   }

bool CollisionDetector::ContainsOrigin( Simplex& simplex, Vec3& direction )
   {
   if( simplex.m_Size == 1 )
      {
      auto point = simplex.m_Vertice[ 0 ].m_PointCSO;
      if( point.Dot( point ) < g_EsplionSqr )
         {
         return true;
         }
      direction = -point;
      direction.Normalize();
      return false;
      }
   else if( simplex.m_Size == 2 )
      {
      //     o
      //
      // a---k---b
      // find k, k = a + tv , v is vector ab
      // ( k - a ) . ( k ) = 0
      // ( tv ) . ( a + tv ) = 0
      // t ( v.a ) + t^2 ( v . v ) = 0
      // t = -2( v.a ) / 2 ( v.v ) = 0
      // We don't need to worry about if t is larger than 1 or smaller than 0 because
      // This simplex has been updated
      auto& a = simplex.m_Vertice[ 0 ].m_PointCSO;
      auto& b = simplex.m_Vertice[ 1 ].m_PointCSO;
      auto v = ( b - a );
      float t = -v.Dot( a ) / v.Dot( v );
      auto p = a + t * v;
      if( p.Dot( p ) < g_EsplionSqr )
         {
         return true;
         }
      direction = -p;
      direction.Normalize();
      return false;
      }
   else if( simplex.m_Size == 3 )
      {
      const Vec3& a = simplex.m_Vertice[ 0 ].m_PointCSO;
      const Vec3& b = simplex.m_Vertice[ 1 ].m_PointCSO;
      const Vec3& c = simplex.m_Vertice[ 2 ].m_PointCSO;

      float d1 = ( b - a ).Dot( -a );
      float d2 = ( c - a ).Dot( -a );
      float d3 = ( b - a ).Dot( -b );
      float d4 = ( c - a ).Dot( -b );
      float d5 = ( b - a ).Dot( -c );
      float d6 = ( c - a ).Dot( -c );
      
      float va = d3 * d6 - d5 * d4;
      float vb = d5 * d2 - d1 * d6;
      float vc = d1 * d4 - d3 * d2;
      
      float sum = va + vb + vc;
      float u = va / sum;
      float v = vb / sum;
      float w = 1.f - va - vb;

      auto p = u * a + v * b + w * c;
      if( p.Dot( p ) < g_EsplionSqr )
         {
         return true;
         }
      direction = -p;
      direction.Normalize();
      }
   else
      {
      // First test if the origin is inside the four planes 
      // The order of the vertex in simplex should be ccw
      const Vec3& a = simplex.m_Vertice[ 0 ].m_PointCSO;
      const Vec3& b = simplex.m_Vertice[ 1 ].m_PointCSO;
      const Vec3& c = simplex.m_Vertice[ 2 ].m_PointCSO;
      const Vec3& d = simplex.m_Vertice[ 3 ].m_PointCSO;
      
      Plane planeABC;
      planeABC.Init( a, b, c );
      Plane planeADB;
      planeADB.Init( a, b, c );
      Plane planeACD;
      planeACD.Init( a, c, d );
      Plane planeCBD;
      planeCBD.Init( c, b, d );

      // only update direction if the origin is under the 4 planes
      float bestSqDist = std::numeric_limits<float>::max();
      float distSq = 0.f;
      bool isInside = true;
      Vec3 bestPoint;
      Vec3 projPoint;
      // dot normal is always zero, so test d only
      // if it's above the plane, then origin must not inside the shape
      // so try to update the direction
      if( planeABC.GetD() > 0.f )
         {
         isInside = false;
         // Decide the project point r = p + tn
         // r dot n + d = 0
         // ( p + tn ) dot n + d = 0
         // n.p + t + d = 0
         // t = -( np + d )
         // p = 0, t = -d, r = -tn
         projPoint = -planeABC.GetD() * planeABC.GetNormal();
         distSq = projPoint.Dot( projPoint );
         // The distance will be negative, so we need to find the value which is
         // closest to 0, i.e. largest sign distance
         if( distSq < bestSqDist )
            {
            bestSqDist = distSq;
            bestPoint = projPoint;
            }
         }
      if( distSq = planeADB.GetD() > 0.f )
         {
         isInside = false;
         projPoint = -planeADB.GetD() * planeADB.GetNormal();
         distSq = projPoint.Dot( projPoint );
         if( distSq < bestSqDist )
            {
            bestSqDist = distSq;
            bestPoint = projPoint;
            }
         }

      if( distSq = planeACD.GetD() > 0.f )
         {
         isInside = false;
         projPoint = -planeACD.GetD() * planeACD.GetNormal();
         distSq = projPoint.Dot( projPoint );
         if( distSq < bestSqDist )
            {
            bestSqDist = distSq;
            bestPoint = projPoint;
            }
         }

      if( distSq = planeCBD.GetD() > 0.f )
         {
         isInside = false;
         projPoint = -planeCBD.GetD() * planeCBD.GetNormal();
         distSq = projPoint.Dot( projPoint );
         if( distSq < bestSqDist )
            {
            bestSqDist = distSq;
            bestPoint = projPoint;
            }
         }
      if( !isInside )
         {
         direction = -bestPoint;
         direction.Normalize();
         }
      return isInside;
      }
      
   }

bool CollisionDetector::GJK( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex )
   {
   // Make up a 1D simplex
   auto supportPoint = GetCSOSupportPoint( pColliderA, pColliderB, g_Forward );
   simplex.Push( supportPoint );
   auto direction = -supportPoint.m_PointCSO;
   direction.Normalize();
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

void CollisionDetector::EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Simplex& simplex, Manifold& manifold )
   {
   // Blow up the simplex to tetrahedron
   if( simplex.m_Size < 4 )
      {
      }


   }

SupportPoint CollisionDetector::GetCSOSupportPoint( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Vec3& direction )
   {
   // The input is in world space, must convert it to local space then convert it back
   auto pRigidBodyA = pColliderA->GetRigidBody().lock();
   auto pRigidBodyB = pColliderB->GetRigidBody().lock();
   ENG_ASSERT( pRigidBodyA && pRigidBodyB );


   Vec3 pA( pColliderA->VSupportMapping( pRigidBodyA->TransformToLocal( direction, false ) ) );
   Vec3 pB( pColliderB->VSupportMapping( pRigidBodyB->TransformToLocal( -direction, false ) ) );

   pA = pRigidBodyA->TransformToGlobal( pA, true );
   pB = pRigidBodyB->TransformToGlobal( pB, true );

   return SupportPoint( pA - pB, pA, pB );
   }