#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.h
////////////////////////////////////////////////////////////////////////////////
#include "..\Event\Events.h"
#include "LuaPlus.h"

typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void);  // function ptr typedef to create a script events

class ScriptEvent : public BaseEventData<ScriptEvent>
   {
   public:
      ScriptEvent( void ) { m_IsEventDataValid = false; }
      LuaPlus::LuaObject GetEventData( void );
      bool SetEventData( LuaPlus::LuaObject eventData );
      // find or create event matching table in lua and insert a key-value pair ( event name, event id )
      static void RegisterEventTypeWithScript( const char* key, EventType type );
      static void AddCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr );
      static ScriptEvent* CreateEventFromScript( EventType type );

   protected:
      virtual bool VBuildEventData( void );
      virtual bool VBuildEventFromScript( void ) { return true; }

   protected:
      LuaPlus::LuaObject m_EventData;

   private:
      typedef std::map< EventType, CreateEventForScriptFunctionType > CreationFunctions;
      static CreationFunctions s_creationFunctions;
      bool m_IsEventDataValid;
   };