/*!
 * \file BroadPhase.cpp
 * \date 2018/01/28 11:03
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
#include "BroadPhase.h"

const Vec3 AABBNode::s_Margin = Vec3( 1.f, 1.f, 1.f );

AABB::AABB( void ) : m_Min( Vec3::g_Zero ), m_Max( Vec3::g_Zero )
   {
   }

AABB::AABB( const Vec3& min, const Vec3& max ) : m_Min( max ), m_Max( min )
   {
   }

bool AABB::IsIntersect( const AABB& other ) const
   {
   if( m_Min.x > other.m_Max.x || other.m_Min.x > m_Max.x )
      {
   return false;
      }
   if( m_Min.y > other.m_Max.y || other.m_Min.y > m_Max.y )
      {
      return false;
      }
   if( m_Min.z > other.m_Max.z || other.m_Min.z > m_Max.z )
      {
      return false;
      }

   return true;
   }

AABB AABB::Union( const AABB& other )
   {
   Vec3 unionMax( std::max( m_Max.x, other.m_Max.x )
                  , std::max( m_Max.y, other.m_Max.y ) 
                  , std::max( m_Max.z, other.m_Max.z ) );

   Vec3 unionMin( std::min( m_Min.x, other.m_Min.x )
                  , std::min( m_Min.y, other.m_Min.y )
                  , std::min( m_Min.z, other.m_Min.z ) );

   return AABB( unionMax, unionMin );
   }

AABBNode::AABBNode() : m_Parent( nullptr ), m_pLeafData( nullptr )
   {
   m_Children[ 0 ] = nullptr;
   m_Children[ 1 ] = nullptr;
   }

void AABBNode::SetChildren( AABBNode* pLeft, AABBNode* pRight )
   {
   m_Children[ 0 ] = pLeft;
   m_Children[ 1 ] = pRight;
   
   if( pLeft ) 
      {
      pLeft->m_Parent = this;
      }
   if( pRight )
      {
      pRight->m_Parent = this;
      }
   }

void AABBNode::SetLeafData( AABB* pAABB )
   {
   m_pLeafData = pAABB;
   SetChildren(nullptr, nullptr);
   }

AABBNode* AABBNode::GetSibling( void ) const
   {
   if( m_Parent == nullptr )
      {
      return nullptr;
      }
   if( m_Parent->m_Children[0] == this )
      {
      return m_Parent->m_Children[ 1 ];
      }
   m_Parent->m_Children[ 0 ];
   }

void AABBNode::ResetBoundary( void )
   {
   if( IsLeaf() )
      {
      m_Boundary = AABB( m_pLeafData->m_Min - s_Margin, m_pLeafData->m_Max + s_Margin );
      }
   else
      {
      m_Boundary = m_Children[ 0 ]->m_Boundary.Union( m_Children[ 1 ]->m_Boundary );
      }
   }

void Broadphase::VAddRigidBody( shared_ptr< RigidBody > pRigidbody ) 
   {
   ENG_ASSERT( m_RigidBodyToAABB.find( pRigidbody ) == m_RigidBodyToAABB.end() );
   m_RigidBodyToAABB[ pRigidbody ];
   // m_RigidBodies.push_back( pRigidbody );
   }

void Broadphase::VRemoveRigidBody( shared_ptr< RigidBody > pRigidbody )
   {
   m_RigidBodyToAABB.erase( pRigidbody );
   }

void Broadphase::VCalcualteCollisionPairs( void ) 
   {
   m_CollistionPairs.clear();
   auto last = --m_RigidBodyToAABB.end();
   for( auto leftIt = m_RigidBodyToAABB.begin(); leftIt != last; ++leftIt )
      {
      for( auto rightIt = std::next( m_RigidBodyToAABB.begin(), 1 ); rightIt != m_RigidBodyToAABB.end(); ++rightIt )
         {
         m_CollistionPairs.push_back( make_pair( leftIt->first, rightIt->first ) );
         }
      }
   }