#pragma once
/*!
 * \file TransformComponent.h
 * \date 2016/05/21 21:52
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

#include "ActorComponent.h"



//---------------------------------------------------------------------------------------------------------------------
// This component implementation is a very simple representation of the physical aspect of an actor.  It just defines 
// the transform and doesn't register with the physics system at all.
//---------------------------------------------------------------------------------------------------------------------

class TransformComponent : public BaseActorComponent<TransformComponent>
   {
   public:
      virtual void Destory( void ) override;
      TransformComponent( void ) : m_pTransform( ENG_NEW Transform( Transform::g_Identity ) ) { }
      virtual bool VInit( TiXmlElement* pData ) override;
      virtual TiXmlElement* VGenerateXML( void ) override;
      // transform functions
      TransformPtr GetTransform( void ) const { return m_pTransform; }
      void SetTransformPtr( const TransformPtr pNewTransform ) { m_pTransform = pNewTransform; }
      void SetTransform( const Transform& newTransform ) { *m_pTransform = newTransform; }
      
      Vec3 GetToWorldPosition( void ) const { return m_pTransform->GetToWorldPosition(); }
      void SetToWorldPosition(const Vec3& pos) { m_pTransform->SetPosition( pos ); }
      Vec3 GetForward( void ) const { return m_pTransform->GetForward(); }
   
   private:
      TransformPtr m_pTransform;
   };

