#pragma once
/*!
 * \file PhysicsComponent.h
 * \date 2016/07/08 9:51
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
#include "..\Graphics\SceneNode\SceneNodes.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

class PhysicsComponent : public BaseRenderComponent<PhysicsComponent>
   {
   public:
      PhysicsComponent( void );

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

   };

#pragma warning( pop )