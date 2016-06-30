/*!
 * \file PhysicsAttributes.cpp
 * \date 2016/06/27 21:46
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
#include "PhysicsAttributes.h"
#include "btBulletCollisionCommon.h"

BulletPhysicsAttributes::BulletPhysicsAttributes( void )
   {
   m_CollisionId = 0;
   m_Acceleration = 0.f;
   m_MaxVelocity = 0.f;

   m_AngularAcceleration = 0;
   m_MaxAngularVelocity = 0;

   m_Shape = "PointCloud";
   m_Density = "empty";
   m_Material = "None";

   m_pRigidBody = NULL;
   }

bool BulletPhysicsAttributes::Vinit( TiXmlElement* pData )
   {
   // shape
   TiXmlElement* pShape = pData->FirstChildElement( "Shape" );
   if( pShape )
      {
      m_Shape = pShape->FirstChild()->Value();
      }

   // density
   TiXmlElement* pDensity = pData->FirstChildElement( "Density" );
   if( pDensity )
      m_Density = pDensity->FirstChild()->Value();

   // material
   TiXmlElement* pMaterial = pData->FirstChildElement( "PhysicsMaterial" );
   if( pMaterial )
      m_Material = pMaterial->FirstChild()->Value();

   return true;
   }
