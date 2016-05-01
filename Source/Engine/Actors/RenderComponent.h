#pragma once
/*!
 * \file RenderComponent.h
 * \date 2016/04/30 21:47
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: refactor to support component with  multi sceneNode
 * LATER: add functions for editor 
 * \note
*/
#include "ActorComponent.h"

class BaseRenderComponent : public ActorComponent
   {
   public:
      /**
       * @brief set member variable from xml elements
       *
       * This function should not override by derived Rendercomponent
       * you should override VDelegateInit instead
       * @param  pData TiXmlElement * pData
       * @return bool
       */
      virtual bool VInit( TiXmlElement* pData ) override;
      virtual void VPostInit( void ) override;
	   //virtual void VOnChanged( void ) override;
      //virtual TiXmlElement* VGenerateXml(void) override;

   protected:
      /**
       * @brief loads the SceneNode specific data ( represented in the <SceneNode> tag )
       *
       * @param  pData TiXmlElement * pData
       * @return bool
       */
      virtual bool VDelegateInit( TiXmlElement* pData ) { return true; }
      // factory method to create the appropriate scene node
      // This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) = 0;  
      static Color LoadColor( TiXmlElement* pData );

   protected:
      shared_ptr<SceneNode> m_pRootSceneNode;

   private:
      virtual shared_ptr<SceneNode> VGetSceneNode(void);
   };

//---------------------------------------------------------------------------------------------------------------------
// This class represents a render component built from a Mesh.  In a real game, this is the one you'll use 99% of the 
// time towards the end of the project.  The other classes are important for testing since programming tends to move 
// a lot faster than art in the early stages of development.
//---------------------------------------------------------------------------------------------------------------------
class MeshRenderComponent : public BaseRenderComponent
   {
   public:
      MeshRenderComponent( void );
	   virtual const char *VGetName() const { return g_Name; }

   public:
      static const char *g_Name;

   protected:     
      /**
       * @brief factory method to create the appropriate scene node
       *
       * @param   void
       * @return shared_ptr<SceneNode>
       */
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override; 
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      Resource m_MeshResource;
      Resource m_TextureResource;
   };
