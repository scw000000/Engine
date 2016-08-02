#pragma once
/*!
 * \file ScriptExports.h
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

#include "..\Event\ScriptEvent.h"
#include "FastDelegate.h"
#include "LuaStateManager.h"
#include "..\Event\Events.h"
#include "..\Event\ScriptEvent.h"
#include "..\Event\EventManager.h"

namespace ScriptExports
   {
   // decide which static functions in InternalScriptExports can be exposed to lua
   void Register( void );
   void Unregister( void );
   }

class ScriptEventListenerMgr;
class ScriptEventFactory;

// This class contains some static functions for lua to call
class InternalScriptExports
   {
   public:
      static bool Init( void );
      static void Destroy( void );
      static bool LoadAndExecuteScriptResource( const char *scriptRes );
      // new a eventListener and bind lua function, also create a delegate for EventManager
      static unsigned long RegisterEventListener( EventType eventType, LuaPlus::LuaObject callbackFunction );
      // this function enables lua to remove listener by calling listenerId, which is actually a pointer
      // because lua does not know what is pointer
      static void RemoveEventListener( unsigned long listenerId );
      // Generate a corresponding C++ event ( IScriptEvent )from Lua Event and queue it
      static bool QueueEvent( EventType eventType, LuaPlus::LuaObject eventData );
      // Generate a corresponding C++ event ( IScriptEvent )from Lua Event and trigger it
	   static bool TriggerEvent( EventType eventType, LuaPlus::LuaObject eventData );

   private:
      static ScriptEventListenerMgr* s_pScriptEventListenerMgr;
   };


//---------------------------------------------------------------------------------------------------------------------
// This is the C++ listener proxy for script event listeners.  It pairs a single event type with a Lua callback 
// function.  Note that this event can be defined in C++ or Lua.  It may also be sent from C++ or Lua.
// 
// The Lua callback function should take in a table with the event data.  The return value is ignored.
// function Callback(eventData)
//  

//
// And it is response for 
//---------------------------------------------------------------------------------------------------------------------
class ScriptEventListener
   {
   public:
      explicit ScriptEventListener( const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction );
      ~ScriptEventListener( void );
      EventListenerDelegate GetDelegate( void );
      // A Callback function, it is registered in EventManager by ScriptEventListenerMgr
      void ScriptEventDelegate( IEventPtr pEvent );

   private:
      EventType m_EventType;
      LuaPlus::LuaObject m_ScriptCallbackFunction;
   };

//---------------------------------------------------------------------------------------------------------------------
// This class manages the C++ ScriptListener objects needed for script event listeners.
// The ScriptEventDelegate function will be registered in EventManager, and the action of registration is made by this class
// In other words, ScriptEventImpListenerMgr class takes care of ScriptEventListener registration / unregistration in EventManager
// This class is manipulated by class InternalScriptExports though a static menber variable
//---------------------------------------------------------------------------------------------------------------------
class ScriptEventListenerMgr
   {
   public:
      ~ScriptEventListenerMgr( void );
      void AddListener( ScriptEventListener *pListener );
      void DestroyListener( ScriptEventListener *pListener );

   private:
      typedef std::set< ScriptEventListener * > ScriptEventListenerSet;
      ScriptEventListenerSet m_Listeners;

   };