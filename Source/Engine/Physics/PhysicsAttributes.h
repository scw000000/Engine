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
class RigidBody;
//struct MaterialData;


struct MaterialData
   {
   float m_Restitution;
   float m_Friction;

   MaterialData( void ) :
      m_Restitution( 0.f )
      , m_Friction( 0.f )
      {}

   MaterialData( float restitution, float friction ) :
      m_Restitution( restitution )
      , m_Friction( friction )
      {}

   MaterialData( const MaterialData& other ) :
      m_Restitution( other.m_Restitution )
      , m_Friction( other.m_Friction )

      {
      /*m_Restitution = other.m_Restitution;
      m_Friction = other.m_Friction;*/
      }
   };

// typedef btRigidBody RigidBody;
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
      virtual float VGetDensity( void ) const = 0;
      virtual MaterialData VGetMaterial( void ) const = 0;
      virtual bool VIsLinkedToPhysicsWorld( void ) const = 0;
      virtual void VSetIsLinkedToPhysicsWorld( bool isLinked ) = 0;
      virtual void VSetTransform( const Transform& transform ) = 0;
      virtual shared_ptr< RigidBody > VGetBtRigidBody( void ) const = 0;
      virtual void VSetRigidBody( shared_ptr< RigidBody > body ) = 0;
      virtual int VGetCollisionFlags( void ) const = 0;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<IPhysicsAttributes> pPhysicsAttr ) = 0;
      virtual TiXmlElement* VGenerateXML( void ) const = 0;
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const = 0;
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) const = 0;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) const = 0;

     private:
   };
