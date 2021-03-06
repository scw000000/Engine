/*!
 * \file LightRenderComponent.cpp
 * \date 2016/05/22 16:32
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
#include "LightRenderComponent.h"
#include "..\Graphics\SceneNode\DirectionalLightSceneNode.h"

//const char* LightRenderComponent::g_Name = "LightRenderComponent";

const ComponentRegisterId BaseActorComponent<LightRenderComponent>::s_ComponentId = 0xf501193f;
const std::string BaseActorComponent<LightRenderComponent>::s_Name = "LightRenderComponent";

void LightRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pLightProps.reset();
   }

//---------------------------------------------------------------------------------------------------------------------
// LightRenderComponent
//---------------------------------------------------------------------------------------------------------------------
LightRenderComponent::LightRenderComponent(void) : m_pLightProps( ENG_NEW( LightProperties ) )
   {
   }

// This function is calle by BaseRenderComponent
bool LightRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   m_pLightProps->Init( pData->FirstChildElement( "Light" ) );

   return true;
   }

shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode( void )
   {
   return shared_ptr<SceneNode>( ENG_NEW DirectionalLightNode( m_pOwner->GetId(), this, m_pLightProps, m_pTransform ) );

   /*shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::s_ComponentId ) );
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
   }*/
   // return shared_ptr<SceneNode>();
   }

void LightRenderComponent::VDelegateGenerateXML( TiXmlElement *pBaseElement )
   {
   TiXmlElement* pLight = m_pLightProps->GenerateXML();
   pBaseElement->LinkEndChild( pLight );
   }

void LightRenderComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {
//   TiXmlElement* pLight = m_pLightProps->GenerateOverridesXML( pResourceNode->FirstChildElement( "Light" ) );
//   pBaseElement->LinkEndChild( pLight );
   }
