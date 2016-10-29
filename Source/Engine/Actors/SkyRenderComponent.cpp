/*!
 * \file SkyRenderComponent.cpp
 * \date 2016/05/03 16:53
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
#include "SkyRenderComponent.h"
#include "..\Graphics\SceneNode\SkySceneNode.h"

//const char* SkyRenderComponent::g_Name = "SkyRenderComponent";

const ComponentRegisterId BaseActorComponent<SkyRenderComponent>::s_ComponentId = 0x249dbe49;
const std::string BaseActorComponent<SkyRenderComponent>::s_Name = "SkyRenderComponent";


void SkyRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pTextureResource.reset();
   m_pMeshResource.reset();
   }

SkyRenderComponent::SkyRenderComponent( void ) : m_pMeshResource( ENG_NEW Resource( "" ) ), m_pTextureResource( ENG_NEW Resource( "" ) )
   {
   }

// This function is calle by BaseRenderComponent
bool SkyRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   // Set mesh file path
   if( !pData ) 
      {
      return false;
      }
   m_pMeshResource->Init( pData->FirstChildElement( "Mesh" ) );

   m_pTextureResource->Init( pData->FirstChildElement( "Texture" ) );
   return true;
   }

shared_ptr<SceneNode> SkyRenderComponent::VCreateSceneNode( void )
   {
   switch( EngineApp::GetRendererImpl() )
      {
      case Renderer::Renderer_OpenGL:
            {
            return shared_ptr<SceneNode>( ENG_NEW SkySceneNode( m_pOwner->GetId(), 
                                                                this, 
                                                                m_pMeshResource, 
                                                                m_pTextureResource, 
                                                                RenderPass::RenderPass_Sky, 
                                                                m_pTransform ) );
            }
      default:
         ENG_ASSERT( 0 && "Unknown Renderer Implementation in GridRenderComponent" );
      }

   /*shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::s_ComponentId ) );
   if( pTransformComponent )
      {
      WeakBaseRenderComponentPtr weakThis( this );

      switch( EngineApp::GetRendererImpl( ) )
         {
            case Renderer::Renderer_OpenGL:
               {
               return shared_ptr<SceneNode>( ENG_NEW SkySceneNode( m_pOwner->GetId( ), weakThis, m_pMeshResource, m_pTextureResource, RenderPass::RenderPass_Sky, pTransformComponent->GetTransform( ) ) );
               }
            default:
            ENG_ASSERT( 0 && "Unknown Renderer Implementation in GridRenderComponent" );
         }
      }*/
   return shared_ptr<SceneNode>( );
   }

void SkyRenderComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement )
   {
   TiXmlElement* pMesh = m_pMeshResource->GenerateXML();
   pMesh->SetValue( "Mesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pTexture = m_pTextureResource->GenerateXML();
   pTexture->SetValue( "Texture" );
   pBaseElement->LinkEndChild( pTexture );
   }

void SkyRenderComponent::VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode )
   {
   TiXmlElement* pMesh = m_pMeshResource->GenerateOverridesXML( pResourceNode->FirstChildElement( "Mesh" ) );
   pMesh->SetValue( "Mesh" );
   pBaseElement->LinkEndChild( pMesh );

   TiXmlElement* pTextrue = m_pTextureResource->GenerateOverridesXML( pResourceNode->FirstChildElement( "Texture" ) );
   pTextrue->SetValue( "Texture" );
   pBaseElement->LinkEndChild( pTextrue );
   }
