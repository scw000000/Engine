#pragma once
/*!
 * \file LightRenderComponent.h
 * \date 2016/05/03 15:07
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
#include "..\Graphics\Light.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

class LightRenderComponent : public BaseRenderComponent<LightRenderComponent>
   {
   public:
      LightRenderComponent( void );
      virtual void Destory( void ) override;
      const LightProperties& GetLightProperties( void ) const{ return *m_pLightProps; };

   protected:
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

      // editor stuff
      virtual void VCreateInheritedXmlElements( TiXmlElement* pBaseElement ) override;

   protected:
      LightPropertiesPtr m_pLightProps; 

   };

#pragma warning( pop )
