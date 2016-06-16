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
#include "..\Event\EventManager.h"
#include "..\Event\Events.h"
#include "..\Graphics\MeshSceneNode.h"
#include "TransformComponent.h"

#pragma warning( push )
#pragma warning( disable : 4250 )

class IRenderComponent : virtual public IActorComponent
   {
   friend class Actor;
   public:
      virtual ~IRenderComponent( void ){ }
      virtual shared_ptr<SceneNode> VGetSceneNode( void ) = 0;
      virtual TransformPtr VGetTransform( void ) = 0;

   protected:
      virtual void VBuildSceneNode( SceneNode* pParentNode ) = 0;
      /**
      * @brief loads the SceneNode specific data ( represented in the <SceneNode> tag )
      *
      * @param  pData TiXmlElement * pData
      * @return bool
      */
      virtual bool VDelegateInit( TiXmlElement* pData ) = 0;
      // factory method to create the appropriate scene node
      // This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) = 0;
      virtual void VDelegateGenerateXML( TiXmlElement* pBaseElement ) = 0;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) = 0;

   protected:
      
   };

template <typename T>class BaseRenderComponent : public IRenderComponent, public BaseActorComponent<T>
   {
   friend class Actor;

   public:
      BaseRenderComponent( void );
      virtual void Destory( void ) override;
      virtual TransformPtr VGetTransform( void ) override final { return m_pTransform; }
      /**
         * @brief set member variable from xml elements
         *
         * This function should not override by derived Rendercomponent
         * you should override VDelegateInit instead
         * @param  pData TiXmlElement * pData
         * @return bool
         */
      virtual bool VInit( TiXmlElement* pData ) override final;
      virtual void VPostInit( void ) override final;
   	//virtual void VOnChanged( void ) override;
      virtual TiXmlElement* VGenerateXML( void ) override final;
      virtual shared_ptr<SceneNode> VGetSceneNode( void ) override final;
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) override final;

   protected:
      virtual void VBuildSceneNode( SceneNode* pParentNode ) override final;
      /**
       * @brief factory method to create the appropriate scene node
       *        this function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
       * @param   void
       * @return shared_ptr<SceneNode>
       */
       virtual shared_ptr<SceneNode> VCreateSceneNode( void ) = 0;  

   protected:
         shared_ptr<SceneNode> m_pSceneNode;
         TransformPtr m_pTransform;
   
   private:
      
   };

   
template < typename T > BaseRenderComponent<T>::BaseRenderComponent( void )
   {
   m_pTransform = TransformPtr( ENG_NEW Transform( Transform::g_Identity ) );
   }

template < typename T > void BaseRenderComponent<T>::Destory( void )
   {
   m_pSceneNode.reset();
   m_pTransform.reset();
   }

template < typename T > bool BaseRenderComponent<T>::VInit( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   m_pTransform->Init( pData->FirstChildElement( "Transform" ) );
   return VDelegateInit( pData );
   }

template < typename T > void BaseRenderComponent<T>::VPostInit( void )
   {
   if( m_pParentComponent.expired() )
      {
      shared_ptr<SceneNode> pSceneNode( VGetSceneNode() );
      shared_ptr<Event_New_Render_Component_Root> pEvent( ENG_NEW Event_New_Render_Component_Root( m_pOwner->GetId(), pSceneNode ) );
      IEventManager::GetSingleton()->VTriggerEvent( pEvent ); // process this event immediately
      }
   }

template < typename T > TiXmlElement* BaseRenderComponent<T>::VGenerateXML( void )
   {
   TiXmlElement* pBaseElement = ENG_NEW TiXmlElement( VGetName().c_str() );
   
   TiXmlElement* pTransform = m_pTransform->GenerateXML();
   pBaseElement->LinkEndChild( pTransform );

   VDelegateGenerateXML( pBaseElement );

   return pBaseElement;
   }

template < typename T > void BaseRenderComponent<T>::VBuildSceneNode( SceneNode* pParentNode )
   {
   m_pSceneNode = VCreateSceneNode();
   m_pSceneNode->VSetParentNode( pParentNode );
   for( auto pChildComponent : m_ChildComponents )
      {
      shared_ptr< IRenderComponent > pChildRenderComponent = dynamic_pointer_cast< IRenderComponent >( pChildComponent.lock() );
      if( pChildRenderComponent )
         {
         m_pSceneNode->VAddChild( pChildRenderComponent->VGetSceneNode() );
         }
      }
   shared_ptr< IEvent > pEvent( ENG_NEW Event_New_Scene_Node( m_pSceneNode ) );
   EventManager::GetSingleton()->VTriggerEvent( pEvent );
   }

/*
void BaseRenderComponent::VOnChanged( void )
   {
   shared_ptr<EvtData_Modified_Render_Component> pEvent( ENG_NEW EvtData_Modified_Render_Component( m_pOwner->GetId() ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   }*/

template < typename T > shared_ptr<SceneNode> BaseRenderComponent<T>::VGetSceneNode( void )
   {
   if( !m_pSceneNode )
      {
      SceneNode* pParentSceneNode = NULL;
      if( !m_pParentComponent.expired() )
         {
         shared_ptr< IRenderComponent > pParentRenderComponent = dynamic_pointer_cast< IRenderComponent >( m_pParentComponent.lock() );
         if( pParentRenderComponent )
            {
            pParentSceneNode = &*( pParentRenderComponent->VGetSceneNode() );
            }
         }
      VBuildSceneNode( pParentSceneNode );
      }
   return m_pSceneNode;
   }

template < typename T > TiXmlElement* BaseRenderComponent<T>::VGenerateOverridesXML( TiXmlElement* pResourceNode )
   {
   TiXmlElement* pBaseElement = ENG_NEW TiXmlElement( VGetName().c_str() );

   TiXmlElement* pTransform = m_pTransform->GenerateOverridesXML( pResourceNode->FirstChildElement( "Transform" ) );
   pBaseElement->LinkEndChild( pTransform );

   VDelegateGenerateOverridesXML( pBaseElement, pResourceNode );

   return pBaseElement;
   }

//
//class RenderComponent : virtual public IActorComponent
//   {
//   friend class Actor;
//   public:
//      RenderComponent( void );
//      virtual void Destory( void ) override;
//      /**
//       * @brief set member variable from xml elements
//       *
//       * This function should not override by derived Rendercomponent
//       * you should override VDelegateInit instead
//       * @param  pData TiXmlElement * pData
//       * @return bool
//       */
//      virtual bool VInit( TiXmlElement* pData ) override;
//      virtual void VPostInit( void ) override;
//      virtual void VUpdate( const unsigned long deltaMs ) override { };
//	   //virtual void VOnChanged( void ) override;
//      virtual TiXmlElement* VGenerateXML( void ) override;
//
//   protected:
//      /*virtual void SetOwner( StrongActorPtr pOwner ) override { m_pOwner = pOwner; }
//      virtual void AddChildComponent( weak_ptr<IActorComponent> pChild ) override { m_ChildComponents.push_back( pChild ); }
//      virtual void SetParentComponent( weak_ptr<IActorComponent> pParent ) override { m_pParentComponent = pParent; }*/
//      void BuildSceneNode( SceneNode* pParentNode );
//      /**
//       * @brief loads the SceneNode specific data ( represented in the <SceneNode> tag )
//       *
//       * @param  pData TiXmlElement * pData
//       * @return bool
//       */
//      virtual bool VDelegateInit( TiXmlElement* pData ) { return true; }
//      // factory method to create the appropriate scene node
//      // This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
//      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) = 0;  
//      virtual void VCreateInheritedXmlElements( TiXmlElement* pBaseElement ) = 0; 
//
//   protected:
//      StrongActorPtr m_pOwner;
//      /*weak_ptr<IActorComponent> m_pParentComponent;
//      ChildComponents m_ChildComponents;*/
//      shared_ptr<SceneNode> m_pSceneNode;
//      TransformPtr m_pTransform;
//
//   private:
//      virtual shared_ptr<SceneNode> VGetSceneNode( void );
//   };
//
//template <typename T>class BaseRenderComponent : public RenderComponent, public BaseActorComponent<T>
//   {
//   public:
//
//      virtual ComponentId VGetId( void ) const override
//         {
//         return s_ComponentId;
//         };
//
//      virtual const std::string& VGetName( void ) const override
//         {
//         return s_Name;
//         }
//
//   public:
//      // GUID of this event
//      const static ComponentId  s_ComponentId;
//      const static std::string      s_Name;
//   };

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
      virtual void VDelegateGenerateXML( TiXmlElement* pBaseElement ) override;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) override;

   protected:
      MaterialPtr m_pMaterial;
      shared_ptr<Resource> m_pMeshResource;
   };

#pragma warning( pop )