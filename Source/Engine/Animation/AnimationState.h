#pragma once
/*!
 * \file AnimationState.h
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
#include <assimp/scene.h>
#include "..\LuaScripting\ScriptClass.h"

class ResHandle;
class MeshResourceExtraData;
class IAnimationNode;

#define INVALID_ANIMSTATE_ID 0

class AnimationState : public BaseScriptClass< AnimationState >
   {
   friend class SkeletalMeshSceneNode;
   friend class AnimationManager;
   public:
      AnimationState( void ) {}
      AnimationState( shared_ptr< ResHandle > pMeshRes, shared_ptr< IAnimationNode > pAnimRootNode );
      
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) override;
      bool Init( void );
      float GetTimePosition( void ) const;
      void SetTimePosition( float newTimePos ); 
      void AddTimeOffset( float timeOffset );
      void Update( unsigned long elapsedMs );
      void UpdatetGlobalBoneTransform( aiNode* pAiNode, const aiMatrix4x4& parentTransfrom );
      void SetIsRunning( bool isRunning );
      bool GetIsRunning( void ) const;
      void SetShouldLoop( bool shouldLoop );
      bool GetShouldLoop( void ) const;
      const std::vector< aiMatrix4x4 >& GetGlobalBoneTransform( void ) const { return m_GlobalBoneTransform; }
      void SetOwner( shared_ptr< Actor > pOwner );

   private:
      void SetMeshResourcePtr( shared_ptr< ResHandle > pMeshRes );

   private:
      shared_ptr< ResHandle > m_pMeshResource;
      shared_ptr< MeshResourceExtraData > m_pMeshExtraData;
      shared_ptr< IAnimationNode > m_pRootAnimNode;
      shared_ptr< Actor > m_pOwner;
      std::vector< aiMatrix4x4 > m_GlobalBoneTransform;
   };