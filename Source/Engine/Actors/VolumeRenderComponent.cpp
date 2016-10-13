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
#include "..\Graphics\VolumeRenderSceneNode.h"

const ComponentRegisterId BaseActorComponent< VolumeRenderComponent >::s_ComponentId = 0x17c79c8f;
const std::string BaseActorComponent< VolumeRenderComponent >::s_Name = "VolumeRenderComponent";

VolumeRenderComponent::VolumeRenderComponent( void ) : m_pVolumeTextureResource( ENG_NEW Resource( "" ) ), m_pTransferFuncionResource( ENG_NEW Resource( "" ) )
   {
   
   }

shared_ptr<SceneNode> VolumeRenderComponent::VCreateSceneNode( void ) 
   {
   return shared_ptr< SceneNode >( ENG_NEW VolumeRenderSceneNode( m_pOwner->GetId(), this, m_pVolumeTextureResource, m_VolumeDimension, m_pTransferFuncionResource, RenderPass::RenderPass_Actor, m_pTransform ) );
   }

bool VolumeRenderComponent::VDelegateInit( TiXmlElement* pData ) 
   {
   auto pVolumeTextureRes = pData->FirstChildElement( "VolumeTexture" );
   if( pVolumeTextureRes )
      {
      m_pVolumeTextureResource->Init( pVolumeTextureRes );
      auto pDimensionElement = pVolumeTextureRes->FirstChildElement( "Dimension" );
      if( pDimensionElement )
         {
         m_VolumeDimension.Init( pDimensionElement );
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

   auto pVolumeDimension = m_VolumeDimension.GernerateXML();
   pVolumeDimension->SetValue( "Dimension" );
   pVolumeTexture->LinkEndChild( pVolumeDimension );
   pBaseElement->LinkEndChild( pVolumeTexture );

   auto pTransferFunction = m_pTransferFuncionResource->GenerateXML();
   pTransferFunction->SetValue( "TransferFunction" );
   pBaseElement->LinkEndChild( pVolumeTexture );
   }