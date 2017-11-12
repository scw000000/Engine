#pragma once

/*!
 * \file BulletPhysicsAttributes.h
 * \date 2017/09/11 12:58
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * Some methods have been modified, remember to uncomment some of them
 *
 * \note
*/
#include "Physics.h"
#include "PhysicsAttributes.h"

// struct MaterialData;

class BulletPhysicsAttributes : public IPhysicsAttributes
   {
   friend class BulletPhysics;
      public:
      static shared_ptr< IPhysicsAttributes > GetInstanceFromShape( TiXmlElement* pData );
      BulletPhysicsAttributes( void );
      bool VInit( TiXmlElement* pData ) override;
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual bool VGetIsActive( void ) const override { return m_IsActive; }
      virtual Vec3 VGetTranslateFactor( void ) const override { return m_TransLateFactor; }
      virtual Vec3 VGetRotationFactor( void ) const override { return m_RotateFactor; }
      virtual CollisionId VGetCollisionId( void ) const override { return m_CollisionId; }
      virtual std::string VGetShpae( void ) const override { return m_Shape; }
      virtual float VGetDensity( void ) const override { return 0.f; /*return m_Density;*/ }
      virtual MaterialData VGetMaterial( void ) const override { return MaterialData(); /*return m_Material; */ }
      virtual bool VIsLinkedToPhysicsWorld( void ) const override { return m_IsLinkedToPhysicsWorld; }
      virtual void VSetIsLinkedToPhysicsWorld( bool isLinked ) override { m_IsLinkedToPhysicsWorld = isLinked; }
      virtual void VSetTransform( const Transform& transform ) override;
      virtual void VSetScale( const Vec3& scale ) {}
      virtual shared_ptr<RigidBody> VGetBtRigidBody( void ) const override { 
         return NULL;
        /* return m_pRigidBody; */
         }
      virtual void VSetRigidBody( shared_ptr<RigidBody> body ) override { /*m_pRigidBody = body;*/ };
      virtual int VGetCollisionFlags( void ) const override { return m_CollisionFlags; }
      virtual TiXmlElement* VGenerateXML( void ) const override;
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const override {};
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) const override;
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) const override {};

      protected:
      CollisionId m_CollisionId;
      float m_Acceleration;
      float m_MaxVelocity;

      float m_AngularAcceleration;
      float m_MaxAngularVelocity;

      std::string m_Shape;
      std::string m_Density;
      std::string m_Material;

      btRigidBody* m_pRigidBody;
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
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const override;
      virtual void VSetScale( const Vec3& scale ) override;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<IPhysicsAttributes> pPhysicsAttr ) override;
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
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const override;
      virtual void VSetScale( const Vec3& scale ) override;
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<IPhysicsAttributes> pPhysicsAttr ) override;
      void SetSize( const Vec3& size ) { m_Size = size; }

      private:
      Vec3 m_Size;
   };