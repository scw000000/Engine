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


typedef ScriptEventImp* ( *CreateEventForScriptFunctionType )( void );  // function ptr typedef to create a script event

#define REGISTER_SCRIPT_EVENT( eventClass ) \
	ScriptEventFactory::RegisterEventTypeWithScript( #eventClass, eventClass::s_EventType ); \
	ScriptEventFactory::RegisterCreationFunction<eventClass>( eventClass::s_EventType )

class ScriptEventFactory
   {
   public:
      // find or create event matching table in lua and insert a key-value pair ( event name, event id )
      // Then the event class and type pair information is stored in lua, this function is called by MACRO REGISTER_SCRIPT_EVENT
      static void RegisterEventTypeWithScript( const char* key, EventType type );
      // add eventType - creationfunction ptr pair to map, this function is called by MACRO REGISTER_SCRIPT_EVENT
      template <typename T> static void RegisterCreationFunction( EventType type ) { ENG_ASSERT( s_ScriptEventFactory.Register<T>( type ) ); }
      // This function is called by InternalScriptExports::BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData )
      // ( when lua queue want to trigger an event, we have to make a C++ script event )
      static ScriptEventImp* CreateEventFromScript( EventType type );

   private:
      static GenericObjectFactory< ScriptEventImp, EventType> s_ScriptEventFactory;
   };