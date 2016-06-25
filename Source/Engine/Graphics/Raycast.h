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
      float m_tu;
      float m_tv;               // texture coords of intersection
      ActorId m_actorId;				// Which actor was intersected if there was one
      Vec3 m_worldLoc;				// world location of intersection
      Vec3 m_actorLoc;				// actor local coordinates of intersection
      Vec3 m_normal;					// normal of intersection

      bool const operator < ( Intersection const &other ) { return m_Dist < other.m_Dist; }
   };

template <class T>
void InitIntersection( Intersection &intersection, DWORD faceIndex, FLOAT dist, FLOAT u, FLOAT v, ActorId actorId, WORD* pIndices, T* pVertices, const Mat4x4 &matWorld );


typedef std::vector<Intersection> IntersectionArray;
//
//class RayCast
//   {
//   public:
//      RayCast( Point point, unsigned long maxIntersections = 16 );
//
//      HRESULT Pick( Scene *pScene, ActorId actorId, aiScene* pAiScene );
//
//      HRESULT Pick( Scene *pScene, ActorId actorId, LPDIRECT3DVERTEXBUFFER9 pVerts, LPDIRECT3DINDEXBUFFER9 pIndices, DWORD numPolys );
//
//      void Sort();
//
//   public:
//      unsigned long m_MaxIntersections;
//      unsigned long m_NumIntersections;
//      bool m_bUseD3DXIntersect;      // Whether to use D3DXIntersect
//      bool m_bAllHits;			// Whether to just get the first "hit" or all "hits"
//      Point m_Point;
//
//      Vec3 m_vPickRayDir;
//      Vec3 m_vPickRayOrig;
//
//      IntersectionArray m_IntersectionArray;
//
//   protected:
//      LPDIRECT3DVERTEXBUFFER9     m_pVB;
//   };