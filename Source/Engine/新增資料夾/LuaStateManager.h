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
      virtual void VExecuteFile( const char* resource ) override;
      virtual void VExecuteString( const char*str ) override;

      LuaPlus::LuaObject GetGlobalVars( void );
      LuaPlus::LuaState* GetLuaState( void ) const;

      LuaPlus::LuaObject CreatePath( const char *pathStr, bool toIgnoreLastElement = false );
      void ConvertVec3ToTable( const Vec3& vec, LuaPlus::LuaObject& luaTable, Vec3& outVec3 ) const;

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