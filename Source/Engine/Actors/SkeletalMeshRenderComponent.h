#pragma once
/*!
 * \file SkeletalMeshRenderComponent.h
 * \date 2016/07/24 14:32
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
#include "RenderComponent.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

class SkeletalMeshRenderComponent : public BaseRenderComponent< SkeletalMeshRenderComponent >
   {
   public:
      SkeletalMeshRenderComponent( void );
      virtual void Destory( void ) override;

   protected:
      /**
      * @brief factory method to create the appropriate scene node
      *
      * @param   void
      * @return shared_ptr<SceneNode>
      */
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override;
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual void VDelegatePostInit( void ) override;
      virtual void VDelegateGenerateXML( TiXmlElement* pBaseElement ) override;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) override;

   protected:
      MaterialPtr m_pMaterial;
      shared_ptr< Resource > m_pSkeletalMeshResource;
      shared_ptr< Resource > m_pAnimScriptResource;
   };

#pragma warning( pop )