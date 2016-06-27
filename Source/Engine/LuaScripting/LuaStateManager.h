#pragma once
/*!
 * \file LuaStateManager.h
 * \date 2016/06/27 8:05
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

#include "LuaPlus.h"

// TODO: add all virutal function overriding with the keyword "override"
class LuaStateManager : public IScriptManager
   {
   public:
      static LuaStateManager& GetSingleton( void );
      bool Init( void );
      void Destroy( void );
      
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
      void ClearLuaState( void );

      explicit LuaStateManager( void );
      virtual ~LuaStateManager( void );

   private:
      LuaPlus::LuaState* m_pLuaState;
      std::string m_LastError;
   };