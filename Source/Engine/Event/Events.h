#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Events.h
////////////////////////////////////////////////////////////////////////////////
#include <strstream>
#include "FastDelegate.h"

class IEventData;

typedef unsigned long EventType;
typedef shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;

class IEventData
   {
   public:
      virtual ~IEventData(void) {}
      virtual const EventType& VGetEventType( void ) const = 0;
      virtual float VGetTimeStamp( void ) const = 0;
      // for Networking
      // virtual void VSerialize( std::ostrstream& out ) const  = 0;
      virtual IEventDataPtr VCopy( void ) const = 0;
      virtual const char* GetName( void ) const = 0;
   };

// Using Curiously recurring template pattern (CRTP) to prevent declaring GUID mulit times
template <typename T>class BaseEventData : public IEventData
   {
   public:
      explicit BaseEventData( const float timeStamp = 0.0f ) : m_TimeStamp( timeStamp ) {  };
      virtual ~BaseEventData( void ) { }

      virtual const EventType& VGetEventType( void ) const = 0;

      virtual float VGetTimeStamp( void ) const { return m_TimeStamp; }
   
   public:
       
   protected:
      // GUID of this event
      static EventType sk_EventType;

   private:
      
      const float m_TimeStamp;
   };

class EvtData_Destroy_Actor : public BaseEventData<EvtData_Destroy_Actor>
   {
   public:
      explicit EvtData_Destroy_Actor( ActorId id ) : m_Id( id ) {  }
      virtual const EventType& VGetEventType( void ) const { return sk_EventType; }
      virtual IEventDataPtr VCopy( void ) const { return IEventDataPtr( ENG_NEW EvtData_Destroy_Actor( m_Id ) ); }
      //virtual void VSerialize( std::ostre )
      virtual const char* GetName( void ) const { return "EvtDat_Destyot_Actor"; } 

   private:
      ActorId m_Id;
   };
