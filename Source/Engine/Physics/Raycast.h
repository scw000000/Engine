#pragma once
/*!
 * \file Raycast.h
 * \date 2016/06/25 14:16
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */


class Intersection
   {
   public:
      float m_Dist;                  // distance from ray origin to intersection
      DWORD m_dwFace;					// the face index of the intersection
      float m_Bary1;
      float m_Bary2;		// Barycentric coordinates of the intersection
      float m_Tu;
      float m_Tv;               // texture coords of intersection
      ActorId m_ActorId;				// Which actor was intersected if there was one
      WeakRenderComponentPtr pWeakRenderComp;
      Vec3 m_WorldLoc;				// world location of intersection
      Vec3 m_ActorLoc;				// actor local coordinates of intersection
      Vec3 m_Normal;					// normal of intersection

      bool const operator < ( Intersection const &other ) { return m_Dist < other.m_Dist; }
   };

template <class T>
void InitIntersection( Intersection &intersection, DWORD faceIndex, FLOAT dist, FLOAT u, FLOAT v, ActorId actorId, WORD* pIndices, T* pVertices, const Mat4x4 &matWorld );


class RayCast : public ENG_Noncopyable
   {
   public:
      RayCast( Point point, float distance, unsigned long maxIntersections = 16 );
      RayCast( const Vec3& start, const Vec3& end, unsigned long maxIntersections = 16 );
      RayCast( const Vec3& start, const Vec3& dir, float length, unsigned long maxIntersections = 16 );
      void Sort();

   public:
      unsigned long m_MaxIntersections;
      unsigned long m_NumIntersections;
      bool m_bAllHits;			// Whether to just get the first "hit" or all "hits"

      Vec3 m_RayEnd;
      Vec3 m_RayStart;

      shared_ptr< std::vector< Intersection > > m_pIntersectionArray;

   protected:
      
   };

class RayCastManager : public ENG_Noncopyable
   {
   friend class BulletPhysics;
   public:
      static RayCastManager& GetSingleton( void );
      bool PerformRayCast( RayCast& castResult );

   protected:
      RayCastManager( BulletPhysics* pImp );
      

   protected:
      static RayCastManager* s_pSingleton;
      BulletPhysics* m_pBulletPhysics;
   };