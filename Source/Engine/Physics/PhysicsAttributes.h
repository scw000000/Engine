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
typedef btRigidBody RigidBody;
class IPhysicsAttributes
   {
   public:
      virtual ~IPhysicsAttributes( void ){ }
      virtual bool VInit( TiXmlElement* pData ) = 0;
      virtual bool VDelegateInit( TiXmlElement* pData ) = 0;
    //  virtual TiXmlElement* VGenerateXML( void ) = 0;
      virtual bool VGetIsActive( void ) const = 0;
      virtual Vec3 VGetTranslateFactor( void ) const = 0;
      virtual Vec3 VGetRotationFactor( void ) const = 0;
      virtual CollisionId VGetCollisionId( void ) const = 0;
      virtual std::string VGetShpae( void ) const = 0;
      virtual std::string VGetDensity( void ) const = 0;
      virtual std::string VGetMaterial( void ) const = 0;
      virtual bool VIsLinkedToPhysicsWorld( void ) const = 0;
      virtual void VSetIsLinkedToPhysicsWorld( bool isLinked ) = 0;
      virtual void VSetTransform( const Transform& transform ) = 0;
      virtual RigidBody* VGetBtRigidBody( void ) const = 0;
      virtual void VSetRigidBody( RigidBody* body ) = 0;
      virtual int VGetCollisionFlags( void ) const = 0;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp ) = 0;
      virtual TiXmlElement* VGenerateXML( void ) const = 0;
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const = 0;
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) const = 0;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) const = 0;
   };
