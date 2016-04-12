#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptProcess.h
////////////////////////////////////////////////////////////////////////////////

#include "..\MainLoop\Process.h"
#include "LuaStateManager.h"

// This is C++ version of lua class definition
// In order to use this class. lua have to call 
// RegisterScriptClass and use class template in p.353
// to wrap around it
class ScriptProcess : public Process
   {
   public:
      // Create an metatable data in lua and expose menber functions to lua by calling RegisterScriptClassFunctions
      static void RegisterScriptClass( void );

   protected:
      virtual void VOnInit( void ) override;
      virtual void VOnUpdate( unsigned long deltaMs ) override;
      virtual void VOnSuccess( void ) override;
      virtual void VOnFail( void ) override;
      virtual void VOnAbort( void ) override;

   private:
      // Decide which script menber functions can be called in lua
      static void RegisterScriptClassFunctions( LuaPlus::LuaObject& metaTableObj );
      // This function is compatable with create function in p.353
      static LuaPlus::LuaObject CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass );
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ); // This function is C++ version of Create function

      bool ScriptIsAlive( void ) { return IsAlive(); }
      bool ScriptIsDead( void ) { return IsDead(); }
      bool ScriptIsPaused( void ) { return IsPaused(); }
      // Attach a script child from lua process instace
      void ScriptAttachChild( LuaPlus::LuaObject child );
      explicit ScriptProcess( void );
      static ScriptProcess *Create( const char* scriptName = NULL );
      static void Destroy( ScriptProcess* pObj );

   private:
      unsigned long m_Frequency; // lua update function calling frequency, ( how many Ms per call ) 
      unsigned long m_Time; // how long since last lua update function is executed

      LuaPlus::LuaObject m_ScriptInitFunction; // lua counter part of menber function
      LuaPlus::LuaObject m_ScriptUpdateFunction;
      LuaPlus::LuaObject m_ScriptSuccessFunction;
      LuaPlus::LuaObject m_ScriptFailFunction;
      LuaPlus::LuaObject m_ScriptAbortFunction;
      LuaPlus::LuaObject m_LuaInstance; // lua conterpart of this process instance

   };