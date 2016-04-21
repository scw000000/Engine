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
      // This function returns eventType when you don't know the event instance ID
      // IF you want to know the instance ID of a specific event type, access the const static
      // menber variable in BaseEventData instead
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

      virtual const EventType& VGetEventType( void ) const override { return sk_EventType; };

      virtual float VGetTimeStamp( void ) const { return m_TimeStamp; }
   
   public:
       // GUID of this event
      const static EventType sk_EventType;

   protected:
      

   private:
      
      const float m_TimeStamp;
   };

class EvtData_Destroy_Actor : public BaseEventData<EvtData_Destroy_Actor>
   {
   public:
      explicit EvtData_Destroy_Actor( ActorId id ) : m_Id( id ) {  }
      virtual IEventDataPtr VCopy( void ) const { return IEventDataPtr( ENG_NEW EvtData_Destroy_Actor( m_Id ) ); }
      //virtual void VSerialize( std::ostre )
      virtual const char* GetName( void ) const { return "EvtDat_Destyot_Actor"; } 

   private:
      ActorId m_Id;
   };


class EvtData_New_Render_Component : public BaseEventData<EvtData_New_Render_Component>
   {

   public:
      EvtData_New_Render_Component( void ) { m_ActorId = INVALID_ACTOR_ID; }

      explicit EvtData_New_Render_Component(ActorId actorId, shared_ptr<SceneNode> pSceneNode) : m_ActorId(actorId), m_pSceneNode(pSceneNode) { }

      virtual IEventDataPtr VCopy(void) const override { return IEventDataPtr( ENG_NEW EvtData_New_Render_Component( m_ActorId, m_pSceneNode ) ); }

      virtual const char* GetName(void) const override { return "EvtData_New_Render_Component"; }

      const ActorId GetActorId(void) const { return m_ActorId; }

      shared_ptr<SceneNode> GetSceneNode(void) const { return m_pSceneNode; }

   private:
      ActorId m_ActorId;
      
      shared_ptr<SceneNode> m_pSceneNode;
   
   };