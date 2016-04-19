////////////////////////////////////////////////////////////////////////////////
// Filename: RenderComponent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "RenderComponent.h"
#include "..\Event\EventManager.h"
#include "..\Event\Events.h"
#include "..\Graphics\MeshSceneNode.h"
#include "TransformComponent.h"


const char* MeshRenderComponent::g_Name = "MeshRenderComponent";

//---------------------------------------------------------------------------------------------------------------------
// RenderComponent
//---------------------------------------------------------------------------------------------------------------------
bool BaseRenderComponent::VInit( TiXmlElement* pData )
   {
   ENG_ASSERT( pData );
   return VDelegateInit( pData );
   }

void BaseRenderComponent::VPostInit( void )
   {
   shared_ptr<SceneNode> pSceneNode( VGetSceneNode() );
   shared_ptr<EvtData_New_Render_Component> pEvent( ENG_NEW EvtData_New_Render_Component( m_pOwner->GetId(), pSceneNode ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent ); // process this event immediately
   }

/*
void BaseRenderComponent::VOnChanged( void )
   {
   shared_ptr<EvtData_Modified_Render_Component> pEvent( ENG_NEW EvtData_Modified_Render_Component( m_pOwner->GetId() ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   }*/

/*
TiXmlElement* BaseRenderComponent::VGenerateXml(void)
{
    TiXmlElement* pBaseElement = VCreateBaseElement();

    // color
    TiXmlElement* pColor = GCC_NEW TiXmlElement("Color");
    pColor->SetAttribute("r", ToStr(m_color.r).c_str());
    pColor->SetAttribute("g", ToStr(m_color.g).c_str());
    pColor->SetAttribute("b", ToStr(m_color.b).c_str());
    pColor->SetAttribute("a", ToStr(m_color.a).c_str());
    pBaseElement->LinkEndChild(pColor);

    // create XML for inherited classes
    VCreateInheritedXmlElements(pBaseElement);

    return pBaseElement;
}
*/

shared_ptr<SceneNode> BaseRenderComponent::VGetSceneNode( void )
   {
   if( !m_pRootSceneNode )
      {
      m_pRootSceneNode = VCreateSceneNode();
      }
   return m_pRootSceneNode;
   }

//---------------------------------------------------------------------------------------------------------------------
// MeshRenderComponent
//---------------------------------------------------------------------------------------------------------------------
MeshRenderComponent::MeshRenderComponent( void ) : m_MeshResource( "" )
   {
   
   }

// This function is called by PostInit->VGetSceneNode->VCreateSceneNode
shared_ptr<SceneNode> MeshRenderComponent::VCreateSceneNode( void )
   {
   // get the transform component
   shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( ActorComponent::GetIdFromName( TransformComponent::g_Name ) ) );
   if ( !pTransformComponent )
      {
      // can't render without a transform
      return shared_ptr<SceneNode>();
      }

   WeakBaseRenderComponentPtr wbrcp(this);
   shared_ptr< SceneNode > pMeshSceneNode( ENG_NEW MeshSceneNode( m_pOwner->GetId(), wbrcp, m_MeshResource, RenderPass::RenderPass_Actor, &pTransformComponent->GetTransform() ) );
   
   return pMeshSceneNode;
   }

bool MeshRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pMeshFileElement = pData->FirstChildElement( "meshfile" );
   if( pMeshFileElement )
      {
      const char *filePath = pMeshFileElement->Attribute( "path" );
      if( filePath )
         {
         m_MeshResource = Resource( filePath );
         return true;
         }
      else
         {
         return false;
         }
      }
   return false;
   }

/*
void MeshRenderComponent::VCreateInheritedXmlElements(TiXmlElement *)
{
    GCC_ERROR("MeshRenderComponent::VGenerateSubclassXml() not implemented");
}
*/
