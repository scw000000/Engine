////////////////////////////////////////////////////////////////////////////////
// Filename: RenderComponent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "RenderComponent.h"
#include "..\Event\EventManager.h"


const char* MeshRenderComponent::g_Name = "MeshRenderComponent";

//---------------------------------------------------------------------------------------------------------------------
// RenderComponent
//---------------------------------------------------------------------------------------------------------------------
bool BaseRenderComponent::VInit( TiXmlElement* pData )
   {
    // color
    TiXmlElement* pColorNode = pData->FirstChildElement( "Color" );
    if( pColorNode )
       {
       m_color = LoadColor( pColorNode );
       }

    return VDelegateInit(pData);
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
   if( !m_pSceneNode )
      {
      m_pSceneNode = VCreateSceneNode();
      }
   return m_pSceneNode;
   }

Color BaseRenderComponent::LoadColor( TiXmlElement* pData )
   {
	Color color;

   double r = 1.0;
   double g = 1.0;
   double b = 1.0;
   double a = 1.0;

   pData->Attribute("r", &r);
   pData->Attribute("g", &g);
   pData->Attribute("b", &b);
   pData->Attribute("a", &a);

   color.m_Component.r = (float)r;
   color.m_Component.g = (float)g;
   color.m_Component.b = (float)b;
   color.m_Component.a = (float)a;

	return color;
   }


//---------------------------------------------------------------------------------------------------------------------
// MeshRenderComponent
//---------------------------------------------------------------------------------------------------------------------
bool MeshRenderComponent::VInit( TiXmlElement* pData )
   {
    // color
    TiXmlElement* pColorNode = pData->FirstChildElement( "Color" );
    if( pColorNode )
       {
       m_color = LoadColor( pColorNode );
       }
    TiXmlElement* pMeshFileNode = pData->FirstChildElement( "MeshFile" );
    if( pMeshFileNode )
       {
       m_MeshResource = Resource( pMeshFileNode->GetText() );
      
      // m_pMeshResource = g_pApp->m_pResCache->GetHandle( &Resource( "fucjk" ) );
       }

    return VDelegateInit(pData);
   }

shared_ptr<SceneNode> MeshRenderComponent::VCreateSceneNode( void )
   {
   return shared_ptr<SceneNode>();
   }

/*
void MeshRenderComponent::VCreateInheritedXmlElements(TiXmlElement *)
{
    GCC_ERROR("MeshRenderComponent::VGenerateSubclassXml() not implemented");
}
*/
