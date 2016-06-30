#pragma once
/*!
 * \file PhysicsAttributes.h
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

#include "CollisionTable.h"
#include "..\Actors\RenderComponent.h"

class btRigidBody;

class IPhysicsAttributes
   {
   public:
      virtual ~IPhysicsAttributes( void ){ }
      virtual bool Vinit( TiXmlElement* pData ) = 0;
    //  virtual TiXmlElement* VGenerateXML( void ) = 0;
      virtual CollisionId VGetCollisionId( void ) const = 0;
      virtual std::string VGetShpae( void ) const = 0;
      virtual std::string VGetDensity( void ) const = 0;
      virtual std::string VGetMaterial( void ) const = 0;
   };


class BulletPhysicsAttributes : public IPhysicsAttributes
   {
   friend class BulletPhysics;
   public:
      BulletPhysicsAttributes( void );
      bool Vinit( TiXmlElement* pData ) override;

      virtual CollisionId VGetCollisionId( void ) const override { return m_CollisionId; }
      virtual std::string VGetShpae( void ) const override { return m_Shape; }
      virtual std::string VGetDensity( void ) const override { return m_Density; }
      virtual std::string VGetMaterial( void ) const override { return m_Material; }

   private:
      CollisionId m_CollisionId;
      float m_Acceleration;
      float m_MaxVelocity;

      float m_AngularAcceleration;
      float m_MaxAngularVelocity;

      std::string m_Shape;
      std::string m_Density;
      std::string m_Material;

      btRigidBody* m_pRigidBody;
   };