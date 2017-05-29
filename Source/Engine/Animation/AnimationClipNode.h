#pragma once
/*!
 * \file AnimationClipNode.h
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
#include "AnimationNode.h"
#include "..\LuaScripting\ScriptClass.h"

struct aiNodeAnim;

class AnimationClipNode : public BaseAnimationNode< AnimationClipNode >
   {
   public:
      AnimationClipNode( void );
      virtual bool VDelegateBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) override;
      //AnimationClipNode( shared_ptr< MeshResourceExtraData > pMeshExtra, const std::string& clipName );
      virtual bool VDelegateVInit( void ) override;
      virtual void VDelegateUpdate( float deltaSeconds ) override;
      virtual bool VGetLocalBoneTransform( BoneTransform& boneTransform, BoneId boneId ) const override;
      virtual void VSetTimePosition( float timePos ) override;

   private:
      aiNodeAnim* FindNodeAnim( BoneId boneId ) const;
      unsigned int FindTranslation( const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindRotation( const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindScale( const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedTranslation( const aiNodeAnim* pNodeAnim ) const;
      aiQuaternion CalcInterpolatedRotation( const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedScale( const aiNodeAnim* pNodeAnim ) const;

   private:
      shared_ptr< Resource > m_pAnimRes;
      shared_ptr< ResHandle > m_pAnimResHandle;
      aiAnimation* m_pAnimation;
      float m_AiTicksPerMs;
      float m_DurationInMs;
      float m_CurrentAiTicks;
      //std::string m_ClipName;
      std::unordered_map< BoneId, aiNodeAnim* > m_BoneIdToNodeAnimMapping;
   };