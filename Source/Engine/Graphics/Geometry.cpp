////////////////////////////////////////////////////////////////////////////////
// Filename: Geometry.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Geometry.h"

const Mat4x4 Mat4x4::g_Identity( glm::mat4( 1.0f ) );
const Quaternion Quaternion::g_Identity( glm::fquat( 0, 0, 0, 1 ) );

bool Plane::Inside( Vec3 p )
   {
   return ( p.Dot( n ) + d >= 0.0f );
   }

bool Plane::Inside( Vec3 p, const float radius )
   {
   float distance = p.Dot( n ) + d;
   // we need to consider if the point is outside the plane, but its radius may compensate for it
   // -> we should test distance + radius >= 0
   return ( distance >= -radius );
   }

Frustum::Frustum( void )
   {
   m_Fov = ENG_QUARTERPI;
   m_Aspect = 4.0f / 3.0f;
   m_NearDis = 1.0f;
   m_FarDis = 1000.0f;
   }

bool Frustum::Inside( const Vec3 &point )
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Plane[i].Inside( point ) )
         {
         return false;
         }
      }
   return true;
   }

bool Frustum::Inside( const Vec3 &point, float radius )
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Plane[i].Inside( point, radius ) )
         {
         return false;
         }
      }
   return true;
   }

void Frustum::Init( const float fov, const float aspect, const float nearClipDis, const float farClipDis )
   {
   m_Fov = fov;
   m_Aspect = aspect;
   m_NearDis = nearClipDis;
   m_FarDis = farClipDis;

   double tanFovOver2 = tan( fov / 2.0f );
   Vec3 nearRight = static_cast<float>( m_NearDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 farRight = static_cast<float>( m_FarDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 nearUp = static_cast<float>( m_NearDis * tanFovOver2 ) * g_Up;
   Vec3 farUp = static_cast<float>( m_FarDis * tanFovOver2 ) * g_Up;

   // These vertives start in upper right and go around clockwise
   m_NearPlaneVerts[0] = ( m_NearDis * g_Forward ) - nearRight + nearUp;
   m_NearPlaneVerts[1] = ( m_NearDis * g_Forward ) + nearRight + nearUp;
   m_NearPlaneVerts[2] = ( m_NearDis * g_Forward ) + nearRight - nearUp;
   m_NearPlaneVerts[3] = ( m_NearDis * g_Forward ) - nearRight - nearUp;

   m_FarPlaneVerts[0] = ( m_FarDis * g_Forward ) - farRight + farUp;
   m_FarPlaneVerts[1] = ( m_FarDis * g_Forward ) + farRight + farUp;
   m_FarPlaneVerts[2] = ( m_FarDis * g_Forward ) + farRight - farUp;
   m_FarPlaneVerts[3] = ( m_FarDis * g_Forward ) - farRight - farUp;

   Vec3 origin(0.0f, 0.0f, 0.0f);
   // todo: implement plane Init
	m_Planes[Near].Init(m_NearPlaneVerts[2], m_NearPlaneVerts[1], m_NearPlaneVerts[0]);
	m_Planes[Far].Init(m_FarPlaneVerts[0], m_FarPlaneVerts[1], m_FarPlaneVerts[2]);
	m_Planes[Right].Init(m_FarPlaneVerts[2], m_FarPlaneVerts[1], origin);
	m_Planes[Top].Init(m_FarPlaneVerts[1], m_FarPlaneVerts[0], origin);
	m_Planes[Left].Init(m_FarPlaneVerts[0], m_FarPlaneVerts[3], origin);
	m_Planes[Bottom].Init(m_FarPlaneVerts[3], m_FarPlaneVerts[2], origin);
   }