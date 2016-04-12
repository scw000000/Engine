////////////////////////////////////////////////////////////////////////////////
// Filename: LuaStateManager.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "LuaStateManager.h"

LuaStateManager* LuaStateManager::s_pLuaStateMgr = NULL;

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

    // expose menber functions to lua as global functions
    m_pLuaState->GetGlobals().RegisterDirect( "ExecuteFile", (*this), &LuaStateManager::VExecuteFile );
    m_pLuaState->GetGlobals().RegisterDirect( "ExecuteString", (*this), &LuaStateManager::VExecuteString );

    return true;
   }

void LuaStateManager::VExecuteFile( const char* resource ) 
   {
   int result = m_pLuaState->DoFile( resource );
   if (result != 0)
      {
      SetError(result);
      }
   }

void LuaStateManager::VExecuteString( const char*str ) 
   {
   int result = 0;

    // Most strings are passed straight through to the Lua interpreter
   if ( strlen(str) <= 1 || str[0] != '=' )
      {
      result = m_pLuaState->DoString(str);
      if ( result != 0 )
         {
         SetError(result);
         }
      }
    // If the string starts with '=' and length > 1( not only "=" ), wrap the statement in the print() function.
   else
      {
      std::string buffer("print(");
      buffer += (str + 1);
      buffer += ")";
      result = m_pLuaState->DoString( buffer.c_str() );
      if ( result != 0 )
         {
         SetError(result);
         }
      }
   }

LuaPlus::LuaObject LuaStateManager::GetGlobalVars( void )
   {
   ENG_ASSERT( m_pLuaState );
   return m_pLuaState->GetGlobals();
   }

LuaPlus::LuaState* LuaStateManager::GetLuaState( void ) const
   {
   ENG_ASSERT( m_pLuaState );
   return m_pLuaState;
   }

LuaPlus::LuaObject LuaStateManager::CreatePath( const char *pathStr, bool toIgnoreLastElement )
   {
   StringVec splitPath;
   Split( pathStr, splitPath, '.' );
   if ( toIgnoreLastElement )
      {
      splitPath.pop_back();
      }

   LuaPlus::LuaObject currTable = GetGlobalVars();
   for ( auto it = splitPath.begin(); it != splitPath.end(); ++it )
      {
      // make sure we still have a valid context
      if ( currTable.IsNil() )
         {
         ENG_ERROR("Something broke in CreatePath(); bailing out (element == " + (*it) + ")");
         return currTable;  // this will be nil
         }

      // grab whatever exists for this element
      const std::string& element = (*it);
      LuaPlus::LuaObject nxtTable = currTable.GetByName( element.c_str() );
      // If next talbe is not exist( may be NULL or stored as different type )
      if ( !nxtTable.IsTable() )
         {
         // if the element is not a table and not nil, we clobber it
         if ( !nxtTable.IsNil() )
            {
            ENG_WARNING( "Overwriting element '" + element + "' in table" );
            currTable.SetNil( element.c_str() );
            }

         // element is either nil or was clobbered so add the new table
         currTable.CreateTable( element.c_str() );
         }

      currTable = currTable.GetByName( element.c_str() );
      }

   // if we get here, we have created the path
   return currTable;
   }

void LuaStateManager::ConvertVec3ToTable( const Vec3& vec, LuaPlus::LuaObject& outLuaTable ) const 
   {
   // Create a new table from m_pState
   outLuaTable.AssignNewTable( GetLuaState() );  
   outLuaTable.SetNumber( "x", vec.x );
   outLuaTable.SetNumber( "y", vec.y );
   outLuaTable.SetNumber( "z", vec.z );
   }

void LuaStateManager::SetError( int errorNum )
   {
   // Note: If we get an error, we're hosed because LuaPlus throws an exception.  So if this function
   // is called and the error at the bottom triggers, you might as well pack it in.

   LuaPlus::LuaStackObject stackObj( m_pLuaState,-1 );
   const char* errStr = stackObj.GetString();
   if ( errStr )
      {
      m_LastError = errStr;
      ClearStack();
      }
   else
      {
      m_LastError = "Unknown Lua parse error";
      }
   ENG_ERROR (m_LastError );
   }

void LuaStateManager::ClearStack( void )
   {
   m_pLuaState->SetTop(0);
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
