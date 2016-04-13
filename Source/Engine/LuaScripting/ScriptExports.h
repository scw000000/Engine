#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptExports.h
////////////////////////////////////////////////////////////////////////////////
#include "FastDelegate.h"
#include "LuaStateManager.h"
#include "..\Event\Events.h"
#include "..\Event\EventManager.h"
#include "ScriptEvent.h"

namespace ScriptExports
   {
   // decide which static functions in InternalScriptExports can be exposed to lua
   void Register( void );
   void Unregister( void );
   }

// This class contains some static functions for lua to call
class InternalScriptExports
   {
   public:
      static bool Init( void );
      static void Destroy( void );
      static bool LoadAndExecuteScriptResource( const char *scriptRes );
      // Generate a corresponding C++ event ( ScriptEvent )from Lua Event and queue it
      static bool QueueEvent( EventType eventType, LuaPlus::LuaObject eventData );
      // Generate a corresponding C++ event ( ScriptEvent )from Lua Event and triggert it
	   static bool TriggerEvent( EventType eventType, LuaPlus::LuaObject eventData );

   private:
	   static shared_ptr<ScriptEvent> BuildEvent(EventType eventType, LuaPlus::LuaObject& eventData);
   };

class ScriptEventListener
   {
   public:
      explicit ScriptEventListener( const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction );
      ~ScriptEventListener( void );
      EventListenerDelegate GetDelegate( void )
         {
         return MakeDelegate( this, &ScriptEventListener::ScriptEventDelegate );
         }
      void ScriptEventDelegate( IEventDataPtr pEventPtr );

   private:
      EventType m_EventType;
      LuaPlus::LuaObject m_ScriptCallBackFunction;
   };