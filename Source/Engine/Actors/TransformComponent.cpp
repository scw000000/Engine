#pragma once
/*!
 * \file TransformComponent.cpp
 * \date 2016/05/02 9:51
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
#include "TransformComponent.h"


//const char* TransformComponent::g_Name = "TransformComponent";

const ComponentId BaseActorComponent<TransformComponent>::s_ComponentId = 0x78e9ef25;
const std::string BaseActorComponent<TransformComponent>::s_Name = "TransformComponent";

void TransformComponent::Destory( void )
   {
   m_pTransform.reset();
   }

bool TransformComponent::VInit( TiXmlElement* pData )
   {
   ENG_ASSERT( pData );

	// [mrmike] - this was changed post-press - because changes to the TransformComponents can come in partial definitions,
	//            such as from the editor, its better to grab the current values rather than clear them out.

   Transform transform = *m_pTransform;
    
/*
	Vec3 pitchYawRoll = m_pTransform->GetPitchYawRollRad();

   Vec3 position = m_pTransform->GetToWorldPosition();

   Vec3 scale = m_pTransform->GetScale();

   TiXmlElement* pPositionElement = pData->FirstChildElement("Position");
   if (pPositionElement)
     {
     double x = 0;
     double y = 0;
     double z = 0;
     pPositionElement->Attribute("x", &x);
     pPositionElement->Attribute("y", &y);
     pPositionElement->Attribute("z", &z);
     position = Vec3(x, y, z);
     }

   TiXmlElement* pOrientationElement = pData->FirstChildElement("PitchYawRoll");
   if (pOrientationElement)
     {
     double yaw = 0;
     double pitch = 0;
     double roll = 0;
     pOrientationElement->Attribute("x", &pitch);
     pOrientationElement->Attribute("y", &yaw);
     pOrientationElement->Attribute("z", &roll);
	  pitchYawRoll = Vec3(pitch, yaw, roll);
	  }

	Quaternion rotation;
	rotation.BuildPitchYawRollDeg( pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z );

    TiXmlElement* pScaleElement = pData->FirstChildElement( "Scale" );
   if (pScaleElement)
      {
      double x = 0;
      double y = 0;
      double z = 0;
      pScaleElement->Attribute( "x", &x );
      pScaleElement->Attribute( "y", &y );
      pScaleElement->Attribute( "z", &z );
      scale = Vec3( (float)x, (float)y, (float)z );
      }
	
    m_pTransform->SetRotation( rotation );
    m_pTransform->SetPosition( position );
    m_pTransform->SetScale( scale );*/
    return true;
}

TiXmlElement* TransformComponent::VGenerateXML( void )
   {
   TiXmlElement* pRoot = m_pTransform->GenerateXML();
   pRoot->SetValue( s_Name.c_str() );

   //// initial transform -> position
   //TiXmlElement* pPosition = ENG_NEW TiXmlElement( "Position" );
   //Vec3 pos( m_pTransform->GetToWorldPosition() );
   //pPosition->SetAttribute( "x", ToStr( pos.x ).c_str() );
   //pPosition->SetAttribute( "y", ToStr( pos.y ).c_str() );
   //pPosition->SetAttribute( "z", ToStr( pos.z ).c_str() );
   //pBaseElement->LinkEndChild( pPosition );

   //// initial transform -> LookAt
   //TiXmlElement* pDirection = ENG_NEW TiXmlElement( "PitchYawRoll" );
   //Vec3 orient( m_pTransform->GetPitchYawRollDeg() );
   //pDirection->SetAttribute( "x", ToStr( orient.x ).c_str() );
   //pDirection->SetAttribute( "y", ToStr( orient.y ).c_str() );
   //pDirection->SetAttribute( "z", ToStr( orient.z ).c_str() );
   //pBaseElement->LinkEndChild( pDirection );

   //// This is not supported yet
   //// initial transform -> position
   //TiXmlElement* pScale = ENG_NEW TiXmlElement( "Scale" );
   //Vec3 scale( m_pTransform->GetScale() );
   //pPosition->SetAttribute( "x", ToStr( scale.x ).c_str() );
   //pPosition->SetAttribute( "y", ToStr( scale.y ).c_str() );
   //pPosition->SetAttribute( "z", ToStr( scale.z ).c_str() );
   //pBaseElement->LinkEndChild( pScale );

   return pRoot;
   }