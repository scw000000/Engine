#pragma once
/*!
 * \file VolumeRenderComponent.h
 * \date 2016/10/12 22:28
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

#include "RenderComponent.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

class VolumeRenderComponent : public BaseRenderComponent< VolumeRenderComponent >
   {
   public:
      VolumeRenderComponent( void );

   protected:
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override;
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual void VDelegatePostInit( void ) override;
      virtual void VDelegateGenerateXML( TiXmlElement* pBaseElement ) override;

   protected:
      /*MaterialPtr m_pMaterial;*/
      shared_ptr<Resource> m_pVolumeTextureResource;
      shared_ptr<Resource> m_pTransferFuncionResource;
      Vec3 m_CuboidDimension;
      Vec3 m_TextureDimension;
   };

#pragma warning( pop )