#pragma once
/*!
 * \file CollisionDetector.h
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
#define MANIFOLD_MAX_NUM 4 

class RigidBody;
class ICollider;

struct SupportPoint
   {
   public:
      SupportPoint( const Vec3& pCSO, const Vec3& pA, const Vec3& pB )
         : m_PointCSO( pCSO ), m_PointA( pA ), m_PointB( pB )
         {}
      SupportPoint( const Vec3& pA, const Vec3& pB )
         : m_PointCSO( pA - pB ), m_PointA( pA ), m_PointB( pB )
         {}
      SupportPoint( void ) {}
      ;

    public:
      Vec3 m_PointCSO;
      Vec3 m_PointA;
      Vec3 m_PointB;
   };

struct ContactPoint
   {
   public:
      ContactPoint( void );
      // ContactPoint( const SupportPoint& supportPoint, const Vec3& normal, const Vec3& ra, const Vec3& rb );
      bool IsValid( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB ) const;
      void Update( shared_ptr<IRigidBody> pRigidBodyA, shared_ptr<IRigidBody> pRigidBodyB );
      float m_PenetrationDepth;
      float m_AccumulatedImpulseN;
      float m_AccumulatedImpulseT;
      float m_AccumulatedImpulseBT;

      Vec3 m_Normal;
      Vec3 m_Tangent;
      Vec3 m_Bitangent;
      Vec3 m_PointAWS;
      Vec3 m_PointALS;
      Vec3 m_PointBWS;
      Vec3 m_PointBLS;
      Vec3 m_RA;
      Vec3 m_RB;
      Vec3 m_NRACrossN;
      Vec3 m_RBCrossN;
      Vec3 m_NRACrossT;
      Vec3 m_RBCrossT;
      Vec3 m_NRACrossBT;
      Vec3 m_RBCrossBT;
   };

struct Manifold
   {
   public:
      Manifold( void ) : m_ContactPointCount( 0 ), m_CombinedRestitution(0.f){}
      shared_ptr<RigidBody> m_pRigidBodyA;
      shared_ptr<RigidBody> m_pRigidBodyB;
      int m_ContactPointCount;
      ContactPoint m_ContactPoints[ MANIFOLD_MAX_NUM + 1 ];
      float m_CombinedRestitution;
      float m_CombinedFriction;
      void AddContactPoint( const ContactPoint& newPoint );
      void CalculateCombinedRestitution( void );
      void CalculateCombinedFriction( void );
   };



class Simplex
   {
   public:
      Simplex( void ) : m_Size(0){ }

      SupportPoint Last( void ) { return m_Vertice[ std::max( 0, m_Size - 1 ) ]; }
   
      void Push( const SupportPoint& v )
         {
         ENG_ASSERT( m_Size < 4 );
         m_Vertice[ m_Size++ ] = v;
         }

      void Set( const SupportPoint& a )
         {
         m_Size = 1;
         m_Vertice[ 0 ] = a;
         }
         
      void Set( const SupportPoint& a, const SupportPoint& b )
         {
         m_Size = 2;
         m_Vertice[ 0 ] = a;
         m_Vertice[ 1 ] = b;
         }
         
      void Set( const SupportPoint& a, const SupportPoint& b, const SupportPoint& c )
         {
         m_Size = 3;
         m_Vertice[ 0 ] = a;
         m_Vertice[ 1 ] = b;
         m_Vertice[ 2 ] = c;
         }
      
      void Set( const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d )
         {
         m_Size = 4;
         m_Vertice[ 0 ] = a;
         m_Vertice[ 1 ] = b;
         m_Vertice[ 2 ] = c;
         m_Vertice[ 3 ] = d;
         }
         
      SupportPoint m_Vertice[ 4 ];
      int m_Size;

   };

struct Face {
   Face( shared_ptr<SupportPoint> a, shared_ptr<SupportPoint> b, shared_ptr<SupportPoint> c )
      {
      m_Vertices[ 0 ] = a;
      m_Vertices[ 1 ] = b;
      m_Vertices[ 2 ] = c;
      m_Plane.Init( a->m_PointCSO, b->m_PointCSO, c->m_PointCSO );
      }

   // The x, y z are the scalars for vertex a, b, and c
   Vec3 FindBarycentricCoords( const Vec3& point );

   Plane m_Plane;
   // These vertices must be ordered in CCW order
   weak_ptr<SupportPoint> m_Vertices[ 3 ];
   };

class Polyhedron
   {
   public:
      std::unordered_map< shared_ptr< SupportPoint >, std::unordered_set< shared_ptr<Face> > > m_VertexToFace;
      std::unordered_set< shared_ptr<Face> > m_Faces;

      // The order must be CCW
      void AddFace( shared_ptr< SupportPoint > a, shared_ptr< SupportPoint > b, shared_ptr< SupportPoint > c )
         {
         shared_ptr<Face> face( ENG_NEW Face( a, b, c ) );
         m_Faces.insert( face );
         m_VertexToFace[ a ].insert( face );
         m_VertexToFace[ b ].insert( face );
         m_VertexToFace[ c ].insert( face );
         }
   };

class CollisionDetector
   {
   public:
      bool CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, ContactPoint& contact );
   
   private:
      bool CollisionDetection( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, ContactPoint& contact );
   
      void GJKUpdateSimplex( Simplex& simplex );
      bool GJKContainsOrigin( Simplex& simplex, Vec3& direction );
      bool GJK( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex );

      void EPAExpandPolyhedron( Polyhedron& polyhedron, shared_ptr< SupportPoint > pNewPoint );
      void EPAExpandToTetrahedron( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex );
      void EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex, ContactPoint& contact );
      SupportPoint GetCSOSupportPoint( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Vec3& direction );
   };