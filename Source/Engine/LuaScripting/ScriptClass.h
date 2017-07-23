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

template< typename T > std::string GetLuaClassTableName( void )
   {
   std::string className = typeid( T ).name();
   className = className.substr( className.find_first_of( " " ) + 1 );
   return std::string( "cppClassTable_" ) + className;
   }

template< typename T > bool IsBaseClassOf( LuaPlus::LuaObject other )
   {
   if( !other.IsTable() )
      {
      return false;
      }
   auto selfMetableTableObj = BaseScriptClass< T >::GetLuaClassMetatable();
   auto otherMetatableObj = other.Lookup( "__index" );
   while( !otherMetatableObj.IsNil() )
      {
      if( selfMetableTableObj == otherMetatableObj )
         {
         return true;
         }
      otherMetatableObj = otherMetatableObj.Lookup( "__index" );
      }
   return false;
   }

// Create an empty metaTable for itself
template< typename T > void RegisterAbstractScriptClass( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().CreateTable( GetLuaClassTableName<T>().c_str() );
   metaTableObj.SetMetatable( metaTableObj );
   //metaTableObj.SetObject( "__index", metaTableObj );
   //metaTableObj.SetObject( "base", metaTableObj );  // base refers to the parent class; ie the metatable
   metaTableObj.SetBoolean( "cpp", true );
   }

// This class has no base parameter
template< typename T > void RegisterScriptClass( void )
   {
   if( !std::is_base_of< BaseScriptClass< T >, T >::value )
      {
      ENG_ASSERT( "Register class failed, this class is not derived from BaseScriptClass" );
      return;
      }
   RegisterAbstractScriptClass< T >();
   auto metaTableObj = BaseScriptClass< T >::GetLuaClassMetatable();
   metaTableObj.RegisterDirect( "Create", &BaseScriptClass<T>::CreateFromScript );
   }

template< typename T > T* GetObjUserDataPtr( const LuaPlus::LuaObject& luaObj )
   {
   T* pObj = ( T* ) luaObj.Lookup( "__object" ).GetLightUserdata();
   return pObj;
   }

template< typename T > class BaseScriptClass
   {
   friend void RegisterScriptClass< T >( void );
   friend bool IsBaseClassOf< T >( LuaPlus::LuaObject other );
   template< typename D, typename B > friend void RegisterScriptClass( void );
   public:

   protected:
      // this class should be used when inherited only
      explicit BaseScriptClass( void ) { };
      static LuaPlus::LuaObject CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass );
      // Get mapping lua class object in C++
      static LuaPlus::LuaObject GetLuaClassMetatable( void );
      // This function should be overrided in subclass, the purpose of this function is to 
      // iterate all of the constructionData in lua and set it up in CPP class
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) { return true; };
      virtual void VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) {};
     
   protected:
      LuaPlus::LuaObject m_LuaInstance;
   };

template< typename T > LuaPlus::LuaObject BaseScriptClass<T>::CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass )
   {
   // Note: The self parameter is not use in this function, but it allows us to be consistent when calling
   // Create().  The Lua version of this function needs self.
   ENG_LOG( "Script", std::string( "Creating instance of " ) + typeid( T ).name() );
   auto pCPPObj = ENG_NEW T;
   //BaseScriptClass<T>* pObj = static_cast< BaseScriptClass<T>* >( orig );

   // Link lua instance to currently lua state
   pCPPObj->m_LuaInstance.AssignNewTable( LuaStateManager::GetSingleton().GetLuaState() );
   if( pCPPObj->VBuildCppDataFromScript( originalSubClass, constructionData ) )
      {
      // metaTableObj represents the Lua class table
      LuaPlus::LuaObject metaTableObj = GetLuaClassMetatable();
      // Point light user data to itself
      pCPPObj->m_LuaInstance.SetLightUserdata( "__object", pCPPObj );
      pCPPObj->m_LuaInstance.SetMetatable( pCPPObj->m_LuaInstance );
      // If you cannot find anything in current lua instance, it will look up __index in metaTable, which is itself
      // and the index will be the lua class metatable
      pCPPObj->m_LuaInstance.SetObject( "__index", metaTableObj );
      pCPPObj->m_LuaInstance.SetObject( "base", metaTableObj );
      // this function should be implemented by class implementation
      pCPPObj->VRegisterExposedMemberFunctions( metaTableObj );
      metaTableObj.SetBoolean( "cpp", true );
      // setup variables
      for( LuaPlus::LuaTableIterator childNodesIt( constructionData ); childNodesIt; childNodesIt.Next() )
         {
         pCPPObj->m_LuaInstance.SetObject( childNodesIt.GetKey(), childNodesIt.GetValue() );
         }
      }
   else
      {
      pCPPObj->m_LuaInstance.AssignNil( LuaStateManager::GetSingleton().GetLuaState() );
      SAFE_DELETE( pCPPObj );
      ENG_WARNING( "Create C++ class from lua obj failed" );
      }

   return pCPPObj->m_LuaInstance;
   }

template< typename T > LuaPlus::LuaObject BaseScriptClass<T>::GetLuaClassMetatable( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( GetLuaClassTableName< T >().c_str() );
   //ENG_ASSERT( !metaTableObj.IsNil() && "This class is not registered yet" );
   return metaTableObj;
   }

// To specify inherit relation between C++ classes, D = derived class, B = base class
template< typename D, typename B > void RegisterScriptClass( void )
   {
   if( !std::is_base_of< BaseScriptClass< D >, D >::value || !std::is_base_of< B, D >::value )
      {
      ENG_ERROR( "Register class failed, this class is not derived from BaseScriptClass" );
      return;
      }
   auto baseClassMetaTableObj = BaseScriptClass< B >::GetLuaClassMetatable();
   // Register base class first 
   if( baseClassMetaTableObj.IsNil() )
      {
      RegisterAbstractScriptClass< B >();
      baseClassMetaTableObj = BaseScriptClass< B >::GetLuaClassMetatable();
      }
   RegisterScriptClass< D >();
   auto derivedClassMetaTableObj = BaseScriptClass< D >::GetLuaClassMetatable();
   derivedClassMetaTableObj.RegisterDirect( "Create", &BaseScriptClass< D >::CreateFromScript );

   derivedClassMetaTableObj.SetObject( "__index", baseClassMetaTableObj );
   derivedClassMetaTableObj.SetObject( "base", baseClassMetaTableObj );  // base refers to the parent class; ie the metatable
   }