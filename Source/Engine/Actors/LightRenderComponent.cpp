////////////////////////////////////////////////////////////////////////////////
// Filename: LightRenderComponent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "LightRenderComponent.h"
#include "TransformComponent.h"
#include "..\Graphics\Light.h"

const char* LightRenderComponent::g_Name = "LightRenderComponent";

//---------------------------------------------------------------------------------------------------------------------
// LightRenderComponent
//---------------------------------------------------------------------------------------------------------------------
LightRenderComponent::LightRenderComponent(void) : m_pLightProps( ENG_NEW( LightProperties ) )
   {
   }

// This function is calle by BaseRenderComponent
bool LightRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pDiffuseNode = pData->FirstChildElement( "Diffuse" );
   if( pDiffuseNode )
      {
      m_pLightProps->m_Color = BaseRenderComponent::LoadColor( pDiffuseNode );
      }

   TiXmlElement* pPowerNode = NULL;
	pPowerNode = pData->FirstChildElement("Power");
   if ( pPowerNode )
	   {
      double temp;
		pPowerNode->Attribute( "magnitude", &temp );
		m_pLightProps->m_Power = (float) temp;
	   }

    return true;
   }

shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode(void)
   {
   shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::g_Name ) );
   if (pTransformComponent)
      {
		WeakBaseRenderComponentPtr weakThis(this);

		switch ( EngineApp::GetRendererImpl() )
		   {
         case Renderer::Renderer_OpenGL :
            {
				return shared_ptr<SceneNode>( ENG_NEW LightNode( m_pOwner->GetId(), weakThis, m_pLightProps, pTransformComponent->GetTransform( ) ) );  
            }
         default:
				ENG_ASSERT( 0 && "Unknown Renderer Implementation in GridRenderComponent" );
		   }
	   }
    return shared_ptr<SceneNode>();
   }

//void LightRenderComponent::VCreateInheritedXmlElements(TiXmlElement *pBaseElement)
//{
//    TiXmlElement* pSceneNode = ENG_NEW TiXmlElement("Light");
//
//    // attenuation
//    TiXmlElement* pAttenuation = ENG_NEW TiXmlElement("Attenuation");
//    pAttenuation->SetAttribute("const", ToStr(m_Props.m_Attenuation[0]).c_str());
//    pAttenuation->SetAttribute("linear", ToStr(m_Props.m_Attenuation[1]).c_str());
//    pAttenuation->SetAttribute("exp", ToStr(m_Props.m_Attenuation[2]).c_str());
//    pSceneNode->LinkEndChild(pAttenuation);
//
//    // shape
//    TiXmlElement* pShape = ENG_NEW TiXmlElement("Shape");
//    pShape->SetAttribute("range", ToStr( m_Props.m_Range ).c_str());
//    pShape->SetAttribute("falloff", ToStr(m_Props.m_Falloff).c_str());
//    pShape->SetAttribute("theta", ToStr(m_Props.m_Theta).c_str());
//    pShape->SetAttribute("phi", ToStr(m_Props.m_Phi).c_str());
//    pSceneNode->LinkEndChild(pShape);
//
//    pBaseElement->LinkEndChild(pSceneNode);
//}

