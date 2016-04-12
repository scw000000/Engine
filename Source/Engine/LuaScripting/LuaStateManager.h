#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: LuaStateManager.h
////////////////////////////////////////////////////////////////////////////////

#include "LuaPlus.h"

// TODO: add all virutal function overriding with the keyword "override"
class LuaStateManager : public IScriptManager
   {
   public:
      static bool Create( void );
      static void Destroy( void );
      static LuaStateManager* GetSingleton( void ){ return s_pLuaStateMgr; }

      virtual bool VInit( void ) override;
      // demand lua to execute a lua file
      virtual void VExecuteFile( const char* resource ) override;
      // demand lua to execute a sting
      virtual void VExecuteString( const char*str ) override;

      LuaPlus::LuaObject GetGlobalVars( void );
      LuaPlus::LuaState* GetLuaState( void ) const;
      // Build a series of connected tables from global variables based on pathStr
      // EX: this.is.a.path -> global->this->is->a->path
      LuaPlus::LuaObject CreatePath( const char *pathStr, bool toIgnoreLastElement = false );
      // Create a table from m_pLuaState and restore x y z value in vec into this table
      void ConvertVec3ToTable( const Vec3& vec, LuaPlus::LuaObject& outLuaTable ) const;

   private:
      void SetError( int errorNum );
      void ClearStack( void );
      
      explicit LuaStateManager( void );
      virtual ~LuaStateManager( void );

   private:
      static LuaStateManager* s_pLuaStateMgr;
      LuaPlus::LuaState* m_pLuaState;
      std::string m_LastError;


   };