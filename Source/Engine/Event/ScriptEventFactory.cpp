/*!
 * \file ScriptEventFactory.cpp
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
#include "ScriptEventFactory.h"
#include "..\LuaScripting\LuaStateManager.h"
#include "PhysicsEvents.h"

GenericObjectFactory< ScriptEventImp, EventType> ScriptEventFactory::s_ScriptEventFactory;

void ScriptEventFactory::RegisterEventTypeWithScript( const char* key, EventType type )
   {
   // load existing eventType table in lua or create it if its not exist
   LuaPlus::LuaObject eventTypeTable = LuaStateManager::GetSingleton( )->GetGlobalVars( ).GetByName( "EventType" );
   if( eventTypeTable.IsNil( ) )
      {
      eventTypeTable = LuaStateManager::GetSingleton( )->GetGlobalVars( ).CreateTable( "EventType" );
      }
   ENG_ASSERT( eventTypeTable.IsTable( ) );
   // Make sure this key and type binding pair does not exist
   ENG_ASSERT( eventTypeTable[key].IsNil( ) );
   eventTypeTable.SetNumber( key, ( double ) type );
   }
//
//template <typename T> void ScriptEventFactory::RegisterCreationFunction( EventType type )
//   {
//   ENG_ASSERT( s_ScriptEventFactory.Register<T>( type ) );
//
//   //// make sure creation functino is not exist in map before
//   //ENG_ASSERT( s_CreationFunctions.find( type ) == s_CreationFunctions.end( ) );
//   //ENG_ASSERT( pCreationFunctionPtr != NULL );
//   //s_CreationFunctions.insert( std::make_pair( type, pCreationFunctionPtr ) );
//   }

ScriptEventImp* ScriptEventFactory::CreateEventFromScript( EventType type )
   {
   return s_ScriptEventFactory.Create( type );
   /*CreationFunctions::iterator findIt = s_CreationFunctions.find( type );
   if( findIt != s_CreationFunctions.end( ) )
   {
   CreateEventForScriptFunctionType func = findIt->second;
   return func( );
   }
   else
   {
   ENG_ERROR( "Couldn't find event type" );
   return NULL;
   }*/
   }