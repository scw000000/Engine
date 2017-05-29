#pragma once
/*!
 * \file AnimationLerpNode.h
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
#include "AnimationNode.h"
#include "..\LuaScripting\ScriptClass.h"

class AnimationLerpNode : public BaseAnimationNode< AnimationLerpNode >
   {
   public:
      AnimationLerpNode( void );
      virtual bool VDelegateBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) override;
      virtual void VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) override;
      virtual bool VDelegateVInit( void ) override;
      virtual void VDelegateUpdate( float deltaSeconds ) override;
      virtual bool VGetLocalBoneTransform( BoneTransform& boneTransform, BoneId boneId ) const override;
      void SetLerpFactor( float factor );

   private:

   private:
      float m_LerpFactor;
      shared_ptr< IAnimationNode > m_pLeftChild;
      shared_ptr< IAnimationNode > m_pRightChild;
   };