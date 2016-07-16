#pragma once
/*!
 * \file EventManager.h
 * \date 2016/05/09 22:15
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
#include "Events.h"


class IEventManager : public ENG_Noncopyable
   {
   public:
      const static unsigned long kINFINITE = 0xffffffff;

      explicit IEventManager( const char* pName, bool setAsGlobal );
      virtual ~IEventManager( void );
      virtual bool VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;
      virtual bool VRemoveListener(  const EventListenerDelegate& eventDelegate, const EventType& type  ) = 0;
      virtual bool VTriggerEvent( IEventPtr pEvent ) const = 0 ;
      virtual bool VQueueEvent( const IEventPtr& pEvent ) = 0;
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
      virtual bool VTriggerEvent( IEventPtr pEvent ) const;
      virtual bool VQueueEvent( const IEventPtr& pEvent );
      virtual bool VAbortEvent( const EventType&type, bool allOfType = false );
      // This function is called in EngineApp::OnUpdateGame
      virtual bool VUpdate( unsigned long maxMs = kINFINITE );

   public:
      

      
   private:
      typedef std::list<EventListenerDelegate> EventListenerList;
      typedef std::map<EventType, EventListenerList> EventListenerMap;
      typedef std::list<IEventPtr> EventQueue;

      EventListenerMap m_EventListeners;
      //  Two queues, one for current processing events, the other is for next around
      EventQueue m_EventQueues[EVENTMANAGER_NUM_QUEUES];
      // Index of current processing queue
      int m_ActiveQueue;

   };



