#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ScriptExports.h
////////////////////////////////////////////////////////////////////////////////


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
   };