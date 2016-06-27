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
   };