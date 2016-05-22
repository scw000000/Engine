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


class LightRenderComponent : public BaseRenderComponent
   {
	
   public:
	   static const char *g_Name;
	   virtual const char *VGetName() const { return g_Name; }
      LightRenderComponent( void );
      virtual void Destory( void ) override;
      const LightProperties& GetLightProperties( void ) const{ return *m_pLightProps; };

   protected:
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

      // editor stuff
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      LightPropertiesPtr m_pLightProps; 

   };
