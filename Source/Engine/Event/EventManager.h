#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: EventManager.h
////////////////////////////////////////////////////////////////////////////////
#include "Events.h"


class IEventManager
   {
   public:
      const static unsigned long kINFINITE = 0xffffffff;

      explicit IEventManager( const char* pName, bool setAsGlobal );
      virtual ~IEventManager( void );
      virtual bool VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;
      virtual bool VRemoveListener(  const EventListenerDelegate& eventDelegate, const EventType& type  ) = 0;
      virtual bool VTriggerEvent( const IEventDataPtr& pEvent ) const = 0 ;
      virtual bool VQueueEvent( const IEventDataPtr& pEvent ) = 0;
      virtual bool VAbortEvent( const EventType&type, bool allOFType = false ) = 0;
      virtual bool VUpdate( unsigned long maxMs = kINFINITE ) = 0;
      
      static IEventManager* GetSingleton( void );
   private:

   };

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
   {
   public:
      explicit EventManager( const char* pName, bool setAsGlobal );
      virtual ~EventManager( void );
      virtual bool VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type );
      virtual bool VRemoveListener(  const EventListenerDelegate& eventDelegate, const EventType& type  );
      virtual bool VTriggerEvent( const IEventDataPtr& pEvent ) const;
      virtual bool VQueueEvent( const IEventDataPtr& pEvent );
      virtual bool VAbortEvent( const EventType&type, bool allOfType = false );
      // This function is called in EngineApp::OnUpdateGame
      virtual bool VUpdate( unsigned long maxMs = kINFINITE );

   public:
      

      
   private:
      typedef std::list<EventListenerDelegate> EventListenerList;
      typedef std::map<EventType, EventListenerList> EventListenerMap;
      typedef std::list<IEventDataPtr> EventQueue;

      EventListenerMap m_EventListeners;
      //  Two queues, one for current processing events, the other is for next around
      EventQueue m_EventQueues[EVENTMANAGER_NUM_QUEUES];
      // Index of current processing queue
      int m_ActiveQueue;
   };



