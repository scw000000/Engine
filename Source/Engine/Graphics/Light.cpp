////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Light.h"
#include "..\Actors\LightRenderComponent.h"


LightNode::LightNode( const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightPropertiesPtr& props, TransformPtr pTransform )
 : SceneNode( actorId, renderComponent,  RenderPass_NotRendered, pTransform ) 
   {
	m_pLightProps = props;
   }

int GLLightNode::VOnUpdate( Scene *, const unsigned long deltaMs )
   { 
	// light color can change anytime! Check the BaseRenderComponent!
	LightRenderComponent* lrc = static_cast<LightRenderComponent*>( m_RenderComponent );
	/*m_Props.GetMaterial().SetDiffuse( lrc->GetLightProperties().m_Color );*/
	return S_OK; 
   }


void LightManager::CalcLighting( Scene *pScene )
   {
	//// LATER: There might be all kinds of things you'd want to do here for optimization, especially turning off lights on actors that can't be seen, etc.


	ENG_ASSERT( m_ActiveLights.size() <= MAXIMUM_LIGHTS_SUPPORTED );

   for( Lights::iterator lightIt = m_ActiveLights.begin( ); lightIt != m_ActiveLights.end( ); ++lightIt )
      {
      if( lightIt == m_ActiveLights.begin() )
         {
         m_LightAmbient = lightIt->get()->GetLightPropertiesPtr()->m_Color * 0.2f;
         }
      memcpy( m_LightPosWorldSpace, &lightIt->get( )->GetWorldPosition( ), sizeof( Vec3 ) );
      memcpy( m_LightDir, &lightIt->get( )->GetForward( ), sizeof( Vec3 ) );
      memcpy( m_LightPower, &lightIt->get( )->GetLightPropertiesPtr( )->m_Power, sizeof( float ) );
      memcpy( m_LightColor, &lightIt->get( )->GetLightPropertiesPtr( )->m_Color, sizeof( Color ) );
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

