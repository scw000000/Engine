/*!
 * \file AnimationNode.cpp
 * \date 2016/07/30 21:42
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
#include "AnimationNode.h"
#include "BoneTransform.h"

AnimationClipNode::AnimationClipNode( shared_ptr< MeshResourceExtraData > pMeshExtra, const std::string& clipName ) : m_pMeshExtraData( pMeshExtra )
   {
   ENG_ASSERT( pMeshExtra );
   m_pAnimation = m_pMeshExtraData->FindAnimation( clipName );
   m_AiTicksPerMs = ( float ) ( m_pAnimation->mTicksPerSecond != 0 ? m_pAnimation->mTicksPerSecond : 25.f ) / 1000.f;
   ENG_ASSERT( m_pAnimation );
   m_PlaybackRate = 1.f;
   m_TimePosition = 0.f ;
   m_IsRunning = false;
   m_ShouldLoop = true;
   }

void AnimationClipNode::VUpdate( unsigned long elapsedMs ) 
   {
   VAddTimeOffset( elapsedMs );
   // If the time is larger than the animation, mod it
   aiAnimTicks = fmod( aiAnimTicks + aiTimeInTicks, ( float ) pAnimation->mDuration );

   UpdateAnimationBones( pMeshExtra, aiAnimTicks, pAnimation, pAiScene->mRootNode, pMeshExtra->m_GlobalInverseTransform );
      
   }

void AnimationClipNode::VSetTimePosition( float timePos ) 
   { 
   m_TimePosition = std::max( 0.f, std::min( 1.0f, timePos ) );
   }

float AnimationClipNode::VGetTimePosition( void ) const
   {
   return m_TimePosition;
   }

void AnimationClipNode::VAddTimeOffset( float offset )
   {
   if()
      {
      }
   else
      {
    
      }
   }

void AnimationClipNode::VSetPlayBackRate( float newRate )
   {
   m_PlaybackRate = newRate;
   }

float AnimationClipNode::VGetPlayBackRate( void ) const 
   {
   return m_PlaybackRate;
   }

void AnimationClipNode::VSetIsRunning( bool isRunning ) 
   {
   m_IsRunning = isRunning;
   }

bool AnimationClipNode::VGetIsRunning( void ) const 
   {
   return m_IsRunning;
   }

void AnimationClipNode::VSetShouldLoop( bool shouldLoop ) 
   {
   m_ShouldLoop = shouldLoop;
   }

bool AnimationClipNode::VGetShouldLoop( void ) const 
   {
   return m_ShouldLoop;
   }

BoneTransform AnimationClipNode::VGetLocalBoneTransform( const std::string& boneName ) const
   {
   return BoneTransform();
   }
