/*!
 * \file AnimationState.cpp
 * \date 2016/07/30 21:54
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
#include "AnimationState.h"
#include "AnimationNode.h"
#include "BoneTransform.h"

AnimationState::AnimationState( shared_ptr< ResHandle > pMeshRes ) : m_pMeshResource( pMeshRes )
   {
   ENG_ASSERT( pMeshRes );
   m_pMeshExtraData = static_pointer_cast< MeshResourceExtraData >( m_pMeshResource->GetExtraData() );
   m_GlobalBoneTransform.resize( m_pMeshExtraData->m_NumBones );

   }

float AnimationState::GetTimePosition( void ) const
   {
   return m_pRootAnimNode->VGetTimePosition();
   }

void AnimationState::SetTimePosition( float newTimePos )
   {
   m_pRootAnimNode->VSetTimePosition( newTimePos );
   }

void AnimationState::AddTimeOffset( float timeOffset )
   {
   m_pRootAnimNode->VAddTimeOffset( timeOffset );
   }

void AnimationState::Update( unsigned long elapsedMs )
   {
   m_pRootAnimNode->VUpdate( elapsedMs );
   UpdatetGlobalBoneTransform( m_pMeshExtraData->m_pScene->mRootNode, m_pMeshExtraData->m_GlobalInverseTransform );
   }

void AnimationState::UpdatetGlobalBoneTransform( aiNode* pAiNode, const aiMatrix4x4& parentTransfrom )
   {
   std::string nodeName( pAiNode->mName.C_Str() );

   BoneTransform localBoneTransform;
   auto boneMappingDataIt = m_pMeshExtraData->m_BoneMappingData.find( nodeName );
   BoneMappingData& boneMappingData = m_pMeshExtraData->m_BoneMappingData;
   ENG_ASSERT( boneMappingDataIt != m_pMeshExtraData->m_BoneMappingData.end() );
   BoneData& boneData = boneMappingDataIt->second;
   
   aiMatrix4x4 localBonePoseTransform;
   if( m_pRootAnimNode->VGetLocalBoneTransform( localBoneTransform, boneData.m_BoneId ) )
      {
      localBonePoseTransform = aiMatrix4x4( localBoneTransform.m_Scale, localBoneTransform.m_Rotation, localBoneTransform.m_Translation );
      }
   else
      {
      localBonePoseTransform = pAiNode->mTransformation;
      }

   aiMatrix4x4 globalBonePoseTransform = parentTransfrom * localBonePoseTransform;
   m_GlobalBoneTransform[ boneData.m_BoneId ] = globalBonePoseTransform * boneData.m_BoneOffset;

   for( unsigned int i = 0; i < pAiNode->mNumChildren; ++i )
      {
      UpdatetGlobalBoneTransform( pAiNode->mChildren[ i ], globalBonePoseTransform );
      }
   }

void AnimationState::SetIsRunning( bool isRunning )
   {
   m_pRootAnimNode->VSetIsRunning( isRunning );
   }

bool AnimationState::GetIsRunning( void ) const
   {
   return m_pRootAnimNode->VGetIsRunning();
   }

void AnimationState::SetShouldLoop( bool shouldLoop )
   {
   m_pRootAnimNode->VSetShouldLoop( shouldLoop );
   }

bool AnimationState::GetShouldLoop( void ) const
   {
   return m_pRootAnimNode->VGetShouldLoop();
   }