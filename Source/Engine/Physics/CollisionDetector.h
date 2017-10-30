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

class Simplex
   {
   public:
   Simplex( void ) : m_Size(0){ }

   void Push(const Vec3& v)
      {
      ENG_ASSERT( m_Size < 4 );
      m_Vertice[ m_Size++ ] = v;
      }

   void Set( const Vec3& a)
      {
      m_Size = 1;
      m_Vertice[ 0 ] = a;
      }
   void Set( const Vec3& a, const Vec3& b )
      {
      m_Size = 2;
      m_Vertice[ 0 ] = a;
      m_Vertice[ 1 ] = b;
      }
   void Set( const Vec3& a, const Vec3& b, const Vec3& c)
      {
      m_Size = 3;
      m_Vertice[ 0 ] = a;
      m_Vertice[ 1 ] = b;
      m_Vertice[ 2 ] = c;
      }
   void Set( const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d)
      {
      m_Size = 4;
      m_Vertice[ 0 ] = a;
      m_Vertice[ 1 ] = b;
      m_Vertice[ 2 ] = c;
      m_Vertice[ 3 ] = d;
      }
   Vec3 m_Vertice[ 4 ];
   int m_Size;

   };

class CollisionDetector
   {
   public:
      bool CollisionDetection( shared_ptr<RigidBody> pRigidBodyA, shared_ptr<RigidBody> pRigidBodyB, Manifold& manifold );
   
   private:
      
      
   };