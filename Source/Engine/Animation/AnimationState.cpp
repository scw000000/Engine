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

AnimationState::AnimationState( shared_ptr< ResHandle > pMeshRes, shared_ptr< IAnimationNode > pAnimRootNode ) : m_pMeshResource( pMeshRes )
   {
   ENG_ASSERT( pMeshRes );
   m_pMeshExtraData = static_pointer_cast< MeshResourceExtraData >( m_pMeshResource->GetExtraData() );
   m_GlobalBoneTransform.resize( m_pMeshExtraData->m_NumBones );

   m_pRootAnimNode = pAnimRootNode;
   }

bool AnimationState::Init( void )
   {
   if( !m_pMeshResource || !m_pMeshExtraData )
      {
      return false;
      }
   if( !m_pRootAnimNode )
      {
      return true;
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
   if( !m_pRootAnimNode )
      {
      return 0.f;
      }
   return m_pRootAnimNode->VGetTimePosition();
   }

void AnimationState::SetTimePosition( float newTimePos )
   {
   if( !m_pRootAnimNode )
      {
      return;
      }
   m_pRootAnimNode->VSetTimePosition( newTimePos );
   }

void AnimationState::AddTimeOffset( float timeOffset )
   {
   if( !m_pRootAnimNode )
      {
      return;
      }
   m_pRootAnimNode->VAddTimeOffset( timeOffset );
   }

void AnimationState::Update( float deltaSeconds )
   {
   if( !m_pRootAnimNode )
      {
      return;
      }
   m_pRootAnimNode->VUpdate( deltaSeconds );
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
      if( m_pRootAnimNode && m_pRootAnimNode->VGetLocalBoneTransform( localBoneTransform, boneData.m_BoneId ) )
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
   if( !m_pRootAnimNode )
      {
      return;
      }
   m_pRootAnimNode->VSetIsRunning( isRunning ); 
   }

bool AnimationState::GetIsRunning( void ) const
   {
   if( !m_pRootAnimNode )
      {
      return false;
      }
   return m_pRootAnimNode->VGetIsRunning();
   }

void AnimationState::SetShouldLoop( bool shouldLoop )
   {
   if( !m_pRootAnimNode )
      {
      return;
      }
   
   m_pRootAnimNode->VSetShouldLoop( shouldLoop );
   }

bool AnimationState::GetShouldLoop( void ) const
   {
   if( m_pRootAnimNode )
      {
      return m_pRootAnimNode->VGetShouldLoop();
      }
   return false;
   }

void AnimationState::SetOwner( shared_ptr< Actor > pOwner )
   {
   ENG_ASSERT( pOwner );
   m_pOwner = pOwner;
   }

void AnimationState::SetMeshResourcePtr( shared_ptr< ResHandle > pMeshRes )
   {
   ENG_ASSERT( pMeshRes );
   m_pMeshResource = pMeshRes;
   m_pMeshExtraData = static_pointer_cast< MeshResourceExtraData >( m_pMeshResource->GetExtraData() );
   m_GlobalBoneTransform.resize( m_pMeshExtraData->m_NumBones );
   if( !m_pRootAnimNode )
      {
      return;
      }
   m_pRootAnimNode->VSetMeshExtraDataPtr( m_pMeshExtraData );
   }