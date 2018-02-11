/*!
 * \file Colliders.cpp
 * \date 2017/10/29 20:09
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
#include "Colliders.h"
#include "..\Graphics\BasicGeometry.h"

void SphereCollider::VRenderShape( const Mat4x4& m, const Mat4x4& pv ) const
   {
   auto copyM = m;
   copyM.MultScale( Vec3( m_Radius, m_Radius, m_Radius ) );
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Red, pv * copyM );
   }

Vec3 SphereCollider::VSupportMapping( const Vec3& direction )
   {
   // Assume that the direction is normalized
   auto localPoint = m_Transform.GetFromWorld().Xform( direction, 0.f ) * m_Radius;
   
   return m_Transform.GetToWorld().Xform( localPoint, 1.f );
   }

void BoxCollider::VRenderShape( const Mat4x4& m, const Mat4x4& pv ) const
   {
   auto copyM = m;
   copyM.MultScale( m_HalfSize * 2.f );
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Box, g_Green, pv * copyM );
   }

Vec3 BoxCollider::VSupportMapping( const Vec3& direction )
   {
   /*float sX = direction.x > 0 ? 0.5f : direction.x < 0 ? -0.5f : 0.f;
   float sY = direction.y > 0 ? 0.5f : direction.y < 0 ? -0.5f : 0.f;
   float sZ = direction.z > 0 ? 0.5f : direction.z < 0 ? -0.5f : 0.f;*/
   auto localDir = m_Transform.GetFromWorld().Xform( direction, 0.f );

   float localPointX = localDir.x > 0 ? m_HalfSize.x : -m_HalfSize.x;
   float localPointY = localDir.y > 0 ? m_HalfSize.y : -m_HalfSize.y;
   float localPointZ = localDir.z > 0 ? m_HalfSize.z : -m_HalfSize.z;

   return m_Transform.GetToWorld().Xform( Vec4( localPointX, localPointY, localPointZ, 1.f ) );
   }