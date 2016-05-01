////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Light.h"
#include "..\Actors\LightRenderComponent.h"


LightNode::LightNode( const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightProperties &props, TransformPtr pTransform )
 : SceneNode( actorId, renderComponent,  RenderPass_NotRendered, pTransform ) 
   {
	m_LightProps = props;
   }

int GLLightNode::VOnUpdate( Scene *, const unsigned long deltaMs )
   { 
	// light color can change anytime! Check the BaseRenderComponent!
	LightRenderComponent* lrc = static_cast<LightRenderComponent*>( m_RenderComponent );
	/*m_Props.GetMaterial().SetDiffuse( lrc->GetLightProperties().m_Color );*/
	return S_OK; 
   }


//
// LightManager::CalcLighting					- Chapter 16, page 554
//
void LightManager::CalcLighting(Scene *pScene)
   {
	//// FUTURE WORK: There might be all kinds of things you'd want to do here for optimization, especially turning off lights on actors that can't be seen, etc.
	//pScene->GetRenderer()->VCalcLighting( &m_Lights, MAXIMUM_LIGHTS_SUPPORTED );

	//int count = 0;

	//ENG_ASSERT( m_Lights.size() < MAXIMUM_LIGHTS_SUPPORTED );
	//for( Lights::iterator i=m_Lights.begin(); i!=m_Lights.end(); ++i, ++count )
	//   {
	//	shared_ptr<LightNode> light = *i;

	//	if (count==0)
	//	   {
	//		// Light 0 is the only one we use for ambient lighting. The rest are ignored in the simple shaders used for GameCode4.
	//		Color ambient = light->VGetProperties().GetMaterial().GetAmbient();
	//		m_vLightAmbient = Vec4( ambient.m_Component.r, ambient.m_Component.g, ambient.m_Component.b, 1.0f ); 		
	//	   }

	//	Vec3 lightDir = light->GetDirection();
	//	m_vLightDir[count] = Vec4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
	//	m_vLightDiffuse[count] = light->VGetProperties()->GetMaterial().GetDiffuse();
	//   }
   }  


void LightManager::CalcLighting( SceneNode *pNode )
   {
	/*int count = GetLightCount( pNode );
	if (count)
	   {
		pLighting->m_vLightAmbient = *GetLightAmbient(pNode);
		memcpy(pLighting->m_vLightDir, GetLightDirection(pNode), sizeof( Vec4 ) * count );
		memcpy(pLighting->m_vLightDiffuse, GetLightDiffuse(pNode), sizeof( Vec4 ) * count);
		pLighting->m_nNumLights = count;
	   }*/
   }

