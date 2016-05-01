#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: LightRenderComponent.h
////////////////////////////////////////////////////////////////////////////////

#include "RenderComponent.h"
#include "..\Graphics\Light.h"

//---------------------------------------------------------------------------------------------------------------------
// Lights
//---------------------------------------------------------------------------------------------------------------------
class LightRenderComponent : public BaseRenderComponent
   {
	
   public:
	   static const char *g_Name;
	   virtual const char *VGetName() const { return g_Name; }
      LightRenderComponent( void );
      const LightProperties& GetLightProperties( void ) const{ return m_Props; };

   protected:
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

      // editor stuff
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      LightProperties m_Props; 

   };
