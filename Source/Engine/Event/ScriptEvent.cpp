////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "ScriptEvent.h"
#include "..\LuaScripting\LuaStateManager.h"

LuaPlus::LuaObject ScriptEventImp::GetLuaEventData( void )
   {
   if ( !m_IsEventDataValid )
	   {
		VBuildScriptEventData();
		m_IsEventDataValid = true;
	   }
	
	return m_LuaEventData;
   }

bool ScriptEventImp::SetEventData( LuaPlus::LuaObject eventData )
   {
   m_LuaEventData = eventData;
	m_IsEventDataValid = VBuildEventFromScript();
	return m_IsEventDataValid;
   }

//---------------------------------------------------------------------------------------------------------------------
// Default implementation for VBuildLuaEventData() sets the event data to nil.
//---------------------------------------------------------------------------------------------------------------------
void ScriptEventImp::VBuildScriptEventData( void )
   {
   m_LuaEventData.AssignNil( LuaStateManager::GetSingleton()->GetLuaState() );
   }
