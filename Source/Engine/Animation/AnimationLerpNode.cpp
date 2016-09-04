/*!
 * \file AnimationLerpNode.cpp
 * \date 2016/09/04 9:32
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
#include "AnimationLerpNode.h"
#include "BoneTransform.h"

AnimationLerpNode::AnimationLerpNode( void )
   {
   m_LerpFactor = 0.f;
   }

bool AnimationLerpNode::VDelegateBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData )
   {
   if( m_ChildAnimNodes.size() != 2 )
      {
      return false;
      }
   m_pLeftChild = m_ChildAnimNodes.front();
   
   m_pRightChild = m_ChildAnimNodes.back();

   return true;
   }

void AnimationLerpNode::VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) 
   {
   metaTableObj.RegisterObjectDirect( "SetLerpFactor", this, &AnimationLerpNode::SetLerpFactor );
   }

bool AnimationLerpNode::VDelegateVInit( void ) 
   {
   return true;
   }

void AnimationLerpNode::VDelegateUpdate( unsigned long elapsedMs ) 
   {
   
   }

bool AnimationLerpNode::VGetLocalBoneTransform( BoneTransform& boneTransform, BoneId boneId ) const
   {
   BoneTransform leftChildTransform;
   if( !m_pLeftChild->VGetLocalBoneTransform( leftChildTransform, boneId ) )
      {
      return false;
      }
   BoneTransform rightChildTransform;
   if( !m_pRightChild->VGetLocalBoneTransform( rightChildTransform, boneId ) )
      {
      return false;
      }
   boneTransform = leftChildTransform;
   boneTransform.InterpolateWith( rightChildTransform, m_LerpFactor );
   return true;
   }

void AnimationLerpNode::SetLerpFactor( float factor )
   {
   m_LerpFactor = factor;
   }