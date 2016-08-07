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
#include "AnimationClipNode.h"

AnimationState::AnimationState( AnimationStateId id, shared_ptr< ResHandle > pMeshRes, shared_ptr< IAnimationNode > pAnimRootNode ) : m_pMeshResource( pMeshRes )
   {
   ENG_ASSERT( pMeshRes );
   m_pMeshExtraData = static_pointer_cast< MeshResourceExtraData >( m_pMeshResource->GetExtraData() );
   m_GlobalBoneTransform.resize( m_pMeshExtraData->m_NumBones );

   m_pRootAnimNode = pAnimRootNode;
   m_Id = id;
   }

bool AnimationState::Init( void )
   {
   if( !m_pMeshResource || !m_pMeshExtraData || !m_pRootAnimNode )
      {
      return false;
      }
   return m_pRootAnimNode->VInit();
   }

bool AnimationState::VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData )
   {
   auto rootAnimNode = constructionData.Lookup( "RootAnimNode" );
   if( !rootAnimNode.IsTable() || !IsBaseClassOf< IAnimationNode >( rootAnimNode ) )
      {
      return false;
      }
   m_pRootAnimNode.reset( GetObjUserDataPtr< IAnimationNode >( rootAnimNode ) );
   return true;
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

   aiMatrix4x4 globalBonePoseTransform;
   BoneMappingData& boneMappingData = m_pMeshExtraData->m_BoneMappingData;
   auto boneMappingDataIt = boneMappingData.find( nodeName );
   
   if( boneMappingDataIt != boneMappingData.end() )
      {
      BoneData& boneData = boneMappingData[ nodeName ];
      BoneTransform localBoneTransform;
      if( m_pRootAnimNode->VGetLocalBoneTransform( localBoneTransform, boneData.m_BoneId ) )
         {
         globalBonePoseTransform = parentTransfrom * aiMatrix4x4( localBoneTransform.m_Scale, localBoneTransform.m_Rotation, localBoneTransform.m_Translation );
         }
      else
         {
         globalBonePoseTransform = parentTransfrom * pAiNode->mTransformation;
         }

      m_GlobalBoneTransform[ boneData.m_BoneId ] = globalBonePoseTransform * boneData.m_BoneOffset;
      }
   else
      {
      globalBonePoseTransform = parentTransfrom * pAiNode->mTransformation;
      }
   
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

void AnimationState::SetMeshResourcePtr( shared_ptr< ResHandle > pMeshRes )
   {
   ENG_ASSERT( pMeshRes );
   m_pMeshResource = pMeshRes;
   m_pMeshExtraData = static_pointer_cast< MeshResourceExtraData >( m_pMeshResource->GetExtraData() );
   m_GlobalBoneTransform.resize( m_pMeshExtraData->m_NumBones );
   ENG_ASSERT( m_pRootAnimNode );
   m_pRootAnimNode->VSetMeshExtraDataPtr( m_pMeshExtraData );
   }