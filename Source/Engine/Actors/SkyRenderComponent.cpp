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
#include "TransformComponent.h"
#include "..\Graphics\SkySceneNode.h"

const char* SkyRenderComponent::g_Name = "SkyRenderComponent";

void SkyRenderComponent::Destory( void )
   {
   BaseRenderComponent::Destory();
   m_pTextureResource.reset();
   m_pMeshResource.reset();
   }

SkyRenderComponent::SkyRenderComponent( void )
   {
   }

// This function is calle by BaseRenderComponent
bool SkyRenderComponent::VDelegateInit( TiXmlElement* pData )
   {
   TiXmlElement* pMeshFileElement = pData->FirstChildElement( "Mesh" );
   // Set mesh file path
   if( pMeshFileElement )
      {
      const char *pMeshFilePath = pMeshFileElement->Attribute( "path" );
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
   TiXmlElement* pTextureFileElement = pData->FirstChildElement( "Texture" );
   if( pTextureFileElement )
      {
      const char *pTextureFilePath = pTextureFileElement->Attribute( "path" );
      if( !pTextureFilePath )
         {
         return false;
         }
      else
         {
        m_pTextureResource = shared_ptr<Resource>( ENG_NEW Resource( pTextureFilePath ) );
         }
      }
   else
      {
      return false;
      }
   return true;
   }

shared_ptr<SceneNode> SkyRenderComponent::VCreateSceneNode( void )
   {
   shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr( m_pOwner->GetComponent<TransformComponent>( TransformComponent::g_Name ) );
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
      }
   return shared_ptr<SceneNode>( );
   }
