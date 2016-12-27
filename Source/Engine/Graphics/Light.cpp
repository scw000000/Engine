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
   if( !m_Color.Init( pData->FirstChildElement( "Diffuse" ) ) )
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

   TiXmlElement* pDiffuse = m_Color.GenerateXML();
   pDiffuse->SetValue( "Diffuse" );
   pRetNode->LinkEndChild( pDiffuse );

   pRetNode->SetAttribute( "power", ToStr( m_Power ).c_str() );

   return pRetNode;
   }

TiXmlElement* LightProperties::GenerateOverridesXML( TiXmlElement* pResource ) 
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Light" );

   TiXmlElement* pDiffuse = m_Color.GenerateOverridesXML( pResource->FirstChildElement( "Diffuse" ) );
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
   : SceneNode( actorId, pRenderComponent, RenderGroup_NotRendered, pTransform )
   {
	m_pLightProps = props;
   }

LightManager::LightManager( void )
   {
   IEventManager* pEventMgr = IEventManager::GetSingleton();
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::NewSceneNodeDelegate ), Event_New_Scene_Node::s_EventType );
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::DestroySceneNodeDelegate ), Event_Destroy_Scene_Node::s_EventType );
 //  m_DeferredShader.VOnRestore();
   }

void LightManager::RenderShadowMap( shared_ptr< LightNode > ) const
   {
   
   }

void LightManager::CalcLighting( Scene *pScene )
   {
 //  m_DeferredShader.VPreRender();
   // Only calculate shadow for nodes in static and actor group
   for( Lights::iterator lightIt = m_ActiveLights.begin(); lightIt != m_ActiveLights.end(); ++lightIt )
      {
      lightIt->get()->VPreRenderShadowMap();
      }

   if( pScene->m_pRoot ) 
      {
      //auto pStaticGroup = pScene->m_pRoot->m_Children[ RenderPass_Static ];
     // CalcShadow( pStaticGroup );
      auto pActorGroup = pScene->m_pRoot->m_Children[ RenderGroup_Actor ];
      auto& childrenList = pActorGroup->VGetChildrenSceneNodes();
      for( auto pChild : childrenList )
         {
         CalcShadow( pScene, pChild );
         }
    //  CalcShadow( pScene, pActorGroup );
      }

   int i = 0;
   for( Lights::iterator lightIt = m_ActiveLights.begin(); lightIt != m_ActiveLights.end(); ++lightIt, ++i )
      {
      /*if( lightIt == m_ActiveLights.begin() )
         {
         m_LightAmbient = lightIt->get()->GetLightPropertiesPtr()->m_Diffuse * 0.2f;
         }*/
      auto globalPos = lightIt->get()->VGetGlobalPosition();
      memcpy( &m_LightPosWorldSpace[ i ], &globalPos, sizeof( Vec3 ) );
      memcpy( &m_LightDir[ i ], &lightIt->get()->VGetGlobalTransformPtr()->GetForward(), sizeof( Vec3 ) );
      memcpy( &m_LightPower[ i ], &lightIt->get()->GetLightPropertiesPtr()->m_Power, sizeof( float ) );
      memcpy( &m_LightColor[ i ], &lightIt->get()->GetLightPropertiesPtr()->m_Color, sizeof( Color ) );
      // auto shadowMapMatrix = lightIt->get()->VGetShadowMapMatrix();
      //  memcpy( &m_ShadowMapMatrix[ i ], &shadowMapMatrix, sizeof( Mat4x4 ) );
    //  auto shadowMapTexture = lightIt->get()->VGetShadowMapTexture();
    //  memcpy( &m_ShadowMapTexture[ i ], &shadowMapTexture, sizeof( GLuint ) );
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


void LightManager::CalcShadow( Scene *pScene, shared_ptr< ISceneNode > pNode )
   {
  // m_DeferredShader.VOnRender( pScene, pNode );
   for( Lights::iterator lightIt = m_ActiveLights.begin(); lightIt != m_ActiveLights.end(); ++lightIt )
      {
      // If the node is inside the frustum of current light, then render it to current light's shadow map
      auto nodeGlobalPos = pNode->VGetGlobalTransformPtr()->GetToWorldPosition();
      if( pNode->VGetProperties().GetEnableShadow() && lightIt->get()->VIsInside( nodeGlobalPos, pNode->VGetProperties().GetRadius() ) )
         {
         //lightIt->get()->VSetUpRenderShadowMap();
        // lightIt->get()->VRenderShadowMap( pNode );
         }
      }
   auto& childrenList = pNode->VGetChildrenSceneNodes();
   for( auto pChild : childrenList )
      {
      CalcShadow( pScene, pChild );
      }
   }

void LightManager::RenderShadowMap( ISceneNode *pNode )
   {
 
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