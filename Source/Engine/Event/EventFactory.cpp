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
#include "EventFactory.h"
#include "..\LuaScripting\LuaStateManager.h"
#include "PhysicsEvents.h"


EventFactory& EventFactory::GetSingleton( void )
   {
   static EventFactory instance;
   return instance;   
   }


void EventFactory::RegisterEventTypeWithScript( const char* key, EventType type )
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

shared_ptr<IScriptEvent> EventFactory::BuildEvent( EventType eventType, LuaPlus::LuaObject& eventData )
   {
   // create the event from the event type
   shared_ptr<IScriptEvent> pEvent( EventFactory::CreateScriptEvent( eventType ) );
   if( !pEvent )
      {
      return shared_ptr<IScriptEvent>( );
      }
   // set the event data that was passed in
   if( !pEvent->SetEventData( eventData ) )
      {
      return shared_ptr<IScriptEvent>( );
      }
   return pEvent;
   }
//
//template <typename T> void EventFactory::RegisterCreationFunction( EventType type )
//   {
//   ENG_ASSERT( s_ScriptEventFactory.Register<T>( type ) );
//
//   //// make sure creation functino is not exist in map before
//   //ENG_ASSERT( s_CreationFunctions.find( type ) == s_CreationFunctions.end( ) );
//   //ENG_ASSERT( pCreationFunctionPtr != NULL );
//   //s_CreationFunctions.insert( std::make_pair( type, pCreationFunctionPtr ) );
//   }

shared_ptr<IScriptEvent> EventFactory::CreateScriptEvent( EventType type )
   {
   return  shared_ptr<IScriptEvent>( GetSingleton().m_ScriptEventFactory.Create( type ) );
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

shared_ptr<IEvent> EventFactory::CreateEvent( EventType type )
   {
   if( GetSingleton().m_EventFactory.IsRegistered( type ) )
      {
      return shared_ptr<IEvent>( GetSingleton().m_EventFactory.Create( type ) );
      }
   return shared_ptr<IEvent>( GetSingleton().m_ScriptEventFactory.Create( type ) );
   }

EventFactory::EventFactory( void )
   {
   
   }