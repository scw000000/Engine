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

class AnimationClipNode : public BaseAnimationNode< AnimationClipNode >, public BaseScriptClass< AnimationClipNode >
   {
   public:
      AnimationClipNode( void );
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) override;
      //AnimationClipNode( shared_ptr< MeshResourceExtraData > pMeshExtra, const std::string& clipName );
      virtual void VUpdate( unsigned long elapsedMs ) override;
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
      aiAnimation* m_pAnimation;
      float m_AiTicksPerMs;
      float m_CurrentAiTicks;
      shared_ptr< MeshResourceExtraData > m_pMeshExtraData;
      std::string m_ClipName;
      std::unordered_map< BoneId, aiNodeAnim* > m_BoneIdToNodeAnimMapping;
   };