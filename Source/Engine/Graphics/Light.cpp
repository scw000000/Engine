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

LightProperties::LightProperties( void )
   {
   m_Type = LIGHT_TYPE_UNDEFINED;
   m_Enabled = 1u;
   }

bool LightProperties::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   Color color;
   if( !color.Init( pData->FirstChildElement( "Color" ) ) )
      {
      return false;
      }
   m_Color.x = color.m_Component.r;
   m_Color.y = color.m_Component.g;
   m_Color.z = color.m_Component.b;

   std::string lightType = pData->Attribute( "type" );

   if( !lightType.compare( "point" ) )
      {
      m_Type = LIGHT_TYPE_POINT;
      }
   else if( !lightType.compare( "directional" ) )
      {
      m_Type = LIGHT_TYPE_DIRECTIONAL;
      }
   else
      {
      m_Type = LIGHT_TYPE_UNDEFINED;
      return false;
      }

   // Reference: http://www.ogre3d.org/tikiwiki/-Point+Light+Attenuation
   if( !m_Attenuation.Init( pData->FirstChildElement( "Attenuation" ) ) )
      {
      return false;
      }
   /*if( TIXML_SUCCESS != pData->QueryFloatAttribute( "power", &m_Power ) )
      {
      return false;
      }*/

   return true;
   }

TiXmlElement* LightProperties::GenerateXML( void )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Light" );

   Color color( m_Color.x, m_Color.y, m_Color.z, 1.0f );
   TiXmlElement* pColor = color.GenerateXML();
   pColor->SetValue( "Color" );
   pRetNode->LinkEndChild( pColor );

   std::string typeStr;
   switch( m_Type ) 
      {
      case LIGHT_TYPE_DIRECTIONAL:
         typeStr = "directional";
         break;
      case LIGHT_TYPE_POINT :
         typeStr = "point";
         break;
      default :
         break;
      };

   pRetNode->SetAttribute( "type", typeStr.c_str() );

   TiXmlElement* pAttenuation = m_Attenuation.GernerateXML();
   pAttenuation->SetValue( "Attenuation" );
   pRetNode->LinkEndChild( pAttenuation );
 //  pRetNode->SetAttribute( "power", ToStr( m_Power ).c_str() );

   return pRetNode;
   }

//TiXmlElement* LightProperties::GenerateOverridesXML( TiXmlElement* pResource ) 
//   {
//   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Light" );
//
//   TiXmlElement* pDiffuse = m_Color.GenerateOverridesXML( pResource->FirstChildElement( "Diffuse" ) );
//   pDiffuse->SetValue( "Diffuse" );
//   pRetNode->LinkEndChild( pDiffuse );
//
//   /*pRetNode->SetAttribute( "power", ToStr( m_Power ).c_str() );
//   if( !strcmp( pRetNode->Attribute( "power" ), pResource->Attribute( "power" ) ) )
//      {
//      pRetNode->RemoveAttribute( "power" );
//      }*/
//   return pRetNode;
//   }

ILightNode::ILightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& props, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, RenderGroup_NotRendered, pTransform )
   , m_pLightProps( props )
   {
   }

LightManager::LightManager( void )
   {
   IEventManager* pEventMgr = IEventManager::GetSingleton();
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::NewSceneNodeDelegate ), Event_New_Scene_Node::s_EventType );
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &LightManager::DestroySceneNodeDelegate ), Event_Destroy_Scene_Node::s_EventType );
   }

void LightManager::RenderShadowMap( shared_ptr< ILightNode > ) const
   {
   
   }

void LightManager::CalcLighting( Scene *pScene )
   {
   // Only calculate shadow for nodes in static and actor group
   auto viewMat = pScene->GetCamera()->GetView();
   for( Lights::iterator lightIt = m_ActiveLights.begin(); lightIt != m_ActiveLights.end(); ++lightIt )
      {
      auto lightNode = lightIt->get();
      auto lightPtr = lightIt->get()->GetLightPropertiesPtr();
      switch( lightPtr->m_Type )
         {
         case LIGHT_TYPE_POINT :
            lightPtr->m_PositionVS = viewMat.Xform( lightNode->VGetGlobalPosition(), 1.0f );
            break;
         case LIGHT_TYPE_DIRECTIONAL :
            lightPtr->m_DirectionVS = viewMat.Xform( lightNode->VGetGlobalTransformPtr()->GetForward(), 0.0f );
            lightPtr->m_DirectionVS.Normalize();
            break;
         };

     // lightIt->get()->VPreRenderShadowMap();
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
   //   memcpy( &m_LightPower[ i ], &lightIt->get()->GetLightPropertiesPtr()->m_Power, sizeof( float ) );
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

bool LightManager::AddLightNode( shared_ptr<ILightNode> pNewLight )
   {
 
   m_Lights.push_back( pNewLight );
   if( m_ActiveLights.size() + 1 < MAXIMUM_LIGHTS_SUPPORTED )
      {
      m_ActiveLights.push_back( pNewLight );
      }
   return true;
   }

bool LightManager::RemoveLightNode( shared_ptr<ILightNode> pRemovedLight )
   {
   m_ActiveLights.remove( pRemovedLight );
   m_Lights.remove( pRemovedLight );
   return true;
   }

void LightManager::NewSceneNodeDelegate( IEventPtr pEvent )
   {
   shared_ptr< Event_New_Scene_Node > pNewSceneNodeEvt = dynamic_pointer_cast< Event_New_Scene_Node >( pEvent );
   shared_ptr< ILightNode > pLightNode = dynamic_pointer_cast< ILightNode >( pNewSceneNodeEvt->GetSceneNode().lock() );
   if( pLightNode )
      {
      AddLightNode( pLightNode );
      }
   }

void LightManager::DestroySceneNodeDelegate( IEventPtr pEvent )
   {
   shared_ptr< Event_Destroy_Scene_Node > pDerivedEvt = dynamic_pointer_cast< Event_Destroy_Scene_Node >( pEvent );
   shared_ptr< ILightNode > pLightNode = dynamic_pointer_cast< ILightNode >( pDerivedEvt->GetSceneNodePtr().lock() );
   if( pLightNode )
      {
      RemoveLightNode( pLightNode );
      }
   }