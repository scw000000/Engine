#pragma once
/*!
 * \file SkyRenderComponent.h
 * \date 2016/05/03 16:54
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

class SkyRenderComponent : public BaseRenderComponent<SkyRenderComponent>
   {
   public:
      SkyRenderComponent( void );
      virtual void Destory( void ) override;

   protected:
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override;  // factory method to create the appropriate scene node

      // editor stuff
      virtual void VDelegateGenerateXML( TiXmlElement* pBaseElement ) override;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) override;

   protected:
      shared_ptr<Resource> m_pTextureResource;
      shared_ptr<Resource> m_pMeshResource;
   };

#pragma warning( pop )