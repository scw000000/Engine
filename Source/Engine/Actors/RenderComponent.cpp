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

   
void IRenderComponent::Destory( void )
   {
   m_pRootSceneNode.reset();
   }

bool IRenderComponent::VInit( TiXmlElement* pData )
   {
   ENG_ASSERT( pData );
   return VDelegateInit( pData );
   }

void IRenderComponent::VPostInit( void )
   {
   shared_ptr<SceneNode> pSceneNode( VGetSceneNode() );
   shared_ptr<EvtData_New_Render_Component> pEvent( ENG_NEW EvtData_New_Render_Component( m_pOwner->GetId(), pSceneNode ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent ); // process this event immediately
   }

TiXmlElement* IRenderComponent::VGenerateXml( void )
   {
   TiXmlElement* pBaseElement = ENG_NEW TiXmlElement( VGetName().c_str() );

   //// color
   //TiXmlElement* pColor = ENG_NEW TiXmlElement( "Color" );
   //pColor->SetAttribute( "r", ToStr( m_color.r ).c_str() );
   //pColor->SetAttribute( "g", ToStr( m_color.g ).c_str() );
   //pColor->SetAttribute( "b", ToStr( m_color.b ).c_str() );
   //pColor->SetAttribute( "a", ToStr( m_color.a ).c_str() );
   //pBaseElement->LinkEndChild( pColor );

   // create XML for inherited classes
   VCreateInheritedXmlElements( pBaseElement );

   return pBaseElement;
   }

/*
void BaseRenderComponent::VOnChanged( void )
   {
   shared_ptr<EvtData_Modified_Render_Component> pEvent( ENG_NEW EvtData_Modified_Render_Component( m_pOwner->GetId() ) );
   IEventManager::GetSingleton()->VTriggerEvent( pEvent );
   }*/

shared_ptr<SceneNode> IRenderComponent::VGetSceneNode( void )
   {
   if( !m_pRootSceneNode )
      {
      m_pRootSceneNode = VCreateSceneNode();
      }
   return m_pRootSceneNode;
   }

Color IRenderComponent::LoadColor( TiXmlElement* pData )
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


//---------------------------------------------------------------------------------------------------------------------
// MeshRenderComponent
//---------------------------------------------------------------------------------------------------------------------

const ComponentId BaseRenderComponent<MeshRenderComponent>::s_ComponentId = 0x7a02ca99;
const std::string BaseRenderComponent<MeshRenderComponent>::s_Name = "MeshRenderComponent";

void MeshRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pMaterial.reset();
   m_pMeshResource.reset();
   }

MeshRenderComponent::MeshRenderComponent( void ) : m_pMeshResource( ENG_NEW Resource( "" ) ), m_pMaterial( ENG_NEW Material )
   {
   
   }

// This function is called by  ActorFactory Actor::PostInit->BaseRenderCompoenent::PostInit->VGetSceneNode->VCreateSceneNode
shared_ptr<SceneNode> MeshRenderComponent::VCreateSceneNode( void )
   {
   // get the transform component
   shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::s_ComponentId ) );
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
   TiXmlElement* pMeshElement = pData->FirstChildElement( "Mesh" );
   // Set mesh file path
   if( pMeshElement )
      {
      const char *pMeshFilePath = pMeshElement->Attribute( "path" );
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

   return true;
   }

/*
void MeshRenderComponent::VCreateInheritedXmlElements(TiXmlElement *)
{
    GCC_ERROR("MeshRenderComponent::VGenerateSubclassXml() not implemented");
}
*/
