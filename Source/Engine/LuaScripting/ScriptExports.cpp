////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptExports.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "ScriptExports.h"


ScriptEventListenerMgr* InternalScriptExports::s_pScriptEventListenerMgr = NULL;

void ScriptExports::Register( void )
   {
   LuaPlus::LuaObject globals = LuaStateManager::GetSingleton()->GetGlobalVars();

	// init	
	InternalScriptExports::Init();

	// resource loading
	globals.RegisterDirect( "LoadAndExecuteScriptResource", &InternalScriptExports::LoadAndExecuteScriptResource );

   globals.RegisterDirect( "RegisterEventListener", &InternalScriptExports::RegisterEventListener );
	globals.RegisterDirect( "RemoveEventListener", &InternalScriptExports::RemoveEventListener );
   globals.RegisterDirect( "QueueEvent", &InternalScriptExports::QueueEvent );
	globals.RegisterDirect( "TriggerEvent", &InternalScriptExports::TriggerEvent );
   }

void ScriptExports::Unregister( void )
   {
   InternalScriptExports::Destroy();
   }

//TODO: finish implementation
bool InternalScriptExports::Init(void)
   {
	ENG_ASSERT( s_pScriptEventListenerMgr == NULL );
	s_pScriptEventListenerMgr = ENG_NEW ScriptEventListenerMgr;
	
	return true;
   }

void InternalScriptExports::Destroy( void )
   {
   ENG_ASSERT( s_pScriptEventListenerMgr );
	SAFE_DELETE( s_pScriptEventListenerMgr );
   };

bool InternalScriptExports::LoadAndExecuteScriptResource( const char *scriptRes )
   {
   Resource resource( scriptRes );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );
   return pResourceHandle != NULL;
   }

unsigned long InternalScriptExports::RegisterEventListener( EventType eventType, LuaPlus::LuaObject callbackFunction )
   {
   ENG_ASSERT( s_pScriptEventListenerMgr );
   if( callbackFunction.IsFunction() )
      {
      ScriptEventListener* pListener = ENG_NEW ScriptEventListener( eventType, callbackFunction );
      s_pScriptEventListenerMgr->AddListener( pListener );
      IEventManager::GetSingleton()->VAddListener( pListener->GetDelegate(), eventType );
      unsigned long handle = reinterpret_cast<unsigned long>( pListener );
      return handle;
      }
   ENG_ERROR("Attempting to register script event listener with invalid callback function");
	return 0;
   }

void InternalScriptExports::RemoveEventListener( unsigned long listenerId )
   {
   ENG_ASSERT( s_pScriptEventListenerMgr );
	ENG_ASSERT( listenerId != 0 );
	
	// convert the listenerId back into a pointer
	ScriptEventListener* pListener = reinterpret_cast< ScriptEventListener* >(listenerId);
	s_pScriptEventListenerMgr->DestroyListener( pListener );  // the destructor will remove the listener
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

ScriptEventListener::ScriptEventListener( const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction ) : m_ScriptCallbackFunction( scriptCallbackFunction )
   {
   m_EventType = eventType;
   }

ScriptEventListener::~ScriptEventListener( void )
   {
   IEventManager* pEventMgr = IEventManager::GetSingleton();
    if ( pEventMgr )
       {
       pEventMgr->VRemoveListener( GetDelegate(), m_EventType );
       }
   }

void ScriptEventListener::ScriptEventDelegate( IEventDataPtr pEvent )
   {
   shared_ptr<ScriptEvent> pScriptEvent = static_pointer_cast<ScriptEvent>( pEvent );
   LuaPlus::LuaFunction< void* > callback = m_ScriptCallbackFunction;
   callback( pScriptEvent->GetLuaEventData() );
   }

ScriptEventListenerMgr::~ScriptEventListenerMgr( void )
   {
   for (auto it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
	   {
		ScriptEventListener* pListener = ( *it );
		SAFE_DELETE( pListener );
	   
      }
	m_Listeners.clear();
   }

void ScriptEventListenerMgr::AddListener( ScriptEventListener *pListener )
   {
   ENG_ASSERT( pListener );
   // std::set won't allow more than one exactly same pListener in set, so we insert it directly without checking
   m_Listeners.insert( pListener );
   }

void ScriptEventListenerMgr::DestroyListener( ScriptEventListener *pListener )
   {
   ScriptEventListenerSet::iterator findIt = m_Listeners.find(pListener);
	if ( findIt != m_Listeners.end() )
	   {
		m_Listeners.erase(findIt);
		delete pListener;
	   }
	else
	   {
		ENG_ERROR("Couldn't find script listener in set; this will probably cause a memory leak");
	   }
   }


