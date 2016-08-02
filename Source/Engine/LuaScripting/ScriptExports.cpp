/*!
 * \file ScriptExports.cpp
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
#include "EngineStd.h"
#include "ScriptExports.h"
#include "..\Event\EventFactory.h"


ScriptEventListenerMgr* InternalScriptExports::s_pScriptEventListenerMgr = NULL;

void ScriptExports::Register( void )
   {
   LuaPlus::LuaObject globals = LuaStateManager::GetSingleton().GetGlobalVars();

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
	SAFE_DELETE( s_pScriptEventListenerMgr );
   };

//TODO: unfinished function
bool InternalScriptExports::LoadAndExecuteScriptResource( const char *scriptRes )
   {
   Resource resource( scriptRes );
   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( resource );
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
	ENG_ASSERT( listenerId );
	
	// convert the listenerId back into a pointer
	ScriptEventListener* pListener = reinterpret_cast< ScriptEventListener* >( listenerId );
	s_pScriptEventListenerMgr->DestroyListener( pListener );  // the destructor will remove the listener
   }

bool InternalScriptExports::QueueEvent( EventType eventType, LuaPlus::LuaObject eventData )
   {
   shared_ptr<IScriptEvent> pEvent( EventFactory::BuildEvent( eventType, eventData ) );
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
	shared_ptr<IScriptEvent> pEvent( EventFactory::BuildEvent( eventType, eventData ) );
    if ( pEvent )
       {
	    return IEventManager::GetSingleton()->VTriggerEvent( pEvent );
       }
    return false;
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

EventListenerDelegate ScriptEventListener::GetDelegate( void )
   {
   return fastdelegate::MakeDelegate( this, &ScriptEventListener::ScriptEventDelegate );
   }

// forward the event ptr to lua and trigger this function
void ScriptEventListener::ScriptEventDelegate( IEventPtr pEvent )
   {
   shared_ptr<IScriptEvent> pScriptEvent = dynamic_pointer_cast<IScriptEvent>( pEvent );
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


