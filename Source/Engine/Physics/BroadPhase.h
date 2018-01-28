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
      bool IsIntersect( const AABB& other ) const;
      AABB Union( const AABB& other );
   private:
      Vec3 m_Min;
      Vec3 m_Max;
   };

class AABBNode
   {
   friend class Broadphase;
   public:
      AABBNode();
      bool IsLeaf( void ) const { return m_Children[ 0 ] == nullptr && m_Children[ 1 ] == nullptr; }
      void SetChildren( AABBNode* pLeft, AABBNode* pRight );
      void SetLeafData( AABB* pAABB );
      AABBNode* GetSibling( void ) const;
      void ResetBoundary( void );
   private:
      static const  Vec3 s_Margin;
      
      AABBNode* m_Parent;
      AABBNode* m_Children[ 2 ];

      AABB m_Boundary;
      AABB* m_pLeafData;
   };

class Broadphase : public IBroadphase
   {
   public:
      virtual void VAddRigidBody( shared_ptr< RigidBody > pRigidbody ) override;
      virtual void VRemoveRigidBody( shared_ptr< RigidBody > pRigidbody ) override;
      virtual void VCalcualteCollisionPairs( void ) override;
      virtual CollisionPairs& VGetCollisionPairs( void ) override { return m_CollistionPairs; };

   private:
   std::unordered_map< shared_ptr< RigidBody >, AABB > m_RigidBodyToAABB;
   // std::vector< shared_ptr< IRigidBody > > m_RigidBodies;
   CollisionPairs m_CollistionPairs;
   AABB* pAABBRoot;
   };

