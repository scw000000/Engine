#pragma once
/*!
 * \file BroadPhase.h
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

class AABB
   {
   friend class AABBNode;
   public:
      AABB( void );
      AABB( const Vec3& min, const Vec3& max );
      AABB( shared_ptr<RigidBody> pRigidBody );
      bool IsIntersect( const AABB& other ) const;
      bool IsContain( const AABB& other ) const;
      AABB Union( const AABB& other );
      void Update( shared_ptr<RigidBody> pRigidBody );

      float GetVolume() const;

   private:
      Vec3 m_Min;
      Vec3 m_Max;
   };

class AABBNode : public std::enable_shared_from_this<AABBNode>
   {
   friend class Broadphase;
   public:
      AABBNode();
      AABBNode( shared_ptr<AABB> pAABB );
      bool IsLeaf( void ) const { return m_pChildren[ 0 ] == nullptr && m_pChildren[ 1 ] == nullptr; }
      void SetChildren( shared_ptr<AABBNode> pLeft, shared_ptr<AABBNode> pRight );
      void SetLeafData( shared_ptr<AABB> pAABB );
      shared_ptr<AABBNode> GetSibling( void ) const;
      void ResetBoundary( void );
   private:
      static const  Vec3 s_Margin;
      
      weak_ptr<AABBNode> m_pParent;
      shared_ptr<AABBNode> m_pChildren[ 2 ];

      AABB m_Boundary;
      shared_ptr<AABB> m_pLeafData;
      bool m_IsChildrenCrossed;
   };

class Broadphase : public IBroadphase
   {
   public:
      virtual void VUpdate( const float deltaSeconds ) override;
      virtual void VAddRigidBody( shared_ptr< RigidBody > pRigidbody ) override;
      virtual void VRemoveRigidBody( shared_ptr< RigidBody > pRigidbody ) override;
      virtual void VCalcualteCollisionPairs( void ) override;
      virtual CollisionPairs& VGetCollisionPairs( void ) override { return m_CollistionPairs; };

   private:
      void AddAABBNode( shared_ptr<AABBNode> pNode, shared_ptr<AABBNode> pParent );
      void DeleteAABBSubTree( shared_ptr<AABBNode> pSubRootNode );
      void ClearCrossFlag(shared_ptr<AABBNode> pNode);
      void TestPairCross( shared_ptr<AABBNode> pNode0, shared_ptr<AABBNode> pNode1 );
      void TestChildrenCross( shared_ptr<AABBNode> pNode );

   private:
   std::unordered_map< shared_ptr< RigidBody >, shared_ptr< AABBNode > > m_RigidBodyToAABBNode;
   std::unordered_map< shared_ptr< AABBNode >, shared_ptr< RigidBody > > m_AABBNodeToRigidBody;
   // std::vector< shared_ptr< IRigidBody > > m_RigidBodies;
   CollisionPairs m_CollistionPairs;
   shared_ptr<AABBNode> m_pAABBRoot;
   };

