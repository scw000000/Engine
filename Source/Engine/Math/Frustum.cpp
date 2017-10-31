/*!
 * \file Frustum.cpp
 * \date 2016/10/29 11:53
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "EngineStd.h"
#include "Frustum.h"

bool BaseFrustum::VInside( const Vec3 &point ) const
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Planes[ i ].IsAbove( point ) )
         {
         return false;
         }
      }
   return true;
   }

bool BaseFrustum::VInside( const Vec3 &pointFrom, const Vec3 &pointTo ) const
   {
   int intersecCount = 0;
   bool fromInside = true;
   bool toInside = true;
   for( int i = 0; i < NumPlanes; ++i )
      {
      bool fromTest = m_Planes[ i ].IsAbove( pointFrom );
      bool toTest = m_Planes[ i ].IsAbove( pointTo );
      if( !fromTest )
         {
         fromInside = false;
         }
      if( !toTest )
         {
         toInside = false;
         }
      // They are in different side of the plane
      if( fromTest ^ toTest )
         {
         ++intersecCount;
         }
      }
   return fromInside || toInside || ( intersecCount == 2 );
   }

bool BaseFrustum::VInside( const Vec3 &point, float radius ) const
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Planes[ i ].IsAbove( point, radius ) )
         {
         return false;
         }
      }
   return true;
   }

PerspectiveFrustum::PerspectiveFrustum( void )
   {
   m_FovY = ENG_QUARTERPI;
   m_Aspect = 4.0f / 3.0f;
   m_NearDis = 1.0f;
   m_FarDis = 1000.0f;
   }

void PerspectiveFrustum::Init( const float fovY, const float aspect, const float nearClipDis, const float farClipDis )
   {
   m_FovY = fovY;
   m_Aspect = aspect;
   m_NearDis = nearClipDis;
   m_FarDis = farClipDis;
   // in half radius
   double tanFovOver2 = tan( fovY / 2.0f );
   Vec3 nearRight = static_cast< float >( m_NearDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 farRight = static_cast< float >( m_FarDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 nearUp = static_cast< float >( m_NearDis * tanFovOver2 ) * g_Up;
   Vec3 farUp = static_cast< float >( m_FarDis * tanFovOver2 ) * g_Up;

   // 0 ---------- 1
   // |            |
   // |            |
   // 3 ---------- 2
   // left up
   m_PlaneVerts[ 0 ] = ( m_NearDis * g_Forward ) - nearRight + nearUp;
   // right up
   m_PlaneVerts[ 1 ] = ( m_NearDis * g_Forward ) + nearRight + nearUp;
   // right down
   m_PlaneVerts[ 2 ] = ( m_NearDis * g_Forward ) + nearRight - nearUp;
   // left down
   m_PlaneVerts[ 3 ] = ( m_NearDis * g_Forward ) - nearRight - nearUp;

   // 4 ---------- 5
   // |            |
   // |            |
   // 7 ---------- 6
   // left up
   m_PlaneVerts[ 4 ] = ( m_FarDis * g_Forward ) - farRight + farUp;
   // right up
   m_PlaneVerts[ 5 ] = ( m_FarDis * g_Forward ) + farRight + farUp;
   // right down
   m_PlaneVerts[ 6 ] = ( m_FarDis * g_Forward ) + farRight - farUp;
   // left down
   m_PlaneVerts[ 7 ] = ( m_FarDis * g_Forward ) - farRight - farUp;

   Vec3 origin( 0.0f, 0.0f, 0.0f );
   
   // The planes should point inward
   m_Planes[ Near ].Init( m_PlaneVerts[ 0 ], m_PlaneVerts[ 1 ], m_PlaneVerts[ 2 ] );
   m_Planes[ Far ].Init( m_PlaneVerts[ 6 ], m_PlaneVerts[ 5 ], m_PlaneVerts[ 4 ] );
   m_Planes[ Right ].Init( origin, m_PlaneVerts[ 5 ], m_PlaneVerts[ 6 ] );
   m_Planes[ Top ].Init( origin, m_PlaneVerts[ 4 ], m_PlaneVerts[ 5 ] );
   m_Planes[ Left ].Init( origin, m_PlaneVerts[ 7 ], m_PlaneVerts[ 4 ] );
   m_Planes[ Bottom ].Init( origin, m_PlaneVerts[ 6 ], m_PlaneVerts[ 7 ] );
   }

OrthogonalFrustum::OrthogonalFrustum( void )
   {
   m_Left = 100.f;
   m_Right = -100.f;
   m_Top = 100.f;
   m_Bottom = -100.f;
   m_FarDis = 200.f;
   m_NearDis = 0.f;
   }

void OrthogonalFrustum::Init( const float left, const float right, const float top, const float bottom, const float farDis, const float nearDis )
   {
   m_Left = left;
   m_Right = right;
   m_Top = top;
   m_Bottom = bottom;
   m_FarDis = farDis;
   m_NearDis = nearDis;

   // left up
   m_PlaneVerts[ 0 ] = Vec3( m_Left, m_Top, m_NearDis );
   // right up
   m_PlaneVerts[ 1 ] = Vec3( m_Right, m_Top, m_NearDis );
   // right down
   m_PlaneVerts[ 2 ] = Vec3( m_Right, m_Bottom, m_NearDis );
   // left down
   m_PlaneVerts[ 3 ] = Vec3( m_Left, m_Bottom, m_NearDis );

   // left up
   m_PlaneVerts[ 4 ] = Vec3( m_Left, m_Top, m_FarDis );
   // right up
   m_PlaneVerts[ 5 ] = Vec3( m_Right, m_Top, m_FarDis );
   // right down
   m_PlaneVerts[ 6 ] = Vec3( m_Right, m_Bottom, m_FarDis );
   // left down
   m_PlaneVerts[ 7 ] = Vec3( m_Left, m_Bottom, m_FarDis );

   Vec3 origin( 0.0f, 0.0f, 0.0f );

   m_Planes[ Near ].Init( m_PlaneVerts[ 0 ], m_PlaneVerts[ 1 ], m_PlaneVerts[ 2 ] );
   m_Planes[ Far ].Init( m_PlaneVerts[ 6 ], m_PlaneVerts[ 5 ], m_PlaneVerts[ 4 ] );
   m_Planes[ Right ].Init( m_PlaneVerts[ 1 ], m_PlaneVerts[ 5 ], m_PlaneVerts[ 6 ] );
   m_Planes[ Top ].Init( m_PlaneVerts[ 1 ], m_PlaneVerts[ 4 ], m_PlaneVerts[ 5 ] );
   m_Planes[ Left ].Init( m_PlaneVerts[ 0 ], m_PlaneVerts[ 7 ], m_PlaneVerts[ 4 ] );
   m_Planes[ Bottom ].Init( m_PlaneVerts[ 2 ], m_PlaneVerts[ 6 ], m_PlaneVerts[ 7 ] );
   }