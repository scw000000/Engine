#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.h
////////////////////////////////////////////////////////////////////////////////
#include "..\Event\Events.h"
#include "LuaPlus.h"

typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void);  // function ptr typedef to create a script events

#define REGISTER_SCRIPT_EVENT( eventClass, eventType ) \
	ScriptEvent::RegisterEventTypeWithScript( #eventClass, eventType ); \
	ScriptEvent::AddCreationFunction( eventType, &eventClass::CreateEventForScript )
	
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
      LuaPlus::LuaObject GetEventData( void );
      bool SetEventData( LuaPlus::LuaObject eventData );
      // find or create event matching table in lua and insert a key-value pair ( event name, event id )
      static void RegisterEventTypeWithScript( const char* key, EventType type );
      // add eventType - creationfunction ptr pair to map
      static void AddCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr );
      static ScriptEvent* CreateEventFromScript( EventType type );

   protected:
      virtual bool VBuildEventData( void );
      virtual bool VBuildEventFromScript( void ) { return true; }

   protected:
      LuaPlus::LuaObject m_LuaEventData; // the counterpary

   private:
      typedef std::map< EventType, CreateEventForScriptFunctionType > CreationFunctions;
      static CreationFunctions s_CreationFunctions;
      bool m_IsEventDataValid;
   };