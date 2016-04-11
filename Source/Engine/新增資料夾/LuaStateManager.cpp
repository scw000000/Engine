////////////////////////////////////////////////////////////////////////////////
// Filename: LuaStateManager.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "LuaStateManager.h"

bool LuaStateManager::Create( void )
   {
   if( s_pLuaStateMgr )
      {
      ENG_ERROR( "Overwriting singleton pointer" );
      SAFE_DELETE( s_pLuaStateMgr );
      }
   s_pLuaStateMgr = ENG_NEW LuaStateManager;

   if( s_pLuaStateMgr )
      {
      return s_pLuaStateMgr->VInit();
      }

   return true;
   }

void LuaStateManager::Destroy( void )
   {
   ENG_ASSERT( s_pLuaStateMgr );
   SAFE_DELETE( s_pLuaStateMgr );
   }

bool LuaStateManager::VInit( void )
   {
   m_pLuaState = LuaPlus::LuaState::Create( true );
   if ( m_pLuaState == NULL )
      {
      return false;
      }

    // register functions
    m_pLuaState->GetGlobals().RegisterDirect( "ExecuteFile", (*this), &LuaStateManager::VExecuteFile );
    m_pLuaState->GetGlobals().RegisterDirect( "ExecuteString", (*this), &LuaStateManager::VExecuteString );

    return true;
   }

void LuaStateManager::VExecuteFile( const char* resource ) 
   {
   
   }

void LuaStateManager::VExecuteString( const char*str ) 
   {
   
   }

LuaPlus::LuaObject LuaStateManager::GetGlobalVars( void )
   {
   }

LuaPlus::LuaState* LuaStateManager::GetLuaState( void )
   {
   }

LuaPlus::LuaObject LuaStateManager::CreatePath( const char *pathStr, bool toIgnoreLastElement = false )
   {
   
   }

void LuaStateManager::ConvertVec3ToTable( const Vec3& vec, LuaPlus::LuaObject& luaTable, Vec3& outVec3 )
   {
   
   }

void LuaStateManager::SetError( int errorNum )
   {
   
   }

void LuaStateManager::ClearStack( void )
   {
   
   }

LuaStateManager::LuaStateManager(void)
   {
   m_pLuaState = NULL;
   }

LuaStateManager::~LuaStateManager( void )
   {
   // Remember not to delete this pointer directly
   if( m_pLuaState )
      {
      LuaPlus::LuaState::Destroy( m_pLuaState );
      m_pLuaState = NULL;
      }
   }
