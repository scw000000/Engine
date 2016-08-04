/*!
 * \file AnimationClipNode.cpp
 * \date 2016/08/01 16:49
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */

#include "EngineStd.h"
#include "AnimationClipNode.h"
#include "BoneTransform.h"

const char* BaseAnimationNode< AnimationClipNode >::s_pName = "AnimationClipNode";
const AnimNodeType BaseAnimationNode< AnimationClipNode >::s_AnimNodeType = 0xb75bd91f;

AnimationClipNode::AnimationClipNode(void )
   {
   }

//AnimationClipNode::AnimationClipNode( shared_ptr< MeshResourceExtraData > pMeshExtra, const std::string& clipName )
//   {
//   ENG_ASSERT( pMeshExtra );
//   m_pAnimation = m_pMeshExtraData->FindAnimation( clipName );
//   ENG_ASSERT( m_pAnimation );
//   m_AiTicksPerMs = ( float ) ( m_pAnimation->mTicksPerSecond != 0 ? m_pAnimation->mTicksPerSecond : 25.f ) / 1000.f;
//   m_DurationInMs = ( float ) 1000.0 * m_pAnimation->mDuration / m_pAnimation->mTicksPerSecond;
//
//   for( unsigned int nodeIdx = 0; nodeIdx < m_pAnimation->mNumChannels; ++nodeIdx )
//      {
//      auto BoneDataIt = pMeshExtra->m_BoneMappingData.find( m_pAnimation->mChannels[ nodeIdx ]->mNodeName.C_Str() );
//      if( BoneDataIt == pMeshExtra->m_BoneMappingData.end() )
//         {
//         m_BoneIdToNodeAnimMapping[ BoneDataIt->second.m_BoneId ] = m_pAnimation->mChannels[ nodeIdx ];
//         }
//      }
//   m_CurrentAiTicks = 0.f;
//   }

void AnimationClipNode::VUpdate( unsigned long elapsedMs )
   {
   VAddTimeOffset( m_PlaybackRate * elapsedMs );
   
   }

bool AnimationClipNode::VGetLocalBoneTransform( BoneTransform& boneTransform, BoneId boneId ) const
   {
   auto pNodeAnim = FindNodeAnim( boneId );

   if( !pNodeAnim )
      {
      return false;
      }
   
   boneTransform = BoneTransform( CalcInterpolatedScale( pNodeAnim ), CalcInterpolatedRotation( pNodeAnim ), CalcInterpolatedTranslation( pNodeAnim ) );
   return true;
   }

void AnimationClipNode::VSetTimePosition( float timePos )
   {
   BaseAnimationNode::VSetTimePosition( timePos );
   m_CurrentAiTicks = m_TimePosition * ( float ) m_pAnimation->mDuration;
   }

aiNodeAnim* AnimationClipNode::FindNodeAnim( BoneId boneId ) const
   {
   auto pNodeAnimIt = m_BoneIdToNodeAnimMapping.find( boneId );
   if( pNodeAnimIt != m_BoneIdToNodeAnimMapping.end() )
      {
      return pNodeAnimIt->second;
      }
   return NULL;
   }

unsigned int AnimationClipNode::FindTranslation( const aiNodeAnim* pNodeAnim ) const
   {
   for( unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++ )
      {
      if( m_CurrentAiTicks < ( float ) pNodeAnim->mPositionKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }

unsigned int AnimationClipNode::FindRotation( const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumRotationKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++ )
      {
      if( m_CurrentAiTicks < ( float ) pNodeAnim->mRotationKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }

unsigned int AnimationClipNode::FindScale( const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumScalingKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++ )
      {
      if( m_CurrentAiTicks < ( float ) pNodeAnim->mScalingKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }

aiVector3D AnimationClipNode::CalcInterpolatedTranslation( const aiNodeAnim* pNodeAnim ) const
   {
   if( pNodeAnim->mNumPositionKeys == 1 )
      {
      return pNodeAnim->mPositionKeys[ 0 ].mValue;
      }
   unsigned int positionIdx = FindTranslation( pNodeAnim);
   unsigned int nxtPositionIdx = ( positionIdx + 1 );
   ENG_ASSERT( nxtPositionIdx < pNodeAnim->mNumPositionKeys );
   float deltaTime = ( float ) ( pNodeAnim->mPositionKeys[ nxtPositionIdx ].mTime - pNodeAnim->mPositionKeys[ positionIdx ].mTime );
   float factor = ( m_CurrentAiTicks - ( float ) pNodeAnim->mPositionKeys[ positionIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mPositionKeys[ positionIdx ].mValue;
   const aiVector3D& end = pNodeAnim->mPositionKeys[ nxtPositionIdx ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }

aiQuaternion AnimationClipNode::CalcInterpolatedRotation( const aiNodeAnim* pNodeAnim ) const
   {
   // we need at least two values to interpolate...
   if( pNodeAnim->mNumRotationKeys == 1 )
      {
      return pNodeAnim->mRotationKeys[ 0 ].mValue;
      }
   unsigned int rotationIdx = FindRotation( pNodeAnim);
   unsigned int nxtRotationIdx = ( rotationIdx + 1 );
   ENG_ASSERT( nxtRotationIdx < pNodeAnim->mNumRotationKeys );
   float deltaTime = ( float ) ( pNodeAnim->mRotationKeys[ nxtRotationIdx ].mTime - pNodeAnim->mRotationKeys[ rotationIdx ].mTime );
   float factor = ( m_CurrentAiTicks - ( float ) pNodeAnim->mRotationKeys[ rotationIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[ rotationIdx ].mValue;
   const aiQuaternion& endRotationQ = pNodeAnim->mRotationKeys[ nxtRotationIdx ].mValue;
   aiQuaternion ret;
   aiQuaternion::Interpolate( ret, startRotationQ, endRotationQ, factor );
   ret.Normalize();
   return ret;
   }

aiVector3D AnimationClipNode::CalcInterpolatedScale( const aiNodeAnim* pNodeAnim ) const
   {
   if( pNodeAnim->mNumScalingKeys == 1 )
      {
      return pNodeAnim->mScalingKeys[ 0 ].mValue;
      }
   // find the last key frame that its time is smaller than specified animation time
   unsigned int scaleIdx = FindScale( pNodeAnim);
   unsigned int nxtScaleIdx = ( scaleIdx + 1 );
   ENG_ASSERT( nxtScaleIdx < pNodeAnim->mNumScalingKeys );
   // time between these two key frames
   float deltaTime = ( float ) ( pNodeAnim->mScalingKeys[ nxtScaleIdx ].mTime - pNodeAnim->mScalingKeys[ scaleIdx ].mTime );
   float factor = ( m_CurrentAiTicks - ( float ) pNodeAnim->mScalingKeys[ scaleIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mScalingKeys[ scaleIdx ].mValue;
   const aiVector3D& end = pNodeAnim->mScalingKeys[ nxtScaleIdx ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }
