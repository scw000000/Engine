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

//////////////
// INCLUDES //
//////////////
#include "../Utilities/String.h"

class TiXmlElement;

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