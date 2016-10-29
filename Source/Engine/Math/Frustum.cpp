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
      if( !m_Planes[ i ].Inside( point ) )
         {
         return false;
         }
      }
   return true;
   }

bool BaseFrustum::VInside( const Vec3 &point, float radius ) const
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Planes[ i ].Inside( point, radius ) )
         {
         return false;
         }
      }
   return true;
   }

PerspectiveFrustum::PerspectiveFrustum( void )
   {
   m_Fov = ENG_QUARTERPI;
   m_Aspect = 4.0f / 3.0f;
   m_NearDis = 1.0f;
   m_FarDis = 1000.0f;
   }

void PerspectiveFrustum::Init( const float fov, const float aspect, const float nearClipDis, const float farClipDis )
   {
   m_Fov = fov;
   m_Aspect = aspect;
   m_NearDis = nearClipDis;
   m_FarDis = farClipDis;
   // Fov = 90 -> angle +- 45 degree
   double tanFovOver2 = tan( fov / 2.0f );
   Vec3 nearRight = static_cast< float >( m_NearDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 farRight = static_cast< float >( m_FarDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 nearUp = static_cast< float >( m_NearDis * tanFovOver2 ) * g_Up;
   Vec3 farUp = static_cast< float >( m_FarDis * tanFovOver2 ) * g_Up;

   // These vertiCes start in upper right and go around clockwise
   m_NearPlaneVerts[ 0 ] = ( m_NearDis * g_Forward ) - nearRight + nearUp;
   m_NearPlaneVerts[ 1 ] = ( m_NearDis * g_Forward ) + nearRight + nearUp;
   m_NearPlaneVerts[ 2 ] = ( m_NearDis * g_Forward ) + nearRight - nearUp;
   m_NearPlaneVerts[ 3 ] = ( m_NearDis * g_Forward ) - nearRight - nearUp;

   m_FarPlaneVerts[ 0 ] = ( m_FarDis * g_Forward ) - farRight + farUp;
   m_FarPlaneVerts[ 1 ] = ( m_FarDis * g_Forward ) + farRight + farUp;
   m_FarPlaneVerts[ 2 ] = ( m_FarDis * g_Forward ) + farRight - farUp;
   m_FarPlaneVerts[ 3 ] = ( m_FarDis * g_Forward ) - farRight - farUp;

   Vec3 origin( 0.0f, 0.0f, 0.0f );

   m_Planes[ Near ].Init( m_NearPlaneVerts[ 0 ], m_NearPlaneVerts[ 1 ], m_NearPlaneVerts[ 2 ] );
   m_Planes[ Far ].Init( m_FarPlaneVerts[ 2 ], m_FarPlaneVerts[ 1 ], m_FarPlaneVerts[ 0 ] );
   m_Planes[ Right ].Init( origin, m_FarPlaneVerts[ 1 ], m_FarPlaneVerts[ 2 ] );
   m_Planes[ Top ].Init( origin, m_FarPlaneVerts[ 0 ], m_FarPlaneVerts[ 1 ] );
   m_Planes[ Left ].Init( origin, m_FarPlaneVerts[ 3 ], m_FarPlaneVerts[ 0 ] );
   m_Planes[ Bottom ].Init( origin, m_FarPlaneVerts[ 2 ], m_FarPlaneVerts[ 3 ] );
   }

OrthogonalFrustum::OrthogonalFrustum( void )
   {
   m_Left = 100.f;
   m_Right = -100.f;
   m_Up = 100.f;
   m_Bottom = -100.f;
   m_FarDis = 200.f;
   m_NearDis = 0.f;
   }

void OrthogonalFrustum::Init( const float left, const float right, const float up, const float bottom, const float farDis, const float nearDis )
   {
   m_Left = left;
   m_Right = right;
   m_Up = up;
   m_Bottom = bottom;
   m_FarDis = farDis;
   m_NearDis = nearDis;

   // left up
   m_NearPlaneVerts[ 0 ] = Vec3( m_Left, m_Up, m_NearDis );
   // right up
   m_NearPlaneVerts[ 1 ] = Vec3( m_Right, m_Up, m_NearDis );
   // right down
   m_NearPlaneVerts[ 2 ] = Vec3( m_Right, m_Bottom, m_NearDis );
   // left down
   m_NearPlaneVerts[ 3 ] = Vec3( m_Left, m_Bottom, m_NearDis );

   // left up
   m_FarPlaneVerts[ 0 ] = Vec3( m_Left, m_Up, m_FarDis );
   // right up
   m_FarPlaneVerts[ 1 ] = Vec3( m_Right, m_Up, m_FarDis );
   // right down
   m_FarPlaneVerts[ 2 ] = Vec3( m_Right, m_Bottom, m_FarDis );
   // left down
   m_FarPlaneVerts[ 3 ] = Vec3( m_Left, m_Bottom, m_FarDis );

   Vec3 origin( 0.0f, 0.0f, 0.0f );

   m_Planes[ Near ].Init( m_NearPlaneVerts[ 0 ], m_NearPlaneVerts[ 1 ], m_NearPlaneVerts[ 2 ] );
   m_Planes[ Far ].Init( m_FarPlaneVerts[ 2 ], m_FarPlaneVerts[ 1 ], m_FarPlaneVerts[ 0 ] );
   m_Planes[ Right ].Init( origin, m_FarPlaneVerts[ 1 ], m_FarPlaneVerts[ 2 ] );
   m_Planes[ Top ].Init( origin, m_FarPlaneVerts[ 0 ], m_FarPlaneVerts[ 1 ] );
   m_Planes[ Left ].Init( origin, m_FarPlaneVerts[ 3 ], m_FarPlaneVerts[ 0 ] );
   m_Planes[ Bottom ].Init( origin, m_FarPlaneVerts[ 2 ], m_FarPlaneVerts[ 3 ] );
   }