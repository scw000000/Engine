/*!
 * \file Raycast.cpp
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

#include "EngineStd.h"
#include "Raycast.h"
#include "Physics.h"
#include "btBulletDynamicsCommon.h"

RayCastManager* RayCastManager::s_pSingleton = NULL;

template <class T>
void InitIntersection( Intersection &intersection, DWORD faceIndex, FLOAT dist, FLOAT u, FLOAT v, ActorId actorId, WORD* pIndices, T* pVertices, const Mat4x4 &matWorld )
   {
   intersection.m_dwFace = faceIndex;
   intersection.m_fDist = dist;
   intersection.m_fBary1 = u;
   intersection.m_fBary2 = v;

   T *v0 = &pVertices[ pIndices[ 3 * faceIndex + 0 ] ];
   T *v1 = &pVertices[ pIndices[ 3 * faceIndex + 1 ] ];
   T *v2 = &pVertices[ pIndices[ 3 * faceIndex + 2 ] ];

   FLOAT dtu1 = v1->tu - v0->tu;
   FLOAT dtu2 = v2->tu - v0->tu;
   FLOAT dtv1 = v1->tv - v0->tv;
   FLOAT dtv2 = v2->tv - v0->tv;
   intersection.m_Tu = v0->tu + intersection.m_fBary1 * dtu1 + intersection.m_fBary2 * dtu2;
   intersection.m_Tv = v0->tv + intersection.m_fBary1 * dtv1 + intersection.m_fBary2 * dtv2;

   Vec3 a = v0->position - v1->position;
   Vec3 b = v2->position - v1->position;

   Vec3 cross = a.Cross( b );
   cross /= cross.Length();

   Vec3 actorLoc = BarycentricToVec3( v0->position, v1->position, v2->position, intersection.m_fBary1, intersection.m_fBary2 );
   intersection.m_ActorLoc = actorLoc;
   intersection.m_WorldLoc = matWorld.Xform( actorLoc );
   intersection.m_ActorId = actorId;
   intersection.m_Normal = cross;
   }



RayCast::RayCast( Point point, float distance, unsigned int maxIntersections )
   {
   m_MaxIntersections = maxIntersections;
   m_ReturnAllHits = true;
   m_NumIntersections = 0;
   m_RayStart = Vec3::g_Zero;
   m_RayEnd = Vec3::g_Zero;
   g_pApp->m_pEngineLogic->m_pWrold->GetCamera()->GetScreenProjectPoint( m_RayStart, m_RayEnd, point, distance );
   m_IsSorted = false;
   }

RayCast::RayCast( const Vec3& start, const Vec3& end, unsigned int maxIntersections )
   {
   m_MaxIntersections = maxIntersections;
   m_ReturnAllHits = true;
   m_NumIntersections = 0;
   m_RayStart = start;
   m_RayEnd = end;
   m_IsSorted = false;
   }

RayCast::RayCast( const Vec3& start, const Vec3& dir, float length, unsigned int maxIntersections )
   {
   m_MaxIntersections = maxIntersections;
   m_ReturnAllHits = true;
   m_NumIntersections = 0;
   m_RayStart = start;
   Vec3 tempDir( dir );
   tempDir.Normalize();
   m_RayEnd = start + tempDir * length;
   m_IsSorted = false;
   }

void RayCast::Sort( void )
   {
   if( !m_IsSorted )
      {
      m_IsSorted = true;
      std::vector< Intersection >& interscetArray = *m_pIntersectionArray;
      std::sort( interscetArray.begin(), interscetArray.end() );
      }
   }


////
////HRESULT RayCast::Pick( Scene *pScene, ActorId actorId, LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DINDEXBUFFER9 pIB, DWORD numPolys )
////   {
////   if( !m_bAllHits && m_NumIntersections > 0 )
////      return S_OK;
////
////   WORD* pIndices;
////   D3D9Vertex_ColoredTextured* pVertices;
////
////   pIB->Lock( 0, 0, ( void** ) &pIndices, 0 );
////   pVB->Lock( 0, 0, ( void** ) &pVertices, 0 );
////
////   IDirect3DDevice9* pD3Device = DXUTGetD3D9Device();
////
////   // Get the inverse view matrix
////   const Mat4x4 matView = pScene->GetCamera()->GetView();
////   const Mat4x4 matWorld = pScene->GetTopMatrix();
////   const Mat4x4 proj = pScene->GetCamera()->GetProjection();
////
////   // Compute the vector of the Pick ray in screen space
////   D3DXVECTOR3 v;
////   v.x = ( ( ( 2.0f * m_Point.x ) / g_pApp->GetScreenSize().x ) - 1 ) / proj._11;
////   v.y = -( ( ( 2.0f * m_Point.y ) / g_pApp->GetScreenSize().y ) - 1 ) / proj._22;
////   v.z = 1.0f;
////
////
////   D3DXMATRIX mWorldView = matWorld * matView;
////   D3DXMATRIX m;
////   D3DXMatrixInverse( &m, NULL, &mWorldView );
////
////   // Transform the screen space Pick ray into 3D space
////   m_vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
////   m_vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
////   m_vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
////   m_vPickRayOrig.x = m._41;
////   m_vPickRayOrig.y = m._42;
////   m_vPickRayOrig.z = m._43;
////
////   FLOAT fBary1, fBary2;
////   FLOAT fDist;
////   for( DWORD i = 0; i < numPolys; i++ )
////      {
////      Vec3 v0 = pVertices[ pIndices[ 3 * i + 0 ] ].position;
////      Vec3 v1 = pVertices[ pIndices[ 3 * i + 1 ] ].position;
////      Vec3 v2 = pVertices[ pIndices[ 3 * i + 2 ] ].position;
////
////      // Check if the Pick ray passes through this point
////      if( IntersectTriangle( m_vPickRayOrig, m_vPickRayDir, v0, v1, v2,
////         &fDist, &fBary1, &fBary2 ) )
////         {
////         if( m_bAllHits || m_NumIntersections == 0 || fDist < m_IntersectionArray[ 0 ].m_fDist )
////            {
////
////            if( !m_bAllHits )
////               m_NumIntersections = 0;
////
////            ++m_NumIntersections;
////
////            m_IntersectionArray.resize( m_NumIntersections );
////
////            Intersection* pIntersection;
////            pIntersection = &m_IntersectionArray[ m_NumIntersections - 1 ];
////
////            InitIntersection( *pIntersection, i, fDist, fBary1, fBary2, actorId, pIndices, pVertices, matWorld );
////
////            if( m_NumIntersections == m_MaxIntersections )
////               break;
////            }
////         }
////      }
////
////   pVB->Unlock();
////   pIB->Unlock();
////
////   return S_OK;
////   }
//
//
//
//
//void RayCast::Sort()
//   {
//   std::sort( m_IntersectionArray.begin(), m_IntersectionArray.end() );
//   }

RayCastManager& RayCastManager::GetSingleton( void )
   {
   ENG_ASSERT( s_pSingleton );
   return *s_pSingleton;
   }

bool RayCastManager::PerformRayCast( RayCast& castResult )
   {
   btVector3 rayStart = Vec3_to_btVector3( castResult.m_RayStart );
   btVector3 rayEnd = Vec3_to_btVector3( castResult.m_RayEnd );
   btCollisionWorld::AllHitsRayResultCallback rayCallback( rayStart, rayEnd );

   if( !m_pBulletPhysics->m_DynamicsWorld || !g_pApp->m_pEngineLogic->m_pWrold->GetCamera() )
      {
      return false;
      }

   m_pBulletPhysics->m_DynamicsWorld->rayTest( rayStart, rayEnd, rayCallback );
 
   if( rayCallback.hasHit() )
      {
      unsigned int hitNum = rayCallback.m_collisionObjects.size();
      castResult.m_pIntersectionArray.reset( ENG_NEW std::vector< Intersection >( hitNum ) );
      int size = castResult.m_pIntersectionArray->size();
      int cap = castResult.m_pIntersectionArray->capacity();
      for( unsigned long i = 0; i < hitNum; ++i )
         {
         btRigidBody * pRigidBody = ( btRigidBody * ) rayCallback.m_collisionObjects[ i ];

         auto pRenderComp = m_pBulletPhysics->FindRenderComponent( pRigidBody );
         Intersection& currIntersection = ( *castResult.m_pIntersectionArray )[ i ];
         currIntersection.pWeakRenderComp = pRenderComp;
         currIntersection.m_ActorId = pRenderComp->VGetOwner().lock()->GetId();
         currIntersection.m_WorldLoc = btVector3_to_Vec3( rayCallback.m_hitPointWorld[ i ] );
         currIntersection.m_Normal = btVector3_to_Vec3( rayCallback.m_hitNormalWorld[ i ] );
         currIntersection.m_Dist = rayCallback.m_hitPointWorld[ i ].distance( rayCallback.m_rayFromWorld );
         }

      if( castResult.m_MaxIntersections < ( unsigned ) hitNum )
         {
         castResult.Sort();
         castResult.m_pIntersectionArray->resize( std::min( hitNum, castResult.m_MaxIntersections ) );
         int size = castResult.m_pIntersectionArray->size();
         }
      castResult.m_NumIntersections = castResult.m_pIntersectionArray->size();
      }
   return true;
   }

RayCastManager::RayCastManager( BulletPhysics* pImp )
   {
   ENG_ASSERT( pImp );
   m_pBulletPhysics = pImp;
   s_pSingleton = this;
   }




