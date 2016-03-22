#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ActorComponent.h
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "../Utilities/String.h"

class TiXmlElement;

class ActorComponent
   {
   friend class ActorFactory;

   public:
      StrongActorPtr m_pOwner;

   protected:
      virtual ~ActorComponent( void ){}
      virtual bool VInit( TiXmlElement* pData ) = 0;
      virtual void VPostInit( void ){}
      virtual void VUpdate( int deltaMs ){}

      virtual ComponentId VGetId( void ) const { return GetIdFromName( VGetName() ) }
      virtual const char *VGetName() const = 0;
      static ComponentId GetIdFromName( const char* componentName )
         {
         void* rawId = HashedString::hash_name( componentName );
         return reinterpret_cast<ComponentId> ( rawId );
         }

   private:
	   void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
   };