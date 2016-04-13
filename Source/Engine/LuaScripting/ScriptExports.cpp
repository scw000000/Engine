////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptExports.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "ScriptExports.h"


void ScriptExports::Register( void )
   {
   LuaPlus::LuaObject globals = LuaStateManager::GetSingleton()->GetGlobalVars();

	// init	
	InternalScriptExports::Init();

	// resource loading
	globals.RegisterDirect( "LoadAndExecuteScriptResource", &InternalScriptExports::LoadAndExecuteScriptResource );

   }

void ScriptExports::Unregister( void )
   {
   InternalScriptExports::Destroy();
   }

//TODO: finish implementation
bool InternalScriptExports::Init(void)
   {
	//ENG_ASSERT( s_pScriptEventListenerMgr == NULL );
	//s_pScriptEventListenerMgr = GCC_NEW ScriptEventListenerMgr;
	
	return true;
   }

void InternalScriptExports::Destroy( void )
   {
   //GCC_ASSERT( s_pScriptEventListenerMgr != NULL );
	//SAFE_DELETE( s_pScriptEventListenerMgr );
   };

bool InternalScriptExports::LoadAndExecuteScriptResource( const char *scriptRes )
   {
   Resource resource( scriptRes );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );
   return pResourceHandle != NULL;
   }

bool InternalScriptExports::QueueEvent( EventType eventType, LuaPlus::LuaObject eventData )
   {
   shared_ptr<ScriptEvent> pEvent( BuildEvent( eventType, eventData ) );
   if( pEvent )
      {
      IEventManager::GetSingleton()->VQueueEvent( pEvent );
      return false;
      }
   return false;
   }

//---------------------------------------------------------------------------------------------------------------------
// Sends an event from the script.  Returns true if the event was sent, false if not.
//---------------------------------------------------------------------------------------------------------------------
bool InternalScriptExports::TriggerEvent( EventType eventType, LuaPlus::LuaObject eventData )
   {
	shared_ptr<ScriptEvent> pEvent( BuildEvent( eventType, eventData ) );
    if ( pEvent )
       {
	    return IEventManager::GetSingleton()->VTriggerEvent( pEvent );
       }
    return false;
   }

//---------------------------------------------------------------------------------------------------------------------
// Builds the event to be sent or queued
//---------------------------------------------------------------------------------------------------------------------
shared_ptr<ScriptEvent> InternalScriptExports::BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData )
{
	// create the event from the event type
	shared_ptr<ScriptEvent> pEvent( ScriptEvent::CreateEventFromScript( eventType ) );
	if ( !pEvent )
        return shared_ptr<ScriptEvent>();

	// set the event data that was passed in
	if ( !pEvent->SetEventData( eventData ) )
	   {
		return shared_ptr<ScriptEvent>();
	   }
	
	return pEvent;
}

