////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptExports.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "ScriptExports.h"
#include "LuaStateManager.h"
#include "..\Event\Events.h"

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


