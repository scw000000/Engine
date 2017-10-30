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
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Red, pv * m );
   }

Vec3 SphereCollider::SupportMapping( const Vec3& direction ) const
   {
   // Assume that the direction is normalized
   return direction * m_Radius;
   }

void BoxCollider::VRenderShape( const Mat4x4& m, const Mat4x4& pv ) const
   {
   SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Box, g_Green, pv * m );
   }

Vec3 BoxCollider::SupportMapping( const Vec3& direction ) const
   {
   /*float sX = direction.x > 0 ? 0.5f : direction.x < 0 ? -0.5f : 0.f;
   float sY = direction.y > 0 ? 0.5f : direction.y < 0 ? -0.5f : 0.f;
   float sZ = direction.z > 0 ? 0.5f : direction.z < 0 ? -0.5f : 0.f;*/
   float sX = direction.x > 0 ? m_HalfSize.x : -m_HalfSize.x;
   float sY = direction.y > 0 ? m_HalfSize.y : -m_HalfSize.y;
   float sZ = direction.z > 0 ? m_HalfSize.z : -m_HalfSize.z;
   return Vec3( sX, sY, sZ );
   }