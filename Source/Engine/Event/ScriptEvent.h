#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.h
////////////////////////////////////////////////////////////////////////////////
#include "..\Event\Events.h"
#include "LuaPlus.h"

class ScriptEvent;

typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void);  // function ptr typedef to create a script event

#define REGISTER_SCRIPT_EVENT( eventClass, eventType ) \
	ScriptEvent::RegisterEventTypeWithScript( #eventClass, eventType ); \
	ScriptEvent::AddCreationFunction( eventType, &eventClass::CreateEventForScript )

// this MACRO should be called within Scriptclass definition
// 
#define EXPORT_FOR_SCRIPT_EVENT( eventClass ) \
	public: \
		static ScriptEvent* CreateEventForScript( void ) \
		{ \
			return ENG_NEW eventClass; \
		}

//---------------------------------------------------------------------------------------------------------------------
// Script event base class.  This class is meant to be subclassed by any event that can be sent or received by the
// script.  Note that these events are not limited to script and can be received just fine by C++ listeners.
// Furthermore, since the Script data isn't built unless being received by a script listener, there's no worry about 
// performance.
//---------------------------------------------------------------------------------------------------------------------
class ScriptEvent : public BaseEventData<ScriptEvent>
   {
   public:
      ScriptEvent( void ) { m_IsEventDataValid = false; }
      // set m_LuaEventData and copy C++ menber variable to lua event data
      // This function is called by ScriptEventListener::ScriptEventDelegate(IEventDataPtr pEvent)  
      // ( when we want to call a lua callback function )
      LuaPlus::LuaObject GetLuaEventData( void );
      // set m_LuaEventData from eventData and copy it to C++ menber
      // This function is called by InternalScriptExports::BuildEvent(EventType eventType, LuaPlus::LuaObject& eventData) 
      // ( when lua queue or trigger an event, we have to make a C++ script event )
      bool SetEventData( LuaPlus::LuaObject eventData );
      // find or create event matching table in lua and insert a key-value pair ( event name, event id )
      // So the event class and type pair information is stored in lua, this function is called by MACRO REGISTER_SCRIPT_EVENT
      static void RegisterEventTypeWithScript( const char* key, EventType type );
      // add eventType - creationfunction ptr pair to map
      static void AddCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr );
      // This function is called by InternalScriptExports::BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData )
      // ( when lua queue want to trigger an event, we have to make a C++ script event )
      static ScriptEvent* CreateEventFromScript( EventType type );

   protected:
      // This function is for the event being sent from C++ to lua. Called by GetEventData
      // What this function should do is copy specific C++ event data, such as damage value from C++ menber variable to lua
      virtual bool VBuildLuaEventData( void );
      // This function is for the event being sent from lua to C++. Called by SetEventData
      // What this function should do is copy specific lua event data, such as damage value from lua to C++ menber variable
      virtual bool VBuildEventFromScript( void ) { return true; }

   protected:
      LuaPlus::LuaObject m_LuaEventData; // the counterpart lua event data

   private:
      typedef std::map< EventType, CreateEventForScriptFunctionType > CreationFunctions;
      static CreationFunctions s_CreationFunctions;
      bool m_IsEventDataValid;
   };