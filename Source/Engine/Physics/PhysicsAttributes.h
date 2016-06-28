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

class IPhysicsAttributes
   {
   public:
      virtual ~IPhysicsAttributes( void ){ }
      virtual bool Vinit( TiXmlElement* pData );
      virtual TiXmlElement* VGenerateXML( void );
   };


class BulletPhysicsAttr : public IPhysicsAttributes
   {
   public:
      BulletPhysicsAttr( void );

   private:
      CollisionId m_CollisionId;
      float m_Acceleration;
      float m_MaxVelocity;

      float m_AngularAcceleration;
      float m_MaxAngularVelocity;

      std::string m_Density;
      std::string m_Material;

      weak_ptr< IRenderComponent > m_pOwner;
   };