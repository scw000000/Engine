#pragma once
/*!
 * \file MeshRenderComponent.h
 * \date 2016/07/24 14:03
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


//---------------------------------------------------------------------------------------------------------------------
// This class represents a render component built from a Mesh.  In a real game, this is the one you'll use 99% of the 
// time towards the end of the project.  The other classes are important for testing since programming tends to move 
// a lot faster than art in the early stages of development.
//---------------------------------------------------------------------------------------------------------------------
class MeshRenderComponent : public BaseRenderComponent<MeshRenderComponent>
   {
      public:
      MeshRenderComponent( void );
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
      shared_ptr<Resource> m_pMeshResource;
   };

#pragma warning( pop )