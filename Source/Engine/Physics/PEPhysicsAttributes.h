/*!
 * \file PEPhysicsAttributes.h
 * \date 2017/09/11 13:02
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

class ICollider;

class IPEColliderAttributes
   {
   public:
   virtual ~IPEColliderAttributes(void) {}
   virtual shared_ptr<ICollider> VCreateCollider( StrongRenderComponentPtr pRenderComp, shared_ptr< IPhysicsAttributes > pPhtsicsAttr ) = 0;
   ColliderTypes m_ColliderType;
   };

class PESphereColliderAttributes : public IPEColliderAttributes
   {
      public:
      virtual shared_ptr<ICollider> VCreateCollider( StrongRenderComponentPtr pRenderComp, shared_ptr< IPhysicsAttributes > pPhtsicsAttr ) override;
      float m_Radius;
   };

class PEBoxColliderAttributes : public IPEColliderAttributes
   {
      public:
      virtual shared_ptr<ICollider> VCreateCollider( StrongRenderComponentPtr pRenderComp, shared_ptr< IPhysicsAttributes > pPhtsicsAttr ) override;
      Vec3 m_Dimension;
   };

class PEPhysicsAttributes : public IPhysicsAttributes
   {
   public:
      static shared_ptr< IPhysicsAttributes > GetInstanceFromShape( TiXmlElement* pData );
      PEPhysicsAttributes( void );
      bool VInit( TiXmlElement* pData ) override;
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual bool VGetIsActive( void ) const override { return m_IsActive; }
      virtual Vec3 VGetTranslateFactor( void ) const override { return m_TransLateFactor; }
      virtual Vec3 VGetRotationFactor( void ) const override { return m_RotateFactor; }
      virtual CollisionId VGetCollisionId( void ) const override { return m_CollisionId; }
      virtual std::string VGetShpae( void ) const override { return "";/*return m_Shape*/; }
      virtual float VGetDensity( void ) const override { return m_Density; }
      virtual MaterialData VGetMaterial( void ) const override { return m_Material; }
      virtual bool VIsLinkedToPhysicsWorld( void ) const override { return m_IsLinkedToPhysicsWorld; }
      virtual void VSetIsLinkedToPhysicsWorld( bool isLinked ) override { m_IsLinkedToPhysicsWorld = isLinked; }
      virtual void VSetTransform( const Transform& transform ) override;
      virtual void VSetScale( const Vec3& scale ) {}
      virtual shared_ptr<RigidBody> VGetBtRigidBody( void ) const override { return m_pRigidBody; }
      virtual void VSetRigidBody( shared_ptr<RigidBody> pBody ) override { m_pRigidBody = pBody; };
      virtual void VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<IPhysicsAttributes> pPhysicsAttr ) override;
      virtual int VGetCollisionFlags( void ) const override { return m_CollisionFlags; }
      virtual TiXmlElement* VGenerateXML( void ) const override;
      virtual void VDelegateGenerateXML( TiXmlElement* pParent ) const override {};
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResourceNode ) const override { return NULL; };
      virtual void VDelegateGenerateOverridesXML( TiXmlElement* pBaseElement, TiXmlElement* pResourceNode ) const override {};

      protected:
      CollisionId m_CollisionId;
      float m_Acceleration;
      float m_MaxVelocity;

      float m_AngularAcceleration;
      float m_MaxAngularVelocity;

      std::vector< shared_ptr<IPEColliderAttributes> > m_ColliderAttributes;
      shared_ptr<RigidBody> m_pRigidBody;
      bool m_IsLinkedToPhysicsWorld;
      int m_CollisionFlags;
      bool m_IsActive;
      Vec3 m_TransLateFactor;
      Vec3 m_RotateFactor;
      float m_Density;
      MaterialData m_Material;
      bool m_PositionLock[ 3 ];
      bool m_RotationLock[ 3 ];
      float m_GravityScale;
   };
