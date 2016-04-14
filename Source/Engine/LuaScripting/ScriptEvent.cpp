////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "ScriptEvent.h"
#include "LuaStateManager.h"

ScriptEvent::CreationFunctions ScriptEvent::s_CreationFunctions;

LuaPlus::LuaObject ScriptEvent::GetLuaEventData( void )
   {
   if ( !m_IsEventDataValid )
	   {
		VBuildLuaEventData();
		m_IsEventDataValid = true;
	   }
	
	return m_LuaEventData;
   }

bool ScriptEvent::SetEventData( LuaPlus::LuaObject eventData )
   {
   m_LuaEventData = eventData;
	m_IsEventDataValid = VBuildEventFromScript();
	return m_IsEventDataValid;
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
   // Make sure this key and type binding pair does not exist
   ENG_ASSERT( eventTypeTable[key].IsNil() );
   eventTypeTable.SetNumber( key, (double)type );
   }

void ScriptEvent::AddCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr )
   {
   // make sure creation functino is not exist in map before
   ENG_ASSERT( s_CreationFunctions.find(type) == s_CreationFunctions.end() );
	ENG_ASSERT( pCreationFunctionPtr != NULL );
	s_CreationFunctions.insert(std::make_pair( type, pCreationFunctionPtr ) );
   }

ScriptEvent* ScriptEvent::CreateEventFromScript( EventType type )
   {
   CreationFunctions::iterator findIt = s_CreationFunctions.find( type );
	if ( findIt != s_CreationFunctions.end() )
	   {
		CreateEventForScriptFunctionType func = findIt->second;
		return func();
	   }
	else
	   {
		ENG_ERROR( "Couldn't find event type" );
		return NULL;
	   }
   }

//---------------------------------------------------------------------------------------------------------------------
// Default implementation for VBuildLuaEventData() sets the event data to nil.
//---------------------------------------------------------------------------------------------------------------------
bool ScriptEvent::VBuildLuaEventData( void )
   {
   m_LuaEventData.AssignNil( LuaStateManager::GetSingleton()->GetLuaState() );
   return true;
   }
