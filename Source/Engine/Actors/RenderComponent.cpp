/*!
 * \file RenderComponent.cpp
 * \date 2016/05/05 15:43
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

     color
    TiXmlElement* pColor = GCC_NEW TiXmlElement("Color");
    pColor->SetAttribute("r", ToStr(m_color.r).c_str());
    pColor->SetAttribute("g", ToStr(m_color.g).c_str());
    pColor->SetAttribute("b", ToStr(m_color.b).c_str());
    pColor->SetAttribute("a", ToStr(m_color.a).c_str());
    pBaseElement->LinkEndChild(pColor);

     create XML for inherited classes
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
MeshRenderComponent::MeshRenderComponent( void ) : m_pMeshResource( ENG_NEW Resource( "" ) ), m_pMaterial( ENG_NEW Material )
   {
   
   }

// This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
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
   shared_ptr< SceneNode > pMeshSceneNode( ENG_NEW MeshSceneNode( m_pOwner->GetId(), wbrcp, m_pMeshResource, m_pMaterial, RenderPass::RenderPass_Actor, pTransformComponent->GetTransform() ) );
   
   return pMeshSceneNode;
   }

bool MeshRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pMeshFileElement = pData->FirstChildElement( "Mesh" );
   // Set mesh file path
   if( pMeshFileElement )
      {
      const char *pMeshFilePath = pMeshFileElement->Attribute( "path" );
      if( !pMeshFilePath )
         {
         return false;
         }
      m_pMeshResource = shared_ptr<Resource>( ENG_NEW Resource( pMeshFilePath ) );
      }
   else
      {
      return false;
      }
   // Set texture file path
   TiXmlElement* pMaterialElement = pData->FirstChildElement( "Material" );
   if( pMaterialElement )
      {
      TiXmlElement* pTextureFileElement = pMaterialElement->FirstChildElement( "Texture" );
      if( pTextureFileElement )
         {
         const char *pTextureFilePath = pTextureFileElement->Attribute( "path" );
         if( pTextureFilePath )
            {
            m_pMaterial->SetTextureResource( Resource( pTextureFilePath ) );
            }
         }
      TiXmlElement* pDiffuseElement = pMaterialElement->FirstChildElement( "Diffuse" );
      if( pDiffuseElement )
         {
         m_pMaterial->SetDiffuse( BaseRenderComponent::LoadColor( pDiffuseElement ) );
         }
      }

   //// Set texture file path
   //TiXmlElement* pTextureFileElement = pData->FirstChildElement( "Texture" );
   //if( pTextureFileElement )
   //   {
   //   const char *pTextureFilePath = pTextureFileElement->Attribute( "path" );
   //   if( !pTextureFilePath )
   //      {
   //      return false;
   //      }
   //   else
   //      {
   //      m_pMaterial->SetTextureResource( Resource( pTextureFilePath ) );
   //      }
   //   }
   //else
   //   {
   //   return false;      
   //   }

   return true;
   }

Color BaseRenderComponent::LoadColor( TiXmlElement* pData )
   {
   Color color;

   double r = 1.0;
   double g = 1.0;
   double b = 1.0;
   double a = 1.0;

   pData->Attribute( "r", &r );
   pData->Attribute( "g", &g );
   pData->Attribute( "b", &b );
   pData->Attribute( "a", &a );

   color.m_Component.r = ( float ) r;
   color.m_Component.g = ( float ) g;
   color.m_Component.b = ( float ) b;
   color.m_Component.a = ( float ) a;

   return color;
   }


/*
void MeshRenderComponent::VCreateInheritedXmlElements(TiXmlElement *)
{
    GCC_ERROR("MeshRenderComponent::VGenerateSubclassXml() not implemented");
}
*/
