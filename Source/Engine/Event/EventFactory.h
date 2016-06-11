#pragma once
/*!
 * \file ScriptEventFactory.h
 * \date 2016/05/08 15:08
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
#include "ScriptEvent.h"


class EventFactory : public ENG_Noncopyable
   {
   public:
      static EventFactory& GetSingleton( void );
      // find or create event matching table in lua and insert a key-value pair ( event name, event id )
      // Then the event class and type pair information is stored in lua, this function is called by MACRO REGISTER_SCRIPT_EVENT
      static void RegisterEventTypeWithScript( const char* key, EventType type );
      // add eventType - creationfunction ptr pair to map, this function is called by MACRO REGISTER_SCRIPT_EVENT
      template <typename T> static void RegisterScriptEventCreation( EventType type );

      template <typename T> static void RegisterEventCreation( EventType type );
      // This function is called by InternalScriptExports::BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData )
      // ( when lua queue want to trigger an event, we have to make a C++ script event )
      static shared_ptr<IScriptEvent> CreateScriptEvent( EventType type );
      static shared_ptr<IEvent> CreateEvent( EventType type );

      static shared_ptr<IScriptEvent> BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData );

   private:
      EventFactory( void );

      GenericObjectFactory< IScriptEvent, EventType> m_ScriptEventFactory;
      GenericObjectFactory< IEvent, EventType> m_EventFactory;
   };

template <typename T> void EventFactory::RegisterScriptEventCreation( EventType type ) 
   { 
   ENG_ASSERT( GetSingleton().m_ScriptEventFactory.Register<T>( type ) ); 
   }

template <typename T> void EventFactory::RegisterEventCreation( EventType type )
   {
   ENG_ASSERT( GetSingleton( ).m_EventFactory.Register<T>( type ) );
   }

#define REGISTER_EVENT( eventClass ) EventFactory::RegisterEventCreation<eventClass>( eventClass::s_EventType )
// This Macro is for online features
#define CREATE_EVENT( s_EventType ) EventFactory::CreateEvent( s_EventType )   

#define REGISTER_SCRIPT_EVENT( eventClass ) \
do \
   { \
	EventFactory::RegisterEventTypeWithScript( #eventClass, eventClass::s_EventType ); \
	EventFactory::RegisterScriptEventCreation<eventClass>( eventClass::s_EventType ); \
   } \
while( 0 )\

#define CREATE_SCRIPT_EVENT( s_EventType, eventData ) EventFactory::BuildEvent( s_EventType, eventData )