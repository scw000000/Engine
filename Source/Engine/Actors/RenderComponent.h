#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: RenderComponent.h
////////////////////////////////////////////////////////////////////////////////

#include "ActorComponent.h"

// Later: add functions for editor
// TODO: refactor to support component with  multi sceneNode
class BaseRenderComponent : public ActorComponent
   {
   public:
      // This function should not override by derived Rendercomponent
      // override VDelegateInit instedad
      virtual bool VInit( TiXmlElement* pData ) override;
      virtual void VPostInit( void ) override;
	   //virtual void VOnChanged( void ) override;
      //virtual TiXmlElement* VGenerateXml(void) override;

   protected:
      // loads the SceneNode specific data (represented in the <SceneNode> tag)
      virtual bool VDelegateInit( TiXmlElement* pData ) { return true; }
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) = 0;  // factory method to create the appropriate scene node
      Color LoadColor( TiXmlElement* pData );

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
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override;  // factory method to create the appropriate scene node
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      Resource m_MeshResource;
      Resource m_TextureResource;
   };
