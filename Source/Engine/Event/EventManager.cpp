/*!
 * \file EventManager.cpp
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
#include "EngineStd.h"
#include "EventManager.h"

static IEventManager* g_pEventMgr = NULL;

IEventManager* IEventManager::GetSingleton( void )
   {
   ENG_ASSERT( g_pEventMgr );
	return g_pEventMgr;
   }

IEventManager::IEventManager( const char* pName, bool setAsGlobal )
   {
	if ( setAsGlobal )
      {
      if ( g_pEventMgr )
         {
         ENG_ERROR("Attempting to create two global event managers! The old one will be destroyed and overwritten with this one.");
         delete g_pEventMgr;
         }

		g_pEventMgr = this;
      }
   }

IEventManager::~IEventManager(void)
   {
	if ( g_pEventMgr == this )
		g_pEventMgr = NULL;
   }

EventManager::EventManager( const char* pName, bool setAsGlobal ) : IEventManager( pName, setAsGlobal )
   {
   m_ActiveQueue = 0;
   }

EventManager::~EventManager( void )
   {
   
   }

bool EventManager::VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type )
   {
   // find correspond listing list in the map, if it doesn't exist, create it
   EventListenerList& eventListernList = m_EventListeners[type];
   for( auto it = eventListernList.begin(); it != eventListernList.end(); ++it )
      {
      if( eventDelegate == (*it) )
         {
         ENG_WARNING( "Attempting to double-register a delegate" );
         return false;
         }
      }
   eventListernList.push_back( eventDelegate );
   return true;
   }

bool EventManager::VRemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type  )
   {
   auto mapIt = m_EventListeners.find( type );
   // corresponding listing list is not exist
   if( mapIt == m_EventListeners.end() )
      {
      return false;
      }
   auto eventListenerList = mapIt->second;
   for( auto listIt = eventListenerList.begin(); listIt != eventListenerList.end(); ++listIt )
      {
      if( eventDelegate == (*listIt) )
         {
         eventListenerList.erase( listIt );
         return true;
         }
      }
   return false;
   }

// Make all listeners process the event right now instead of add the event into queue
bool EventManager::VTriggerEvent( const IEventPtr& pEvent ) const
   {
   auto mapIt = m_EventListeners.find( pEvent->VGetEventType() );
   // corresponding listing list is not exist
   if( mapIt == m_EventListeners.end() )
      {
      return false;
      }
   auto eventListenerList = mapIt->second;
   bool processed = false;
   for( auto listIt = eventListenerList.begin(); listIt != eventListenerList.end(); ++listIt )
      {
      EventListenerDelegate listener = (*listIt);
      // trigger event
      listener( pEvent );
      processed = true;
      }
   return processed;
   }

//  Push event into active queue
bool EventManager::VQueueEvent( const IEventPtr& pEvent )
   {
   ENG_ASSERT( m_ActiveQueue >= 0 );
   ENG_ASSERT( m_ActiveQueue < EVENTMANAGER_NUM_QUEUES );
   auto mapIt = m_EventListeners.find( pEvent->VGetEventType() );
   // If no one is lisiting, abort
   if( mapIt == m_EventListeners.end() || !mapIt->second.size() )
      {
      return false;
      }
   
   m_EventQueues[m_ActiveQueue].push_back( pEvent );
   return true;
   }

//  Remove specific type of event from event queue, pretend it never happen
bool EventManager::VAbortEvent( const EventType&type, bool allOfType )
   {
   ENG_ASSERT( m_ActiveQueue >= 0 );
   ENG_ASSERT( m_ActiveQueue < EVENTMANAGER_NUM_QUEUES );
   auto mapIt = m_EventListeners.find( type );
   // If no one is lisiting, forget about it
   if( mapIt == m_EventListeners.end() )
      {
      return false;
      }
   bool processed = false;
   EventQueue& eventQueue = m_EventQueues[ m_ActiveQueue ];
   auto queueIt = eventQueue.begin();
   while(  queueIt != eventQueue.end() )
      {
      auto currQueueIt = queueIt;
      ++queueIt;
      if( (*currQueueIt)->VGetEventType() == type )
         {
         eventQueue.erase( currQueueIt );
         processed = true;
         // If we only want to delete only first one event, break;
         if( !allOfType )
            {
            break;
            }
         }
      }
   return processed;
   }

bool EventManager::VUpdate( unsigned long maxMs ) 
   {
   unsigned long currMs = (unsigned long) ( GetGlobalTimer()->GetTime() * 1000.0 );
   unsigned long targetMs = ( maxMs == kINFINITE )? kINFINITE: currMs + maxMs;

	// swap active queues and clear the new queue after the swap
    int queueToProcess = m_ActiveQueue;
	m_ActiveQueue = (m_ActiveQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_EventQueues[m_ActiveQueue].clear();

   std::string s( ( "EventLoop", "Processing Event Queue " +
      ToStr( queueToProcess ) + "; " +
      ToStr( ( unsigned long ) m_EventQueues[ queueToProcess ].size() )
      + " events to process" ) ); \
      Logger::Log( "EventLoop", s, NULL, NULL, 0 ); \
   /*ENG_LOG("EventLoop", "Processing Event Queue " + 
            ToStr(queueToProcess) + "; " + 
            ToStr((unsigned long)m_EventQueues[queueToProcess].size()) 
            + " events to process");*/

	// Process the queue
	while ( !m_EventQueues[queueToProcess].empty() )
	   {
      // pop the front of the queue
		IEventPtr pEvent = m_EventQueues[queueToProcess].front();
      m_EventQueues[queueToProcess].pop_front();
      ENG_LOG( "EventLoop", "\t\tProcessing Event " + std::string( pEvent->GetName() ) );

		const EventType& eventType = pEvent->VGetEventType();

      // find all the delegate functions registered for this event
		auto findIt = m_EventListeners.find(eventType);
		if (findIt != m_EventListeners.end())
		   {
			const EventListenerList& eventListeners = findIt->second;
         ENG_LOG("EventLoop", "\t\tFound " + ToStr((unsigned long)eventListeners.size()) + " delegates");

         // call each listener
			for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
			   {
            EventListenerDelegate listener = (*it);
            ENG_LOG("EventLoop", "\t\tSending event " + std::string(pEvent->GetName()) + " to delegate");
				listener(pEvent);
			   }
		   }

        // check to see if time ran out
		currMs = (unsigned long) ( GetGlobalTimer()->GetTime() * 1000.0 );
		if ( targetMs != IEventManager::kINFINITE && currMs >= targetMs )
         {
         ENG_LOG("EventLoop", "Aborting event processing; time ran out");
		   break;
         }
	   }
	
	// If we couldn't process all of the events, push the remaining events to the new active queue.
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
	bool queueFlushed = ( m_EventQueues[queueToProcess].empty() );
	if ( !queueFlushed )
	   {
		while (!m_EventQueues[queueToProcess].empty())
		   {
			IEventPtr pEvent = m_EventQueues[queueToProcess].back();
			m_EventQueues[queueToProcess].pop_back();
			m_EventQueues[m_ActiveQueue].push_front(pEvent);
		   }
	   }
	
	return queueFlushed;
   }