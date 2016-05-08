#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriprEvent.h
////////////////////////////////////////////////////////////////////////////////
#include "..\Event\Events.h"
#include "LuaPlus.h"

class IScripiEvent : public IEvent
   {
   public:
      virtual LuaPlus::LuaObject GetLuaEventData( void ) = 0;
      
      virtual bool SetEventData( LuaPlus::LuaObject eventData ) = 0;

   protected:
      virtual void VBuildScriptEventData( void ) = 0;
      
      virtual bool VBuildEventFromScript( void ) = 0;
   
   };


//---------------------------------------------------------------------------------------------------------------------
// Script event base class.  This class is meant to be subclassed by any event that can be sent or received by the
// script.  Note that these events are not limited to script and can be received just fine by C++ listeners.
// Furthermore, since the Script data isn't built unless being received by a script listener, there's no worry about 
// performance.
//---------------------------------------------------------------------------------------------------------------------
class ScriptEventImp : public IScripiEvent
   {
   public:
      ScriptEventImp( void ) { m_IsEventDataValid = false; }
      // set m_LuaEventData and copy C++ menber variable to lua event data
      // This function is called by ScriptEventImpListener::ScriptEventImpDelegate(IEventPtr pEvent)  
      // ( when we want to call a lua callback function )
      virtual LuaPlus::LuaObject GetLuaEventData( void ) override;
      // set m_LuaEventData from eventData and copy it to C++ menber
      // This function is called by InternalScriptExports::BuildEvent(EventType eventType, LuaPlus::LuaObject& eventData) 
      // ( when lua queue or trigger an event, we have to make a C++ script event )
      virtual bool SetEventData( LuaPlus::LuaObject eventData ) override;

   protected:
      // This function is for the event being sent from C++ to lua. Called by GetEventData
      // What this function should do is copy specific C++ event data, such as damage value from C++ menber variable to lua
      virtual void VBuildScriptEventData( void ) override;
      // This function is for the event being sent from lua to C++. Called by SetEventData
      // What this function should do is copy specific lua event data, such as damage value from lua to C++ menber variable
      virtual bool VBuildEventFromScript( void ) override { return true; }

   protected:
      LuaPlus::LuaObject m_LuaEventData; // the counterpart lua event data

   private:
      bool m_IsEventDataValid;
   };

// Using Curiously recurring template pattern (CRTP) to prevent declaring GUID mulit times
template <typename T>class BaseScriptEvent : public ScriptEventImp 
   {
   public:
   explicit BaseScriptEvent( const float timeStamp = 0.0f ) : m_TimeStamp( timeStamp )
      {
      };

   virtual ~BaseScriptEvent( void ) { }

   virtual const EventType& VGetEventType( void ) const override { return s_EventType;};

   virtual float VGetTimeStamp( void ) const override { return m_TimeStamp; }

   virtual const char* GetName( void ) const override { return s_pName; }

public:
   // GUID of this event
   const static EventType  s_EventType;
   const static char*      s_pName;

protected:


private:

   const float m_TimeStamp;
   };