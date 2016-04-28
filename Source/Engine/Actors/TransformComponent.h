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

      TransformComponent(void) : m_Transform( Transform::g_Identity ) { }
      virtual bool VInit(TiXmlElement* pData) override;
   //   virtual TiXmlElement* VGenerateXml(void) override;

   // transform functions
      Transform GetTransform(void) const { return m_Transform; }
      void SetTransform(const Transform& newTransform) { m_Transform = newTransform; }
      Vec3 GetToWorldPosition(void) const { return m_Transform.GetPosition(); }
      void SetToWorldPosition(const Vec3& pos) { m_Transform.SetPosition( pos ); }
      Vec3 GetForward(void) const { return m_Transform.GetForward(); }
   
   private:
      Transform m_Transform;
   };

