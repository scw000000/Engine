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
const float g_EsplionSq = g_Esplion * g_Esplion;

ContactPoint::ContactPoint( const SupportPoint& supportPoint ) 
   : m_SupportPoint( supportPoint )
   {
   auto vBA = supportPoint.m_PointA - supportPoint.m_PointB;
   // calculate normal and depth
   m_PenetrationDepth = vBA.Length();
   // For rigid body A. since A, B are colliding, the 
   // Distance from centriod of A to contact point B is shorter than to contact point A
   // so the normal becomes vector BA = a - b
   vBA.Normalize();
   m_Normal = vBA;
   }

Vec3 Face::FindBarycentricCoords( const Vec3& point )
   {
   const Vec3& a = m_Vertices[ 0 ].lock()->m_PointCSO;
   const Vec3& b = m_Vertices[ 1 ].lock()->m_PointCSO;
   const Vec3& c = m_Vertices[ 2 ].lock()->m_PointCSO;

   float va = m_Plane.GetNormal().Dot( ( b - point ).Cross( c - point ) );
   float vb = m_Plane.GetNormal().Dot( ( c - point ).Cross( a - point ) );
   float vc = m_Plane.GetNormal().Dot( ( a - point ).Cross( b - point ) );

   float u = va / ( va + vb + vc );
   float v = vb / ( va + vb + vc );
   return Vec3( u, v, 1.f - u - v );
   }


void Manifold::AddContactPoint( const SupportPoint& newPoint )
   {
   ENG_ASSERT( m_ContactPointCount < MANIFOLD_MAX_NUM );
   m_ContactPoints[ m_ContactPointCount++ ] = ContactPoint( newPoint );
   }

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
      if( point.Dot( point ) < g_EsplionSq )
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
      if( p.Dot( p ) < g_EsplionSq )
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
      if( p.Dot( p ) < g_EsplionSq )
         {
         return true;
         }
      direction = -p;
      direction.Normalize();
      return false;

      ///////
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

      p = u * a + v * b + w * c;
      if( p.Dot( p ) < g_EsplionSq )
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
   std::unordered_map< shared_ptr< SupportPoint >, std::unordered_set<shared_ptr< SupportPoint >> > edgeMap;
   // For each faces, test the direction
   for( auto pFaceIt = polyhedron.m_Faces.begin(); pFaceIt != polyhedron.m_Faces.end(); )
      {
      // if the face can be seen by the point, try to delete the face
      if( ( *pFaceIt )->m_Plane.IsAbove( pNewPoint->m_PointCSO ) )
         {
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
            int j = ( i + 1 ) % 3;
            auto findIt = edgeMap.find( verticesInFace[ j ] );
            // for each edge in reverse order, add to temporal edge set if it doesn't exist
            if( findIt == edgeMap.end() || ( *findIt ).second.find( verticesInFace[ i ] ) == ( *findIt ).second.end() )
               {
               edgeMap[ verticesInFace[ i ] ].insert( verticesInFace[ j ] );
               }
            // else remove it
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
      for( auto& connectVertex : vertexFrom.second )
         {
         polyhedron.AddFace( vertexFrom.first, connectVertex, pNewPoint );
         }
    //  polyhedron.AddFace( edge.first, edge.second, pNewPoint );
      }
   
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
         for(const auto& searchDir : searchDirs)
            {
            auto searchPoint = GetCSOSupportPoint( pColliderA, pColliderB, searchDir );
            auto vec = ( searchPoint.m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO );
            if( vec.Dot( vec ) > g_EsplionSq )
               {
               simplex.Push( searchPoint );
               }
            }
         }
      if( simplex.m_Size == 2)
         {
         Vec3 lineDir = simplex.m_Vertice[ 1 ].m_PointCSO - simplex.m_Vertice[ 0 ].m_PointCSO;
         Vec3 lestSignificantAxis( 1.f, 0.f, 0.f );

         if( std::abs( lineDir.y ) < std::abs( lineDir.z ) && std::abs( lineDir.y ) < std::abs( lineDir.x ) )
            {
            lestSignificantAxis = g_Up;
            
            }
         else if( std::abs( lineDir.z ) < std::abs( lineDir.y ) && std::abs( lineDir.z ) < std::abs( lineDir.x ) )
            {
            lestSignificantAxis = g_Forward;
            }

         Vec3 searchDir = lestSignificantAxis.Cross( lineDir );
         searchDir.Normalize();
         Mat3x3 rotMat;
         rotMat.BuildAxisRad( searchDir, ENG_PI / 3.f );
         for( int i = 0; i < 6; ++i )
            {
            auto searchPoint = GetCSOSupportPoint( pColliderA, pColliderB, searchDir );
            if( searchPoint.m_PointCSO.Dot( searchPoint.m_PointCSO ) > g_EsplionSq )
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
      if( searchPoint.m_PointCSO.Dot( searchPoint.m_PointCSO ) > g_EsplionSq )
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

void CollisionDetector::EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex, Manifold& manifold )
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
   while( true )
      {
      auto faceIt = polyhedron.m_Faces.begin();
      best_face = *faceIt;
      //  Find closest face of the polytope to the origin.
      for( std::advance(faceIt, 1); faceIt != polyhedron.m_Faces.end(); ++faceIt )
         {
         // Since all faces are facing outward, we wanna find the largest negative value
         if( best_face->m_Plane.GetD() > (*faceIt)->m_Plane.GetD() )
            {
            best_face = ( *faceIt );
            }
         }
      //  If the closest face is no closer by a threshold  
      // to the origin than the previously picked one, break;
      if( std::abs( prev_Distance ) - std::abs( best_face->m_Plane.GetD() ) <= g_Esplion )
         {
         break;
         }
      prev_Distance = best_face->m_Plane.GetD();
      // Find new point;
      shared_ptr< SupportPoint > newPoint( ENG_NEW SupportPoint( GetCSOSupportPoint( pColliderA, pColliderB, best_face->m_Plane.GetNormal() ) ) );
      EPAExpandPolyhedron( polyhedron, newPoint );
      }

   //  Project the origin onto the closest triangle.
   // auto projPoint = best_face->m_Plane.GetProjectPoint(Vec3::g_Zero);
   auto projPoint = -best_face->m_Plane.GetD() * best_face->m_Plane.GetNormal();
   // Compute the barycentric coordinates of this closest point using the vertices from this triangle.
   auto baryCoords = best_face->FindBarycentricCoords( projPoint );
   auto pA = ( best_face->m_Vertices[ 0 ].lock() );
   auto pB = ( best_face->m_Vertices[ 1 ].lock() );
   auto pC = ( best_face->m_Vertices[ 2 ].lock() );

   SupportPoint contactPoint( baryCoords.x * pA->m_PointA
                              + baryCoords.y * pB->m_PointA
                              + baryCoords.z * pC->m_PointA
                              
                              , baryCoords.x * pA->m_PointB
                              + baryCoords.y * pB->m_PointB
                              + baryCoords.z * pC->m_PointB );

   manifold.AddContactPoint( contactPoint );
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