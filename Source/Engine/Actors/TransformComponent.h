#pragma once


#include "ActorComponent.h"



//---------------------------------------------------------------------------------------------------------------------
// This component implementation is a very simple representation of the physical aspect of an actor.  It just defines 
// the transform and doesn't register with the physics system at all.
//---------------------------------------------------------------------------------------------------------------------

class TransformComponent : public ActorComponent
   {
   
   public:
	   static const char* g_Name;
	   virtual const char* VGetName() const { return g_Name; }

      TransformComponent(void) : m_pTransform( ENG_NEW Transform( Transform::g_Identity ) ) { }
      virtual bool VInit(TiXmlElement* pData) override;
   //   virtual TiXmlElement* VGenerateXml(void) override;

   // transform functions
      TransformPtr GetTransform(void) const { return m_pTransform; }
      void SetTransform( const TransformPtr pNewTransform ) { m_pTransform = pNewTransform; }
      Vec3 GetToWorldPosition(void) const { return m_pTransform->GetPosition(); }
      void SetToWorldPosition(const Vec3& pos) { m_pTransform->SetPosition( pos ); }
      Vec3 GetForward(void) const { return m_pTransform->GetForward(); }
   
   private:
      TransformPtr m_pTransform;
      /*Transform m_Transform;*/
   };

