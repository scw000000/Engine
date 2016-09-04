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

bool AnimationLerpNode::VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) 
   {
   auto childNodeObj = constructionData.Lookup( "LeftChild" );

   if( !IsBaseClassOf< IAnimationNode >( childNodeObj ) )
      {
      return false;
      }
   m_pLeftChild.reset( GetObjUserDataPtr< IAnimationNode >( childNodeObj ) );
   
   childNodeObj = constructionData.Lookup( "RightChild" );

   if( !IsBaseClassOf< IAnimationNode >( childNodeObj ) )
      {
      return false;
      }
   m_pRightChild.reset( GetObjUserDataPtr< IAnimationNode >( childNodeObj ) );


   /*auto childAnimNodesObj = constructionData.Lookup( "ChildAnimNodes" );
   ENG_ASSERT( childAnimNodesObj.IsTable() );
   for( LuaPlus::LuaTableIterator childNodesIt( childAnimNodesObj ); childNodesIt; childNodesIt.Next() )
      {

      }*/

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