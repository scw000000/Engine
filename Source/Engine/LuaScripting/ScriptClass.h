#pragma once
/*!
 * \file ScriptClass.h
 * \date 2016/08/03 17:29
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
#include "LuaStateManager.h"

template< typename T > class ScriptClass
   {
   public:
      static void RegisterScriptClass( void );

   protected:
      // don't allow construction outside of this class
      explicit ScriptClass( void ) { };
      static LuaPlus::LuaObject CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass );
      static std::string GetLuaClassTableName( void );
      static LuaPlus::LuaObject GetLuaClassMetatable( void );
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) { return true; };
      virtual void VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) {};
     
   protected:
      LuaPlus::LuaObject m_LuaInstance;
   };

template< typename T > void ScriptClass<T>::RegisterScriptClass( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().CreateTable( GetLuaClassTableName().c_str() );
   metaTableObj.SetObject( "__index", metaTableObj );
   metaTableObj.SetObject( "base", metaTableObj );  // base refers to the parent class; ie the metatable
   metaTableObj.SetBoolean( "cpp", true );
   metaTableObj.RegisterDirect( "Create", &ScriptClass<T>::CreateFromScript );
   }

template< typename T > LuaPlus::LuaObject ScriptClass<T>::CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass )
   {
   // Note: The self parameter is not use in this function, but it allows us to be consistent when calling
   // Create().  The Lua version of this function needs self.
   ENG_LOG( "Script", std::string( "Creating instance of " ) + typeid( T ).name() );
   ScriptClass<T>* pObj = static_cast< ScriptClass<T>* >( ENG_NEW T );

   pObj->m_LuaInstance.AssignNewTable( LuaStateManager::GetSingleton().GetLuaState() );
   if( pObj->VBuildCppDataFromScript( originalSubClass, constructionData ) )
      {
      LuaPlus::LuaObject metaTableObj = GetLuaClassMetatable();
      pObj->m_LuaInstance.SetLightUserdata( "__object", pObj );
      pObj->m_LuaInstance.SetMetatable( metaTableObj );
      pObj->VRegisterExposedMemberFunctions( metaTableObj );
      }
   else
      {
      pObj->m_LuaInstance.AssignNil( LuaStateManager::GetSingleton().GetLuaState() );
      SAFE_DELETE( pObj );
      }

   return pObj->m_LuaInstance;
   }

template< typename T > std::string ScriptClass<T>::GetLuaClassTableName( void )
   {
   return std::string( "cppTable_" ) + typeid( T ).name();
   }

template< typename T > LuaPlus::LuaObject ScriptClass<T>::GetLuaClassMetatable( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( GetLuaClassTableName().c_str() );
   ENG_ASSERT( !metaTableObj.IsNil() && "This class is not registered yet" );
   return metaTableObj;
   }