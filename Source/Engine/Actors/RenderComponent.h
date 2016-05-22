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

class IRenderComponent : virtual public IActorComponent
   {
   public:
      virtual void Destory( void ) override;
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
      virtual void VUpdate( const unsigned long deltaMs ) override { };
	   //virtual void VOnChanged( void ) override;
      //virtual TiXmlElement* VGenerateXml(void) override;

   protected:
      virtual void SetOwner( StrongActorPtr pOwner ) override
         {
         m_pOwner = pOwner;
         }
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
      StrongActorPtr m_pOwner;
      shared_ptr<SceneNode> m_pRootSceneNode;
      
   private:
      virtual shared_ptr<SceneNode> VGetSceneNode(void);
   };

template <typename T>class BaseRenderComponent : public IRenderComponent 
   {
   public:

      virtual ComponentId VGetId( void ) const override
         {
         return s_ComponentId;
         };

      virtual const std::string& VGetName( void ) const override
         {
         return s_Name;
         }

   public:
      // GUID of this event
      const static ComponentId  s_ComponentId;
      const static std::string      s_Name;
   };

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
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      MaterialPtr m_pMaterial;
      shared_ptr<Resource> m_pMeshResource;
   };
