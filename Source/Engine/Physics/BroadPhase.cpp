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
#include "RigidBody.h"

const Vec3 AABBNode::s_Margin = Vec3( 1.f, 1.f, 1.f );

AABB::AABB( void ) : m_Min( Vec3::g_Zero ), m_Max( Vec3::g_Zero )
   {
   }

AABB::AABB( const Vec3& min, const Vec3& max ) : m_Min( max ), m_Max( min )
   {
   }

AABB::AABB( shared_ptr<RigidBody> pRigidBody ) : m_Min( Vec3::g_Zero ), m_Max( Vec3::g_Zero )
   {
   ENG_ASSERT( pRigidBody->m_Colliders.size() );

   const float minFloat = std::numeric_limits<float>::min();
   const float maxFloat = std::numeric_limits<float>::max();
   m_Min = Vec3( maxFloat, maxFloat, maxFloat );
   m_Max = Vec3( minFloat, minFloat, minFloat );
   // calculate aabb from the rigid body
   for( int i = 0; i < pRigidBody->m_Colliders.size(); ++i )
      {
      auto& collider = pRigidBody->m_Colliders[ i ];
      float minX = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( -1.f, 0.f, 0.f ), false ) ).x;
      float minY = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( 0.f, -1.f, 0.f ), false ) ).y;
      float minZ = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( 0.f, 0.f, -1.f ), false ) ).z;
      m_Min.x = std::min( m_Min.x, minX );
      m_Min.y = std::min( m_Min.y, minY );
      m_Min.z = std::min( m_Min.z, minZ );

      float maxX = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( 1.f, 0.f, 0.f ), false ) ).x;
      float maxY = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( 0.f, 1.f, 0.f ), false ) ).y;
      float maxZ = collider->VSupportMapping( pRigidBody->VTransformToLocal( Vec3( 0.f, 0.f, 1.f ), false ) ).z;
      m_Max.x = std::max( m_Max.x, maxX );
      m_Max.y = std::max( m_Max.y, maxY );
      m_Max.z = std::max( m_Max.z, maxZ );
      }
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

float AABB::GetVolume() const
   {
   return ( m_Max.x - m_Min.x ) * ( m_Max.y - m_Min.y ) * ( m_Max.z - m_Min.z );
   }

AABBNode::AABBNode() 
   {
   m_pChildren[ 0 ] = nullptr;
   m_pChildren[ 1 ] = nullptr;
   }

AABBNode::AABBNode( shared_ptr<AABB> pAABB ) : m_pLeafData( pAABB )
   {
   m_pChildren[ 0 ] = nullptr;
   m_pChildren[ 1 ] = nullptr;
   }

void AABBNode::SetChildren( shared_ptr<AABBNode> pLeft, shared_ptr<AABBNode> pRight )
   {
   m_pChildren[ 0 ] = pLeft;
   m_pChildren[ 1 ] = pRight;
   
   if( pLeft ) 
      {
      pLeft->m_pParent = shared_from_this();
      }
   if( pRight )
      {
      pRight->m_pParent = shared_from_this();
      }
   }

void AABBNode::SetLeafData( shared_ptr<AABB> pAABB )
   {
   m_pLeafData = pAABB;
   SetChildren(nullptr, nullptr);
   }

shared_ptr<AABBNode> AABBNode::GetSibling( void ) const
   {
   auto pParent = m_pParent.lock();
   if( pParent == nullptr )
      {
      return nullptr;
      }
   if( pParent->m_pChildren[ 0 ] == shared_from_this() )
      {
      return pParent->m_pChildren[ 1 ];
      }
   return pParent->m_pChildren[ 0 ];
   }

void AABBNode::ResetBoundary( void )
   {
   if( IsLeaf() )
      {
      m_Boundary = AABB( m_pLeafData->m_Min - s_Margin, m_pLeafData->m_Max + s_Margin );
      }
   else
      {
      m_Boundary = m_pChildren[ 0 ]->m_Boundary.Union( m_pChildren[ 1 ]->m_Boundary );
      }
   }

void Broadphase::VAddRigidBody( shared_ptr< RigidBody > pRigidbody ) 
   {
   ENG_ASSERT( m_RigidBodyToAABBNode.find( pRigidbody ) == m_RigidBodyToAABBNode.end() );

   shared_ptr<AABB> pAABB( ENG_NEW AABB( pRigidbody ) );
   shared_ptr<AABBNode> pAABBNode( ENG_NEW AABBNode() );
   pAABBNode->SetLeafData( pAABB );
   pAABBNode->ResetBoundary();

   m_RigidBodyToAABBNode[ pRigidbody ] = pAABBNode;
   
   if( m_pAABBRoot )
      {
      // do insert
      AddAABBNode( pAABBNode, m_pAABBRoot );
      }
   else
      {
      m_pAABBRoot = pAABBNode;
      }
   }

void Broadphase::VRemoveRigidBody( shared_ptr< RigidBody > pRigidbody )
   {
   auto findIt = m_RigidBodyToAABBNode.find( pRigidbody );
   // it must exist
   ENG_ASSERT( findIt != m_RigidBodyToAABBNode.end() );
   DeleteAABBSubTree( findIt->second );
   m_RigidBodyToAABBNode.erase( pRigidbody );
   }

void Broadphase::VCalcualteCollisionPairs( void ) 
   {
   m_CollistionPairs.clear();
   auto last = --m_RigidBodyToAABBNode.end();
   for( auto leftIt = m_RigidBodyToAABBNode.begin(); leftIt != last; ++leftIt )
      {
      for( auto rightIt = std::next( m_RigidBodyToAABBNode.begin(), 1 ); rightIt != m_RigidBodyToAABBNode.end(); ++rightIt )
         {
         m_CollistionPairs.push_back( make_pair( leftIt->first, rightIt->first ) );
         }
      }
   }

void Broadphase::AddAABBNode( shared_ptr<AABBNode> pNode, shared_ptr<AABBNode> pParent )
   {

   if(!pParent->IsLeaf())
      {
      // test which volume increases least when pNode is inserted into the child
      float volumeIncreas0 = pParent->m_pChildren[ 0 ]->m_Boundary.Union( pNode->m_Boundary ).GetVolume() -
         pParent->m_pChildren[ 0 ]->m_Boundary.GetVolume();
      float volumeIncreas1 = pParent->m_pChildren[ 1 ]->m_Boundary.Union( pNode->m_Boundary ).GetVolume() -
         pParent->m_pChildren[ 1 ]->m_Boundary.GetVolume();

      if(volumeIncreas0 < volumeIncreas1)
         {
         AddAABBNode( pNode, pParent->m_pChildren[ 0 ] );
         }
      else
         {
         AddAABBNode( pNode, pParent->m_pChildren[ 1 ] );
         }

      }
   else
      {
      // this old parent Val will become the other leaf
      auto oldParentVal = *pParent.get();
      // copy the data to the new leaf
      shared_ptr<AABBNode> pNewLeaf( ENG_NEW AABBNode( oldParentVal ) );
      pParent->SetChildren( pNode, pNewLeaf );
      // since the parent is not leaf anymore, clear its leaf data
      pParent->m_pLeafData.reset();
      
      }

   pParent->ResetBoundary();
   }

void Broadphase::DeleteAABBSubTree( shared_ptr<AABBNode> pSubRootNode )
   {
   
   if( pSubRootNode != m_pAABBRoot )
      {
      auto pParent = pSubRootNode->m_pParent.lock();
      auto pGrandParent = pParent->m_pParent.lock();
      // if it has grand parent
      if( pGrandParent )
         {
         auto pSibling = pSubRootNode->GetSibling();
         pSibling->m_pParent = pGrandParent;
         if( pGrandParent->m_pChildren[ 0 ] == pParent )
            {
            pGrandParent->m_pChildren[ 0 ] = pSibling;
            }
         else
            {
            pGrandParent->m_pChildren[ 1 ] = pSibling;
            }
         }
      else
         {
         m_pAABBRoot = pSubRootNode->GetSibling();
         m_pAABBRoot->m_pParent.reset();
         }
      }
   else
      {
      m_pAABBRoot.reset();
      }
   }