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


const char* TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::VInit(TiXmlElement* pData)
   {
   ENG_ASSERT(pData);

	// [mrmike] - this was changed post-press - because changes to the TransformComponents can come in partial definitions,
	//            such as from the editor, its better to grab the current values rather than clear them out.
    
	Vec3 pitchYawRoll = m_pTransform->GetPitchYawRollRad();

	Vec3 position = m_pTransform->GetPosition();	

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

	Mat4x4 translation;
	translation.AddTranslation( position );

	Quaternion rotation;
	rotation.BuildPitchYawRollDeg( pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z );

	/**
	// This is not supported yet.
    TiXmlElement* pLookAtElement = pData->FirstChildElement("LookAt");
    if (pLookAtElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pLookAtElement->Attribute("x", &x);
        pLookAtElement->Attribute("y", &y);
        pLookAtElement->Attribute("z", &z);

		Vec3 lookAt((float)x, (float)y, (float)z);
		rotation.BuildRotationLookAt(translation.GetPosition(), lookAt, g_Up);
    }
    */
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
	

    //m_Transform = rotation * translation;
    m_pTransform->SetRotation( rotation );
    m_pTransform->SetPosition( position );
    m_pTransform->SetScale( scale );
    return true;
}