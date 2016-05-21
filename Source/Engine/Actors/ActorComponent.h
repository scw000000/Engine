#pragma once
/*!
 * \file ActorComponent.h
 * \date 2016/05/01 10:03
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  TODO: implement a xml file parser for returning necessary xml data
 *  such as color, transform, etc....
 *
 * \note
 */

#include "../Utilities/String.h"

class TiXmlElement;

class IActorComponent
   {
   public:
      virtual const ComponentId& VGetComponentID( void ) const = 0;
      virtual const char *VGetName( ) const = 0;

      virtual bool VInit( TiXmlElement* pData ) = 0;
      virtual void VPostInit( void ) = 0;
      virtual void VUpdate( const unsigned long deltaMs ) = 0;

   };


// Using Curiously recurring template pattern (CRTP) to prevent declaring GUID mulit times
template <typename T>class BaseActorComponent : virtual public IActorComponent 
   {
   friend class ActorFactory;
   public:

      virtual const ComponentId& VGetComponentID( void ) const override { return s_ComponentID; };

      virtual const char* GetName( void ) const override { return s_pName; }

   public:
      // GUID of this event
      const static ComponentId  s_ComponentID;
      const static char*      s_pName;

   protected:
      void SetOwner( StrongActorPtr pOwner ) { m_pOwner = pOwner; }
   
   protected:
      StrongActorPtr m_pOwner;
   };

class ActorComponent
   {
   friend class ActorFactory;

   public:
      virtual ~ActorComponent( void ){}
      virtual bool VInit( TiXmlElement* pData ) = 0;
      virtual void VPostInit( void ){}
      virtual void VUpdate( const unsigned long deltaMs ){}

      virtual ComponentId VGetId( void ) const { return GetIdFromName( VGetName() ); }
      virtual const char *VGetName() const = 0;
      static ComponentId GetIdFromName( const char* componentName )
         {
         void* rawId = HashedString::hash_name( componentName );
         return reinterpret_cast<ComponentId> ( rawId );
         }
   protected:
      StrongActorPtr m_pOwner;
   private:
	   void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
   };