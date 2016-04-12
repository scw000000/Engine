////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "ScriptEvent.h"
#include "LuaStateManager.h"


LuaPlus::LuaObject ScriptEvent::GetEventData( void )
   {
   
   }

bool ScriptEvent::SetEventData( LuaPlus::LuaObject eventData )
   {
   
   }


void ScriptEvent::RegisterEventTypeWithScript( const char* key, EventType type )
   {
   // load existing eventType table in lua or create it if its not exist
   LuaPlus::LuaObject eventTypeTable = LuaStateManager::GetSingleton()->GetGlobalVars().GetByName( "EventType" );
   if( eventTypeTable.IsNil() )
      {
      eventTypeTable = LuaStateManager::GetSingleton()->GetGlobalVars().CreateTable( "EventType" );
      }
   ENG_ASSERT( eventTypeTable.IsTable() );
   // Make sure this key and type binding do not exist
   ENG_ASSERT( eventTypeTable[key].IsNil() );
   eventTypeTable.SetNumber( key, (double)type );
   }

void ScriptEvent::AddCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr )
   {
   
   }

ScriptEvent* ScriptEvent::CreateEventFromScript( EventType type )
   {
   
   }

bool ScriptEvent::VBuildEventData( void )
   {
   
   }
