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
      virtual bool Vinit( TiXmlElement* pData ) = 0;
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

   };


class BulletPhysicsAttributes : public IPhysicsAttributes
   {
   friend class BulletPhysics;
   public:
      static shared_ptr< IPhysicsAttributes > GetInstanceFromShape( const std::string& shape );
      BulletPhysicsAttributes( void );
      bool Vinit( TiXmlElement* pData ) override;
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual bool VGetIsActive( void ) const override { return m_IsActive; }
      virtual Vec3 VGetTranslateFactor( void ) const override { return m_TransLateFactor; }
      virtual Vec3 VGetRotationFactor( void ) const override { return m_RotateFactor; }
      virtual CollisionId VGetCollisionId( void ) const override { return m_CollisionId; }
      virtual std::string VGetShpae( void ) const override { return m_Shape; }
      virtual std::string VGetDensity( void ) const override { return m_Density; }
      virtual std::string VGetMaterial( void ) const override { return m_Material; }
      virtual bool VIsLinkedToPhysicsWorld( void ) const override { return m_IsLinkedToPhysicsWorld; }
      virtual void VSetIsLinkedToPhysicsWorld( bool isLinked ) override { m_IsLinkedToPhysicsWorld = isLinked; }
      virtual void VSetTransform( const Transform& transform ) override;
      virtual void VSetScale( const Vec3& scale ) {  }
      virtual RigidBody* VGetBtRigidBody( void ) const override { return m_pRigidBody; }
      virtual void VSetRigidBody( RigidBody* body ) override { m_pRigidBody = body; };
      virtual int VGetCollisionFlags( void ) const override { return m_CollisionFlags; }
      virtual TiXmlElement* VGenerateXML( void ) const override;
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const override { };

   protected:
      CollisionId m_CollisionId;
      float m_Acceleration;
      float m_MaxVelocity;

      float m_AngularAcceleration;
      float m_MaxAngularVelocity;

      std::string m_Shape;
      std::string m_Density;
      std::string m_Material;

      RigidBody* m_pRigidBody;
      bool m_IsLinkedToPhysicsWorld;
      int m_CollisionFlags;
      bool m_IsActive;
      Vec3 m_TransLateFactor;
      Vec3 m_RotateFactor;
   };

class BulletSpherePhysicsAttributes : public BulletPhysicsAttributes
   {
   friend class BulletPhysics;
   public: 
      BulletSpherePhysicsAttributes( void );
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual void VSetScale( const Vec3& scale ) override;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp ) override;
      void SetRadius( float radius ) { m_Radius = radius; }
   private:
      float m_Radius;
   };

class BulletBoxPhysicsAttributes : public BulletPhysicsAttributes
   {
   friend class BulletPhysics;
   public:
      BulletBoxPhysicsAttributes( void );
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual void VSetScale( const Vec3& scale ) override;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp ) override;
      void SetSize( const Vec3& size ) { m_Size = size; }

   private:
      Vec3 m_Size;
   };