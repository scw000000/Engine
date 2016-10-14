/*!
 * \file Light.cpp
 * \date 2016/06/12 11:24
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
#include "Light.h"
#include "..\Actors\LightRenderComponent.h"
#include "..\Event\Events.h"
#include "..\Event\EventManager.h"


bool LightProperties::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   if( !m_Diffuse.Init( pData->FirstChildElement( "Diffuse" ) ) )
      {
      return false;
      }
   if( TIXML_SUCCESS != pData->QueryFloatAttribute( "power", &m_Power ) )
      {
      return false;
      }

   return true;
   }

TiXmlElement* LightProperties::GenerateXML( void )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Light" );

   TiXmlElement* pDiffuse = m_Diffuse.GenerateXML();
   pDiffuse->SetValue( "Diffuse" );
   pRetNode->LinkEndChild( pDiffuse );

   pRetNode->SetAttribute( "power", ToStr( m_Power ).c_str() );

   /*TiXmlElement* pDiffuse = ENG_NEW TiXmlElement( "Diffuse" );
   pDiffuse->SetAttribute( "r", ToStr( m_Diffuse.m_Component.r ).c_str() );
   pDiffuse->SetAttribute( "g", ToStr( m_Diffuse.m_Component.g ).c_str() );
   pDiffuse->SetAttribute( "b", ToStr( m_Diffuse.m_Component.b ).c_str() );
   pDiffuse->SetAttribute( "a", ToStr( m_Diffuse.m_Component.a ).c_str() );
   pBaseElement->LinkEndChild( pDiffuse );

   TiXmlElement* pPower = ENG_NEW TiXmlElement( "Power" );
   pPower->SetAttribute( "magnitude", ToStr( m_Power ).c_str() );
   pBaseElement->LinkEndChild( pPower );*/

   return pRetNode;
   }

TiXmlElement* LightProperties::GenerateOverridesXML( TiXmlElement* pResource ) 
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Light" );

   TiXmlElement* pDiffuse = m_Diffuse.GenerateOverridesXML( pResource->FirstChildElement( "Diffuse" ) );
   pDiffuse->SetValue( "Diffuse" );
   pRetNode->LinkEndChild( pDiffuse );

   pRetNode->SetAttribute( "power", ToStr( m_Power ).c_str() );
   if( !strcmp( pRetNode->Attribute( "power" ), pResource->Attribute( "power" ) ) )
      {
      pRetNode->RemoveAttribute( "power" );
      }
   return pRetNode;
   }

LightNode::LightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& props, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, RenderPass_NotRendered, pTransform )
   {
	m_pLightProps = props;
   }

int GLLightNode::VOnUpdate( Scene *, const unsigned long deltaMs )
   { 
	// light color can change anytime! Check the BaseRenderComponent!
	//LightRenderComponent* lrc = static_cast<LightRenderComponent*>( m_pRenderComponent );
	/*m_Props.GetMaterial().SetDiffuse( lrc->GetLightProperties().m_Color );*/
	return S_OK; 
   }

LightManager::LightManager( void )
   {
   IEventManager* pEventMgr = IEventManager::GetSingleton();
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::NewSceneNodeDelegate ), Event_New_Scene_Node::s_EventType );
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::DestroySceneNodeDelegate ), Event_Destroy_Scene_Node::s_EventType );
   }

void LightManager::CalcLighting( Scene *pScene )
   {
	//// LATER: There might be all kinds of things you'd want to do here for optimization, especially turning off lights on actors that can't be seen, etc.


	ENG_ASSERT( m_ActiveLights.size() <= MAXIMUM_LIGHTS_SUPPORTED );

   for( Lights::iterator lightIt = m_ActiveLights.begin( ); lightIt != m_ActiveLights.end( ); ++lightIt )
      {
      /*if( lightIt == m_ActiveLights.begin() )
         {
         m_LightAmbient = lightIt->get()->GetLightPropertiesPtr()->m_Diffuse * 0.2f;
         }*/
      memcpy( m_LightPosWorldSpace, &lightIt->get( )->VGetWorldPosition( ), sizeof( Vec3 ) );
      memcpy( m_LightDir, &lightIt->get( )->GetForward( ), sizeof( Vec3 ) );
      memcpy( m_LightPower, &lightIt->get( )->GetLightPropertiesPtr( )->m_Power, sizeof( float ) );
      memcpy( m_LightColor, &lightIt->get()->GetLightPropertiesPtr()->m_Diffuse, sizeof( Color ) );
      }
   }  


void LightManager::CalcLighting( SceneNode *pNode )
   {
   
	/*int count = m_ActiveLights( pNode );
	if (count)
	   {
            Vec3     m_LightPosWorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3		m_LightDir[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3     m_LightPower[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color		m_LightDiffuse[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3		m_LightAmbient;
		pLighting->m_vLightAmbient = *GetLightAmbient(pNode);
		memcpy(pLighting->m_vLightDir, GetLightDirection(pNode), sizeof( Vec4 ) * count );
		memcpy(pLighting->m_vLightDiffuse, GetLightDiffuse(pNode), sizeof( Vec4 ) * count);
		pLighting->m_nNumLights = count;
	   }*/
   }

bool LightManager::AddLightNode( shared_ptr<LightNode> pNewLight )
   {
 
   m_Lights.push_back( pNewLight );
   if( m_ActiveLights.size() + 1 < MAXIMUM_LIGHTS_SUPPORTED )
      {
      m_ActiveLights.push_back( pNewLight );
      }
   return true;
   }

bool LightManager::RemoveLightNode( shared_ptr<LightNode> pRemovedLight )
   {
   m_ActiveLights.remove( pRemovedLight );
   m_Lights.remove( pRemovedLight );
   return true;
   }

void LightManager::NewSceneNodeDelegate( IEventPtr pEvent )
   {
   shared_ptr< Event_New_Scene_Node > pNewSceneNodeEvt = dynamic_pointer_cast< Event_New_Scene_Node >( pEvent );
   shared_ptr< LightNode > pLightNode = dynamic_pointer_cast< LightNode >( pNewSceneNodeEvt->GetSceneNode().lock() );
   if( pLightNode )
      {
      AddLightNode( pLightNode );
      }
   }

void LightManager::DestroySceneNodeDelegate( IEventPtr pEvent )
   {
   shared_ptr< Event_Destroy_Scene_Node > pDerivedEvt = dynamic_pointer_cast< Event_Destroy_Scene_Node >( pEvent );
   shared_ptr< LightNode > pLightNode = dynamic_pointer_cast< LightNode >( pDerivedEvt->GetSceneNodePtr().lock() );
   if( pLightNode )
      {
      RemoveLightNode( pLightNode );
      }
   }