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
#include "..\Physics\Physics.h"
#include "..\Physics\PhysicsAttributes.h"

class BulletPhysicsAttributes;

#pragma warning( push )
#pragma warning( disable : 4250 )

class IRenderComponent : virtual public IActorComponent
   {
   friend class Actor;
   public:
      virtual ~IRenderComponent( void ){ }
      virtual shared_ptr<SceneNode> VGetSceneNode( void ) = 0;
      virtual void VSetTransform( const Transform& transform ) = 0;
      virtual TransformPtr VGetTransformPtr( void ) const = 0;
      virtual shared_ptr< IPhysicsAttributes > VGetPhysicsAttributes( void ) const = 0;
      virtual StrongRenderComponentPtr VGetSelfStrongRenderCompPtr( void ) const = 0;
      //virtual shared_ptr< BulletPhysicsAttributes > VGetPhysicsAttributesPtr( void ) const = 0;

   protected:
      virtual void VBuildSceneNode( SceneNode* pParentNode ) = 0;
      /**
      * @brief loads the SceneNode specific data ( represented in the <SceneNode> tag )
      *
      * @param  pData TiXmlElement * pData
      * @return bool
      */
      virtual bool VDelegateInit( TiXmlElement* pData ) = 0;
      virtual void VDelegatePostInit( void ) = 0;
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
      virtual TransformPtr VGetTransformPtr( void ) const override final{ return m_pTransform; }
      virtual StrongRenderComponentPtr VGetSelfStrongRenderCompPtr( void ) const override;
      virtual void VSetTransform( const Transform& transform ) override final;
      /**
         * @brief set member variable from xml elements
         *
         * This function should not override by derived Rendercomponent
         * you should override VDelegateInit instead
         * @param  pData TiXmlElement * pData
         * @return bool
         */
      virtual bool VInit( TiXmlElement* pData ) override final;
      virtual bool VDelegateInit( TiXmlElement* pData ) override { return true; }
      virtual void VPostInit( void ) override final;
      virtual void VDelegatePostInit( void ) {  };
   	//virtual void VOnChanged( void ) override;
      virtual TiXmlElement* VGenerateXML( void ) override final;
      virtual shared_ptr<SceneNode> VGetSceneNode( void ) override final;
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) override final;
      virtual void VSyncTransform( void );
      virtual shared_ptr< IPhysicsAttributes > VGetPhysicsAttributes( void ) const override { return m_pPhysicsAttributes; }

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
         shared_ptr< IPhysicsAttributes > m_pPhysicsAttributes;

   private:
      
   };

   
template < typename T > BaseRenderComponent<T>::BaseRenderComponent( void ) : m_pTransform( ENG_NEW Transform( Transform::g_Identity ) )
   {
   }

template < typename T > void BaseRenderComponent<T>::Destory( void )
   {
   shared_ptr<Event_Destory_Render_Component> pEvent( ENG_NEW Event_Destory_Render_Component( VGetSelfStrongRenderCompPtr() ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   m_pSceneNode.reset();
   m_pTransform.reset();
   m_pPhysicsAttributes.reset();
   }

template < typename T > StrongRenderComponentPtr BaseRenderComponent<T>::VGetSelfStrongRenderCompPtr( void ) const 
   {
   auto comp = this->VGetSelfWeakActorComponentPtr().lock();
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( comp );
   ENG_ASSERT( pRenderComp );
   return pRenderComp;
   }

template < typename T > void BaseRenderComponent<T>::VSetTransform( const Transform& transform )
   {
   *m_pTransform = transform;
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
      shared_ptr<Event_New_Render_Root_Component> pEvent( ENG_NEW Event_New_Render_Root_Component( m_pOwner->GetId(), pSceneNode ) );
      IEventManager::GetSingleton()->VTriggerEvent( pEvent ); // process this event immediately
      }
   VDelegatePostInit();
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

template < typename T > void BaseRenderComponent<T>::VSyncTransform( void )
   {
   if( !m_pPhysicsAttributes )
      {

      return;
      }

   if( m_pPhysicsAttributes->VIsLinkedToPhysicsWorld() )
      {
      StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( VGetSelfWeakActorComponentPtr().lock() );
      ENG_ASSERT( pRenderComp );
      IGamePhysics::GetSingleton().VSyncRigidBodyToRenderComponent( pRenderComp );
      }
   }

#pragma warning( pop )