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

AABB SphereCollider::VGetAABB( void ) const
   {
   auto translation = m_Transform.GetToWorldPosition();
   return AABB( Vec3( -m_Radius, -m_Radius, -m_Radius ) + translation, Vec3( m_Radius, m_Radius, m_Radius ) + translation );
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

   float localPointX = localDir.x > 0.f ? m_HalfSize.x : ( localDir.x < 0.f ? -m_HalfSize.x : 0.f );
   float localPointY = localDir.y > 0.f ? m_HalfSize.y : ( localDir.y < 0.f ? -m_HalfSize.y : 0.f );
   float localPointZ = localDir.z > 0.f ? m_HalfSize.z : ( localDir.z < 0.f ? -m_HalfSize.z : 0.f );

   return m_Transform.GetToWorld().Xform( Vec4( localPointX, localPointY, localPointZ, 1.f ) );
   }

AABB BoxCollider::VGetAABB( void ) const
   {
   auto toWorld = m_Transform.GetToWorld();
   Vec3 minPos = toWorld.GetToWorldPosition();
   Vec3 maxPos(minPos);
   
   for( int i = 0; i < 3; ++i )
      {
      for( int j = 0; j < 3; ++j )
         {
         float e = toWorld[ i ][ j ] * -m_HalfSize[ j ];
         float f = toWorld[ i ][ j ] * m_HalfSize[ j ];
         if(e < f)
            {
            minPos[ j ] += e;
            maxPos[ j ] += f;
            }
         else
            {
            minPos[ j ] += f;
            maxPos[ j ] += e;
            }
         }
      }
   return AABB(minPos, maxPos);
   }