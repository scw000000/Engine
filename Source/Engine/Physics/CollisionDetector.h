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

struct ContactPoint
   {
   public:
      Vec3 m_Normal;
      float m_PenetrationDepth;
   };

struct Manifold
   {
   public:
      shared_ptr<RigidBody> pRigidBodyA;
      shared_ptr<RigidBody> pRigidBodyB;
      int m_ContactPointCount;
      ContactPoint m_ContactPoints[ MANIFOLD_MAX_NUM ];
   };

struct SupportPoint 
   {
   public:
   SupportPoint( const Vec3& pCSO, const Vec3& pA, const Vec3& pB) 
      : m_PointCSO( pCSO ), m_PointA( pA ), m_PointB( pB ){ }

   public:
   Vec3 m_PointCSO;
   Vec3 m_PointA;
   Vec3 m_PointB;
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

class CollisionDetector
   {
   public:
      bool CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, Manifold& manifold );
   
   private:
   bool CollisionDetection( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Manifold& manifold );
   void UpdateSimplex( Simplex& simplex );
   bool ContainsOrigin( Simplex& simplex, Vec3& direction );
   bool GJK( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, Simplex& simplex );
   void EPA( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Simplex& simplex, Manifold& manifold );
   SupportPoint GetCSOSupportPoint( shared_ptr<ICollider> pColliderA, shared_ptr<ICollider> pColliderB, const Vec3& direction );
   };