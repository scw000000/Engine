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

#define CONTACT_DISTANCE_TOLERANCE fESPLION

ContactPoint::ContactPoint( void ) :
   m_AccumulatedImpulseN( 0.f )
   , m_AccumulatedImpulseT( 0.f )
   , m_AccumulatedImpulseBT( 0.f )
   , m_PenetrationDepth( 0.f )
   {
   }

//ContactPoint::ContactPoint( const SupportPoint& supportPoint, const Vec3& normal, const Vec3& ra, const Vec3& rb )
//   : m_SupportPoint( supportPoint )
//   , m_AccumulatedImpulseN( 0.f )
//   , m_AccumulatedImpulseT( 0.f )
//   , m_AccumulatedImpulseBT( 0.f )
//   , m_Normal( normal )
//   , m_RA( ra )
//   , m_RB( rb )
//   {
//   // compute tangent and bitangent from normal
//   // http://box2d.org/2014/02/computing-a-basis/
//   // x is significant axis
//   // 0.57735f is sqrt( 1/3 ), for an uniform vector it must have at least one
//   // component which scale is larger than that
//   if( std::abs( m_Normal.x ) >= 0.57735f )
//      {
//      m_Tangent = Vec3( m_Normal.y, -m_Normal.x, 0.f );
//      }
//   else // x is not
//      {
//      m_Tangent = Vec3( 0.f, m_Normal.z, -m_Normal.y );
//      }
//   m_Tangent.Normalize();
//   m_Bitangent = m_Normal.Cross( m_Tangent );
//
//   m_NRACrossN = ( -m_RA ).Cross( m_Normal );
//   m_RBCrossN = ( m_RB ).Cross( m_Normal );
//
//   m_NRACrossT = ( -m_RA ).Cross( m_Tangent );
//   m_RBCrossT = ( m_RB ).Cross( m_Tangent );
//
//   m_NRACrossBT = ( -m_RA ).Cross( m_Bitangent );
//   m_RBCrossBT = ( m_RB ).Cross( m_Bitangent );
//   }

bool ContactPoint::IsValid( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB ) const
   {
   Vec3 vBA = pRigidBodyA->VTransformToGlobal( m_PointALS, true ) - pRigidBodyB->VTransformToGlobal( m_PointBLS, true );
   // vBA should be the same direction with normal
   // if it's negative that means it might be separated
   if( vBA.Dot( m_Normal ) <= -0.01f )
      {
      return false;
      }
   // Get projection vector
   vBA -= m_Normal * ( vBA.Dot( m_Normal ) );
   if( vBA.Dot( vBA ) >= 0.01f )
      {
      return false;
      }

   return true;
   }

void ContactPoint::Update( shared_ptr<IRigidBody> pRigidBodyA, shared_ptr<IRigidBody> pRigidBodyB )
   {
   m_PointALS = pRigidBodyA->VTransformToLocal( m_PointAWS, true );
   m_RA = m_PointAWS - pRigidBodyA->VGetGlobalCentroid();

   m_PointBLS = pRigidBodyB->VTransformToLocal( m_PointBWS, true );
   m_RB = m_PointBWS - pRigidBodyB->VGetGlobalCentroid();
   // compute tangent and bitangent from normal
   // http://box2d.org/2014/02/computing-a-basis/
   // x is significant axis
   // 0.57735f is sqrt( 1/3 ), for an uniform vector it must have at least one
   // component which scale is larger than that
   if( std::abs( m_Normal.x ) >= 0.57735f )
      {
      m_Tangent = Vec3( m_Normal.y, -m_Normal.x, 0.f );
      }
   else // x is not
      {
      m_Tangent = Vec3( 0.f, m_Normal.z, -m_Normal.y );
      }
   m_Tangent.Normalize();
   m_Bitangent = m_Normal.Cross( m_Tangent );

   m_NRACrossN = ( -m_RA ).Cross( m_Normal );
   m_RBCrossN = ( m_RB ).Cross( m_Normal );

   m_NRACrossT = ( -m_RA ).Cross( m_Tangent );
   m_RBCrossT = ( m_RB ).Cross( m_Tangent );

   m_NRACrossBT = ( -m_RA ).Cross( m_Bitangent );
   m_RBCrossBT = ( m_RB ).Cross( m_Bitangent );

   }

Vec3 Face::FindBarycentricCoords( const Vec3& point )
   {
   const Vec3& a = m_Vertices[ 0 ].lock()->m_PointCSO;
   const Vec3& b = m_Vertices[ 1 ].lock()->m_PointCSO;
   const Vec3& c = m_Vertices[ 2 ].lock()->m_PointCSO;
   auto n = m_Plane.GetNormal();
   // Cross: area of sub triangle * 2
   // Dot: project the sub-triangle to the triangle
   float va = n.Dot( ( b - point ).Cross( c - point ) );
   float vb = n.Dot( ( c - point ).Cross( a - point ) );
   float vc = n.Dot( ( a - point ).Cross( b - point ) );

   float u = va / ( va + vb + vc );
   float v = vb / ( va + vb + vc );
   return Vec3( u, v, 1.f - u - v );
   }

float DistanceSqFromPointToLine( const Vec3& pA, const Vec3& pB, const Vec3& pP )
   {
   Vec3 lineDir = ( pB - pA );
   lineDir.Normalize();
   Vec3 vAP = pP - pA;
   return ( vAP - ( vAP.Dot( lineDir ) * lineDir ) ).LengthSq();
   }

float DistanceSqFromPointToTriangle( const Vec3& pA, const Vec3& pB, const Vec3& pC, const Vec3& pP )
   {
   Vec3 vAB = pB - pA;
   Vec3 vAC = pC - pA;
   Vec3 vBC = pC - pA;

   Vec3 vAP = pP - pA;

   // vertices region test
   float snom = vAB.Dot( pP );
   float tnom = vAC.Dot( pP );

   if( snom <= 0.f && tnom <= 0.f )
      {
      return ( vAP ).LengthSq();
      }
   
   Vec3 vBP = pP - pB;
   float sdenom = vBP.Dot( -vAB );
   float unom = vBP.Dot( vBC );
   if( sdenom <= 0.f && unom <= 0.f )
      {
      return ( vBP ).LengthSq();
      }

   Vec3 vCP = pP - pC;
   float tdenom = vCP.Dot( -vAC );
   float udenom = vCP.Dot( -vBC );
   if( tdenom <= 0.f && udenom <= 0.f )
      {
      return ( vCP ).LengthSq();
      }

   Vec3 n = vAB.Cross( vAC );
   // test edge ab
   if( n.Dot( -vAP.Cross( -vBP ) ) <= 0.f && snom >= 0.0f && sdenom >= 0.0f )
      {
      return DistanceSqFromPointToLine( pA, pB, pP );
      }
   // test edge ac
   if( n.Dot( -vCP.Cross( -vAP ) ) <= 0.f && tnom >= 0.0f && tdenom >= 0.0f )
      {
      return DistanceSqFromPointToLine( pA, pC, pP );
      }
   // test edge bc
   if( n.Dot( -vBP.Cross( -vCP ) ) <= 0.f && unom >= 0.0f && udenom >= 0.0f )
      {
      return DistanceSqFromPointToLine( pB, pC, pP );
      }

   float d = -pA.Dot( n );
   return std::abs( pP.Dot( n ) + d );
   }



void Manifold::AddContactPoint( const ContactPoint& newPoint)
   {
   bool replaced = false;
   // Scan through the valid points, remove or update them
   int invalidIdx = 0;
   int validIdx = m_ContactPointCount - 1;
   while( invalidIdx <= validIdx )
      {
      // update invalid idx
      while( invalidIdx <= validIdx )
         {
         auto& currCP = m_ContactPoints[ invalidIdx ];
         Vec3 vA = currCP.m_PointALS - newPoint.m_PointALS;
         Vec3 vB = currCP.m_PointBLS - newPoint.m_PointBLS;
         // test if I can replace it, if true, replace it
         if( !replaced && vA.Dot( vA ) <= fESPLION || vB.Dot( vB ) <= fESPLION )
            {
            replaced = true;
            currCP = newPoint;
            }
         // else if it's not valid, break
         else if( !currCP.IsValid( m_pRigidBodyA, m_pRigidBodyB ) )
         {
            break;
         }
         // else then it's valid, recalculate its info?
         currCP.m_Normal = newPoint.m_Normal;
         currCP.m_PointAWS = m_pRigidBodyA->VTransformToGlobal( currCP.m_PointALS, true );
         currCP.m_PointBWS = m_pRigidBodyB->VTransformToGlobal( currCP.m_PointBLS, true );
         currCP.Update( m_pRigidBodyA, m_pRigidBodyB );
         ++invalidIdx;
         }

      while( invalidIdx <= validIdx )
         {  
         auto& currCP = m_ContactPoints[ validIdx ];
         Vec3 vA = currCP.m_PointALS - newPoint.m_PointALS;
         Vec3 vB = currCP.m_PointBLS - newPoint.m_PointBLS;
         // test if I can replace it, if true, replace it and break
         if( !replaced && vA.Dot( vA ) <= fESPLION || vB.Dot( vB ) <= fESPLION )
            {
            replaced = true;
            currCP = newPoint;
            break;
            }
         // else if it's valid, break
         else if( currCP.IsValid( m_pRigidBodyA, m_pRigidBodyB ) )
            {
            currCP.m_Normal = newPoint.m_Normal;
            currCP.m_PointAWS = m_pRigidBodyA->VTransformToGlobal( currCP.m_PointALS, true );
            currCP.m_PointBWS = m_pRigidBodyB->VTransformToGlobal( currCP.m_PointBLS, true );
            currCP.Update( m_pRigidBodyA, m_pRigidBodyB );
            break;
            }
         --validIdx;
         }
      if( invalidIdx >= validIdx )
         {
         m_ContactPointCount = invalidIdx;
         break;
         }
      std::swap( m_ContactPoints[ invalidIdx ], m_ContactPoints[ validIdx ] );
      ++invalidIdx;
      --validIdx;
      }
   if( !replaced )
      {
      // append the newly added one if this manifold has less than 4 cps
      if( m_ContactPointCount < MANIFOLD_MAX_NUM )
         {
         m_ContactPoints[ m_ContactPointCount++ ] = newPoint;
         }
      else // find a way to discard new one
         {
         // first append the new cp to 5th idx
         m_ContactPoints[ MANIFOLD_MAX_NUM ] = newPoint;

         ContactPoint* pDeepest = &m_ContactPoints[ 0 ];
         for( int i = 1; i <= MANIFOLD_MAX_NUM; ++i )
            {
            if( m_ContactPoints[ i ].m_PenetrationDepth > pDeepest->m_PenetrationDepth )
               {
               pDeepest = &m_ContactPoints[ i ];
               }
            }

         ContactPoint* pfarestFromDeepst = &m_ContactPoints[ 0 ];
         float maxDistSq = ( pfarestFromDeepst->m_PointAWS - pDeepest->m_PointAWS ).LengthSq();
         for( int i = 1; i <= MANIFOLD_MAX_NUM; ++i )
            {
            float distSq = ( m_ContactPoints[ i ].m_PointAWS - pDeepest->m_PointAWS ).LengthSq();
            if( distSq > maxDistSq )
               {
               pfarestFromDeepst = &m_ContactPoints[ i ];
               maxDistSq = distSq;
               }
            }

         ContactPoint* pfarestFromLine = &m_ContactPoints[ 0 ];
         Vec3 lineDir = ( pfarestFromDeepst->m_PointAWS - pDeepest->m_PointAWS );
         lineDir.Normalize();
         Vec3 v = pfarestFromLine->m_PointAWS - pDeepest->m_PointAWS;
         maxDistSq = ( v - ( v.Dot( lineDir ) * lineDir ) ).LengthSq();
         for( int i = 1; i <= MANIFOLD_MAX_NUM; ++i )
            {
            v = m_ContactPoints[ i ].m_PointAWS - pDeepest->m_PointAWS;
            float distSq = ( v - ( v.Dot( lineDir ) * lineDir ) ).LengthSq();
            if( distSq > maxDistSq )
               {
               pfarestFromLine = &m_ContactPoints[ i ];
               maxDistSq = distSq;
               }
            }

         ContactPoint* pfarestFromTriangle = &m_ContactPoints[ 0 ];
         maxDistSq = DistanceSqFromPointToTriangle( pDeepest->m_PointAWS
                                                    , pfarestFromDeepst->m_PointAWS
                                                    , pfarestFromLine->m_PointAWS
                                                    , pfarestFromTriangle->m_PointAWS );

         for( int i = 1; i <= MANIFOLD_MAX_NUM; ++i )
            {
            float distSq = DistanceSqFromPointToTriangle( pDeepest->m_PointAWS
                                                          , pfarestFromDeepst->m_PointAWS
                                                          , pfarestFromLine->m_PointAWS
                                                          , m_ContactPoints[ i ].m_PointAWS );
            if( distSq > maxDistSq )
               {
               pfarestFromTriangle = &m_ContactPoints[ i ];
               maxDistSq = distSq;
               }
            }
         ContactPoint p1( *pDeepest );
         ContactPoint p2( *pfarestFromDeepst );
         ContactPoint p3( *pfarestFromLine );
         ContactPoint p4( *pfarestFromTriangle );
         m_ContactPoints[ 0 ] = p1;
         m_ContactPoints[ 1 ] = p2;
         m_ContactPoints[ 2 ] = p3;
         m_ContactPoints[ 3 ] = p4;

         // m_ContactPoints[ std::rand() % MANIFOLD_MAX_NUM ] = newPoint;
         }
      }
   }

void Manifold::CalculateCombinedRestitution( void )
   {
   // https://www.gamedev.net/articles/programming/math-and-physics/combining-material-friction-and-restitution-values-r4227/
   const float sqrtOf2 = 1.41421356237f;
   const float rX = m_pRigidBodyA->GetRestitution();
   const float rY = m_pRigidBodyB->GetRestitution();

   float wX = sqrtOf2 * std::abs( 2.f * rX - 1.f ) + 1.f;
   float wY = sqrtOf2 * std::abs( 2.f * rY - 1.f ) + 1.f;
   
   m_CombinedRestitution = ( rX * wX + rY * wY ) / ( wX + wY );
   }

void Manifold::CalculateCombinedFriction( void )
   {
   const float sqrtOf2 = 1.41421356237f;
   const float rX = m_pRigidBodyA->GetFriction();
   const float rY = m_pRigidBodyB->GetFriction();

   float wX = sqrtOf2 * std::abs( 1.f - rX ) + 1.f;
   float wY = sqrtOf2 * std::abs( 1.f - rY ) + 1.f;

   m_CombinedFriction = ( rX * wX + rY * wY ) / ( wX + wY );
   }

bool CollisionDetector::CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, ContactPoint& contact )
   {
   bool hasCollision = false;
   for(auto& pColliderA : pRigidBodyA->m_Colliders)
      {
      for( auto& pColliderB : pRigidBodyB->m_Colliders )
         {
         // Not sure how to generate manifold for composite RB
         // this is not correct for composite RB for now, cause you might generate multiple contact point   
         // Maybe use vector of contact points?
         if( CollisionDetection( pColliderA, pColliderB, contact ) )
            {
            hasCollision = true;
            }
         }
      }
   return hasCollision;
   }

// GJK & EPA algorithm
bool CollisionDetector::CollisionDetection( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, ContactPoint& contact )
   {
   Simplex simplex;
   if( !GJK( pColliderA, pColliderB, simplex ) )
      {
      // No need for EPA
      return false;
      }
   EPA( pColliderA, pColliderB, simplex, contact );
   return true;
   }

void CollisionDetector::GJKUpdateSimplex( Simplex& simplex )
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
         n = ab.Cross( ac );
         }
      // Test vertex D
      Vec3 ad = d - a;
      Vec3 cd = d - c;
      Vec3 bd = d - b;
      if( -d.Dot( -bd ) <= 0.f
          && -d.Dot( -cd ) <= 0.f
          && -d.Dot( -ad ) <= 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 1;
         return;
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
         return;
         }

      Vec3 bc = c - b;
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
         return;
         }
      // Test edge CD
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
         return;
         }
      // Test triangle ADB
      // If origin and the rest point ( c ) are in different face
      // then it's on the triangle
      if( ( -a.Dot( nADB ) ) * ( ac.Dot( nADB )  ) < 0.f )
         {
         simplex.m_Vertice[ 2 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 2 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         return;
         }
      // Test triangle ACD
      if( ( -a.Dot( nACD ) ) * ( ab.Dot( nACD ) ) < 0.f )
         {
         simplex.m_Vertice[ 1 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 1 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         return;
         }
      // Test triangle BDC
      if( ( -b.Dot( nBDC ) ) * ( -ab.Dot( nBDC ) ) < 0.f )
         {
         simplex.m_Vertice[ 0 ] = simplex.m_Vertice[ 3 ];
         // std::swap( simplex.m_Vertice[ 0 ], simplex.m_Vertice[ 3 ] );
         simplex.m_Size = 3;
         return;
         }
      }
   }

bool CollisionDetector::GJKContainsOrigin( Simplex& simplex, Vec3& direction )
   {
   if( simplex.m_Size == 1 )
      {
      auto point = simplex.m_Vertice[ 0 ].m_PointCSO;
      if( point.Dot( point ) < fESPLIONSQ )
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
      if( p.Dot( p ) < fESPLIONSQ )
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
      // just project the origin to the triangle
      // We don't need to worry about the origin will lay outside of the triangle
      // because the simplex has been updated
      // project point p = O + t*n = tn
      // (p dot N) + D = 0
      // (t*N dot N ) + D = 0
      // t = -D / N dot N
      // a dot N + D = 0
      // D = - a dot N
      // t = (a dot N ) / N dot N
      auto nABC = ( b - a ).Cross( c - a );
      float t = a.Dot( nABC ) / nABC.Dot( nABC );
      Vec3 p = t * nABC;
      if( p.Dot( p ) < fESPLIONSQ )
         {
         return true;
         }
      direction = -p;
      direction.Normalize();
      return false;
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
      planeADB.Init( a, d, b );
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
      return false;
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
      GJKUpdateSimplex( simplex );
      // if contains origin, return true, and update direction as well
      if( GJKContainsOrigin( simplex, direction ) )
         {
         return true;
         }
      }
   return false;
   }

class EdgePair
   {
   public:
   EdgePair( shared_ptr< SupportPoint > f, shared_ptr< SupportPoint > s ) : first( f ), second( s )
      {
      }

   shared_ptr< SupportPoint > first;
   shared_ptr< SupportPoint > second;
   };

//  Remove all faces from the polytope that can be ¡§seen¡¨ by this new support point, 
// and add new faces to cover up the ¡§hole¡¨ on the polytope, new support point is the common vertex
void CollisionDetector::EPAExpandPolyhedron( Polyhedron& polyhedron, shared_ptr< SupportPoint > pNewPoint )
   {

   static int c = 0;
   std::unordered_map< shared_ptr< SupportPoint >, std::unordered_set<shared_ptr< SupportPoint >> > edgeMap;
   // For each faces, test the direction
   for( auto pFaceIt = polyhedron.m_Faces.begin(); pFaceIt != polyhedron.m_Faces.end(); )
      {
      // if the face can be seen by the point, try to delete the face
      if( ( *pFaceIt )->m_Plane.IsAbove( pNewPoint->m_PointCSO ) )
         {
         // This face can be seen by the point
         std::vector< shared_ptr< SupportPoint > > verticesInFace = 
            {
            ( *pFaceIt )->m_Vertices[ 0 ].lock(),
            ( *pFaceIt )->m_Vertices[ 1 ].lock(),
            ( *pFaceIt )->m_Vertices[ 2 ].lock()
            };

         // For each vertex in face, clear vertex-face mapping
         for( auto pVertex : verticesInFace )
            {
            ENG_ASSERT( polyhedron.m_VertexToFace.find( pVertex ) != polyhedron.m_VertexToFace.end() );
            ENG_ASSERT( polyhedron.m_VertexToFace[ pVertex ].size() );
            ENG_ASSERT( polyhedron.m_VertexToFace[ pVertex ].find( ( *pFaceIt ) ) != polyhedron.m_VertexToFace[ pVertex ].end() );
            polyhedron.m_VertexToFace[ pVertex ].erase( ( *pFaceIt ) );
            }
         // Erase the face
         ENG_ASSERT( polyhedron.m_Faces.find( ( *pFaceIt ) ) != polyhedron.m_Faces.end() );
         pFaceIt = polyhedron.m_Faces.erase( pFaceIt );
         
         for( int i = 0; i < 3; ++i )
            {
            // for each edge i - j 
            int j = ( i + 1 ) % 3;
            auto findIt = edgeMap.find( verticesInFace[ j ] );
            // if edge j - i does not exist, add edge i - j to temporal edge set 
            if( findIt == edgeMap.end() || ( *findIt ).second.find( verticesInFace[ i ] ) == ( *findIt ).second.end() )
               {
               edgeMap[ verticesInFace[ i ] ].insert( verticesInFace[ j ] );
               }
            // else remove edge j - i
            else
               {
               ( *findIt ).second.erase( verticesInFace[ i ] );
               }
            }
         
         }
      else
         {
         ++pFaceIt;
         }
      }

   // for each of the rest edges these are only added to the set once
   // so new faces are formed by the edge and new point
   for( auto& vertexFrom : edgeMap )
      {
   //   ENG_ASSERT( vertexFrom.second.size() <= 1);
      for( auto& connectVertex : vertexFrom.second )
         {
         ENG_ASSERT( vertexFrom.first->m_PointCSO != connectVertex->m_PointCSO );
         polyhedron.AddFace( vertexFrom.first, connectVertex, pNewPoint );
         }
    //  polyhedron.AddFace( edge.first, edge.second, pNewPoint );
      }
   ++c;
   }

void CollisionDetector::EPAExpandToTetrahedron( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex )
   {
   // Expand the simplex to tetrahedron
   if( simplex.m_Size < 4 )
      {
      if( simplex.m_Size == 1 )
         {
         const Vec3 searchDirs[] =
            {
            g_Right
            , -g_Right
            , g_Up
            , -g_Up
            , g_Forward
            , -g_Forward
            };
         // Search along fixed direction, if the distance is long enough then push it into the simplex
         for(const auto& searchDir : searchDirs)
            {
            auto searchPoint = GetCSOSupportPoint( pColliderA, pColliderB, searchDir );
            auto vec = ( searchPoint.m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO );
            if( vec.Dot( vec ) > fESPLIONSQ )
               {
               simplex.Push( searchPoint );
               break;
               }
            }
         }
      if( simplex.m_Size == 2)
         {
         Vec3 lineDir = simplex.m_Vertice[ 1 ].m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO;
         Vec3 leastSignificantAxis( 1.f, 0.f, 0.f );

         if( std::abs( lineDir.y ) < std::abs( lineDir.z ) && std::abs( lineDir.y ) < std::abs( lineDir.x ) )
            {
            leastSignificantAxis = g_Up;
            }
         else if( std::abs( lineDir.z ) < std::abs( lineDir.y ) && std::abs( lineDir.z ) < std::abs( lineDir.x ) )
            {
            leastSignificantAxis = g_Forward;
            }

         Vec3 searchDir = leastSignificantAxis.Cross( lineDir );
         searchDir.Normalize();
         Mat3x3 rotMat;
         rotMat.BuildAxisRad( searchDir, ENG_PI / 3.f );
         for( int i = 0; i < 6; ++i )
            {
            auto searchPoint = GetCSOSupportPoint( pColliderA, pColliderB, searchDir );
            if( searchPoint.m_PointCSO.Dot( searchPoint.m_PointCSO ) > fESPLIONSQ )
               {
               simplex.Push( searchPoint );
               break;
               }
            searchDir = rotMat * searchDir;
            }
         }

      // size == 3
      const Vec3 ab = simplex.m_Vertice[ 1 ].m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO;
      const Vec3 ac = simplex.m_Vertice[ 2 ].m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO;
      Vec3 searchDir = ab.Cross( ac );
      searchDir.Normalize();
      
      auto searchPoint = GetCSOSupportPoint( pColliderA, pColliderB, searchDir );
      if( searchPoint.m_PointCSO.Dot( searchPoint.m_PointCSO ) > fESPLIONSQ )
         {
         simplex.Push( searchPoint );
         }
      else
         {
         GetCSOSupportPoint( pColliderA, pColliderB, -searchDir );
         simplex.Push( searchPoint );
         }
      }

   ENG_ASSERT( simplex.m_Size == 4 );
   Vec3& pA = simplex.m_Vertice[ 0 ].m_PointCSO;
   Vec3& pB = simplex.m_Vertice[ 1 ].m_PointCSO;
   Vec3& pC = simplex.m_Vertice[ 2 ].m_PointCSO;
   Vec3& pD = simplex.m_Vertice[ 3 ].m_PointCSO;

   // Ensure the order of simplex
   Vec3 ab = pB - pA;
   Vec3 ac = pC - pA;
   Vec3 n = ab.Cross( ac );
   if( n.Dot( pD - pA ) > 0.f )
      {
      std::swap( simplex.m_Vertice[ 1 ], simplex.m_Vertice[ 2 ] );
      }
   }

void CollisionDetector::EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex, ContactPoint& contact )
   {
   GJKContainsOrigin( simplex, g_Up );
   EPAExpandToTetrahedron( pColliderA, pColliderB, simplex );
      
   // Initialize polyhedron
   Polyhedron polyhedron;
   shared_ptr< SupportPoint > a( ENG_NEW SupportPoint( simplex.m_Vertice[ 0 ] ) );
   shared_ptr< SupportPoint > b( ENG_NEW SupportPoint( simplex.m_Vertice[ 1 ] ) );
   shared_ptr< SupportPoint > c( ENG_NEW SupportPoint( simplex.m_Vertice[ 2 ] ) );
   shared_ptr< SupportPoint > d( ENG_NEW SupportPoint( simplex.m_Vertice[ 3 ] ) );

   polyhedron.AddFace( a, b, c );
   polyhedron.AddFace( a, d, b );
   polyhedron.AddFace( a, c, d );
   polyhedron.AddFace( b, d, c );


   float prev_Distance = -std::numeric_limits<float>::max();
   shared_ptr<Face> best_face;
   const int maxItNum = 100;
   int it = 0;
   while( true )
      {
      auto faceIt = polyhedron.m_Faces.begin();
      best_face = *faceIt;
      //  Find closest face of the polytope to the origin.
      for( std::advance(faceIt, 1); faceIt != polyhedron.m_Faces.end(); ++faceIt )
         {
         // All faces are facing outward, and we wanna find the face which is closest to original
         if( best_face->m_Plane.GetD() < (*faceIt)->m_Plane.GetD() )
            {
            best_face = ( *faceIt );
            }
         }
     // Vec3 out( prev_Distance, best_face->m_Plane.GetD(), std::numeric_limits<float>::signaling_NaN() );
      // ENG_LOG( "Test", ToStr( out ));
      //  If the closest face is no larger by a threshold  
      // to the origin than the previously picked one, break;
      if( std::abs( prev_Distance - best_face->m_Plane.GetD() ) <= fESPLION || it >= maxItNum)
         {
         break;
         }
      prev_Distance = best_face->m_Plane.GetD();
      // Find new point;
      shared_ptr< SupportPoint > newPoint( ENG_NEW SupportPoint( GetCSOSupportPoint( pColliderA, pColliderB, best_face->m_Plane.GetNormal() ) ) );
      if(best_face->m_Vertices[0].lock()->m_PointCSO == newPoint->m_PointCSO
          || best_face->m_Vertices[ 1 ].lock()->m_PointCSO == newPoint->m_PointCSO 
          || best_face->m_Vertices[ 2 ].lock()->m_PointCSO == newPoint->m_PointCSO )
         {
         break;
         }
      EPAExpandPolyhedron( polyhedron, newPoint );
      ++it;
      }

   //  Project the origin onto the closest triangle.
   // auto projPoint = best_face->m_Plane.GetProjectPoint(Vec3::g_Zero);
   // auto projPoint = -best_face->m_Plane.GetD() * best_face->m_Plane.GetNormal();
   // Compute the barycentric coordinates of this closest point using the vertices from this triangle.
   auto baryCoords = best_face->FindBarycentricCoords( Vec3::g_Zero );
   auto pA = ( best_face->m_Vertices[ 0 ].lock() );
   auto pB = ( best_face->m_Vertices[ 1 ].lock() );
   auto pC = ( best_face->m_Vertices[ 2 ].lock() );

   // ENG_ASSERT( baryCoords.x > 0.f && baryCoords.x < 1.f );
   // ENG_ASSERT( baryCoords.y > 0.f && baryCoords.y < 1.f );
   // ENG_ASSERT( baryCoords.z > 0.f && baryCoords.z < 1.f );

   SupportPoint spPoint( baryCoords.x * pA->m_PointA
                              + baryCoords.y * pB->m_PointA
                              + baryCoords.z * pC->m_PointA
                              
                              , baryCoords.x * pA->m_PointB
                              + baryCoords.y * pB->m_PointB
                              + baryCoords.z * pC->m_PointB );
   auto pRBA = pColliderA->VGetRigidBody().lock();
   auto pRBB = pColliderB->VGetRigidBody().lock();

   contact.m_PointAWS = baryCoords.x * pA->m_PointA + baryCoords.y * pB->m_PointA + baryCoords.z * pC->m_PointA;

   contact.m_PointBWS = baryCoords.x * pA->m_PointB + baryCoords.y * pB->m_PointB + baryCoords.z * pC->m_PointB;


   contact.m_Normal = best_face->m_Plane.GetNormal(); 

   contact.m_PenetrationDepth = std::abs( best_face->m_Plane.GetD() );
   contact.Update( pRBA, pRBB );
   }

SupportPoint CollisionDetector::GetCSOSupportPoint( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Vec3& direction )
   {
   // The input is in world space, must convert it to local space then convert it back
   auto pRigidBodyA = pColliderA->VGetRigidBody().lock();
   auto pRigidBodyB = pColliderB->VGetRigidBody().lock();
   ENG_ASSERT( pRigidBodyA && pRigidBodyB );

   Vec3 pA( pColliderA->VSupportMapping( pRigidBodyA->VTransformToLocal( direction, false ) ) );
   Vec3 pB( pColliderB->VSupportMapping( pRigidBodyB->VTransformToLocal( -direction, false ) ) );

   pA = pRigidBodyA->VTransformToGlobal( pA, true );
   pB = pRigidBodyB->VTransformToGlobal( pB, true );

   return SupportPoint( pA - pB, pA, pB );
   }