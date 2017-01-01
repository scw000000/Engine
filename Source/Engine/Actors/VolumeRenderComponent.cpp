/*!
 * \file VolumeRenderComponent.cpp
 * \date 2016/10/12 22:28
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "VolumeRenderComponent.h"
#include "..\Graphics\SceneNode\VolumeRenderSceneNode.h"

const ComponentRegisterId BaseActorComponent< VolumeRenderComponent >::s_ComponentId = 0x17c79c8f;
const std::string BaseActorComponent< VolumeRenderComponent >::s_Name = "VolumeRenderComponent";

VolumeRenderComponent::VolumeRenderComponent( void ) : m_pVolumeTextureResource( ENG_NEW Resource( "" ) ), m_pTransferFuncionResource( ENG_NEW Resource( "" ) )
   {
   
   }

shared_ptr<SceneNode> VolumeRenderComponent::VCreateSceneNode( void ) 
   {
   return shared_ptr< SceneNode >( ENG_NEW VolumeRenderSceneNode( m_pOwner->GetId(), this, RenderGroup::RenderGroup_Actor, m_pTransform, m_pVolumeTextureResource, m_pTransferFuncionResource, m_TextureDimension, m_CuboidDimension ) );
   }

bool VolumeRenderComponent::VDelegateInit( TiXmlElement* pData ) 
   {
   auto pVolumeTextureRes = pData->FirstChildElement( "VolumeTexture" );
   if( pVolumeTextureRes )
      {
      m_pVolumeTextureResource->Init( pVolumeTextureRes );
      auto pDimensionElement = pVolumeTextureRes->FirstChildElement( "TextureDimension" );
      if( pDimensionElement )
         {
         m_TextureDimension.Init( pDimensionElement );
         }

      pDimensionElement = pVolumeTextureRes->FirstChildElement( "CuboidDimension" );
      if( pDimensionElement )
         {
         m_CuboidDimension.Init( pDimensionElement );
         }
      }

   auto pTransferFuncionTextureRes = pData->FirstChildElement( "TransferFunction" );
   if( pTransferFuncionTextureRes )
      {
      m_pTransferFuncionResource->Init( pTransferFuncionTextureRes );
      }
   
   return true;
   }

void VolumeRenderComponent::VDelegatePostInit( void ) 
   {
   


   }

void VolumeRenderComponent::VDelegateGenerateXML( TiXmlElement* pBaseElement ) 
   {
   auto pVolumeTexture = m_pVolumeTextureResource->GenerateXML();
   pVolumeTexture->SetValue( "VolumeTexture" );

   auto pTextureDimension = m_TextureDimension.GernerateXML();
   pTextureDimension->SetValue( "TextureDimension" );
   pVolumeTexture->LinkEndChild( pTextureDimension );

   auto pCuboidDimension = m_CuboidDimension.GernerateXML();
   pCuboidDimension->SetValue( "CuboidDimension" );
   pVolumeTexture->LinkEndChild( pCuboidDimension );

   pBaseElement->LinkEndChild( pVolumeTexture );

   auto pTransferFunction = m_pTransferFuncionResource->GenerateXML();
   pTransferFunction->SetValue( "TransferFunction" );
   pBaseElement->LinkEndChild( pVolumeTexture );
   }