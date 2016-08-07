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

template< typename T > void RegisterAbstractScriptClass( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().CreateTable( GetLuaClassTableName<T>().c_str() );
   metaTableObj.SetMetatable( metaTableObj );
   //metaTableObj.SetObject( "__index", metaTableObj );
   //metaTableObj.SetObject( "base", metaTableObj );  // base refers to the parent class; ie the metatable
   metaTableObj.SetBoolean( "cpp", true );
   }

template< typename T > void RegisterScriptClass( void )
   {
   if( !std::is_base_of< BaseScriptClass< T >, T >::value )
      {
      ENG_WARNING( "Register class failed, this class is not derived from BaseScriptClass" );
      return;
      }
   RegisterAbstractScriptClass< T >();
   auto metaTableObj = BaseScriptClass< T >::GetLuaClassMetatable();
   metaTableObj.RegisterDirect( "Create", &BaseScriptClass<T>::CreateFromScript );
   }

template< typename D, typename B > void RegisterScriptClass( void );
   //{
   //if( !std::is_base_of< BaseScriptClass< D >, D >::value || !std::is_base_of< B, D >::value )
   //   {
   //   ENG_WARNING( "Register class failed, this class is not derived from BaseScriptClass" );
   //   return;
   //   }
   //auto baseClassMetaTableObj = BaseScriptClass< B >::GetLuaClassMetatable();
   //if( baseClassMetaTableObj.IsNil() )
   //   {
   //   RegisterAbstractScriptClass< B >();
   //   baseClassMetaTableObj = BaseScriptClass< B >::GetLuaClassMetatable();
   //   }
   //RegisterScriptClass< D >();
   //auto derivedClassMetaTableObj = BaseScriptClass< D >::GetLuaClassMetatable();
   //derivedClassMetaTableObj.RegisterDirect( "Create", &BaseScriptClass< D >::CreateFromScript );

   //derivedClassMetaTableObj.SetObject( "__index", baseClassMetaTableObj );
   //derivedClassMetaTableObj.SetObject( "base", baseClassMetaTableObj );  // base refers to the parent class; ie the metatable
   //}

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
     // static void RegisterScriptClass( void );
     // static bool IsBaseClassOf( LuaPlus::LuaObject other );
    //  

   protected:
      // this class should be used when inherited only
      explicit BaseScriptClass( void ) { };
      static LuaPlus::LuaObject CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass );
      //static std::string GetLuaClassTableName( void );
      static LuaPlus::LuaObject GetLuaClassMetatable( void );
      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) { return true; };
      virtual void VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) {};
     
   protected:
      LuaPlus::LuaObject m_LuaInstance;
   };

//template< typename T > void BaseScriptClass<T>::RegisterScriptClass( void )
//   {
//   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().CreateTable( GetLuaClassTableName().c_str() );
//   metaTableObj.SetObject( "__index", metaTableObj );
//   metaTableObj.SetObject( "base", metaTableObj );  // base refers to the parent class; ie the metatable
//   metaTableObj.SetBoolean( "cpp", true );
//   metaTableObj.RegisterDirect( "Create", &BaseScriptClass<T>::CreateFromScript );
//   metaTableObj.RegisterDirect( "IsBaseClassOf", &BaseScriptClass<T>::IsBaseClassOf );
//   }

template< typename T > LuaPlus::LuaObject BaseScriptClass<T>::CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass )
   {
   // Note: The self parameter is not use in this function, but it allows us to be consistent when calling
   // Create().  The Lua version of this function needs self.
   ENG_LOG( "Script", std::string( "Creating instance of " ) + typeid( T ).name() );
   auto pObj = ENG_NEW T;
   //BaseScriptClass<T>* pObj = static_cast< BaseScriptClass<T>* >( orig );

   pObj->m_LuaInstance.AssignNewTable( LuaStateManager::GetSingleton().GetLuaState() );
   if( pObj->VBuildCppDataFromScript( originalSubClass, constructionData ) )
      {
      LuaPlus::LuaObject metaTableObj = GetLuaClassMetatable();
      pObj->m_LuaInstance.SetLightUserdata( "__object", pObj );
      pObj->m_LuaInstance.SetMetatable( pObj->m_LuaInstance );
      pObj->m_LuaInstance.SetObject( "__index", metaTableObj );
      pObj->m_LuaInstance.SetObject( "base", metaTableObj );
      pObj->VRegisterExposedMemberFunctions( metaTableObj );
      }
   else
      {
      pObj->m_LuaInstance.AssignNil( LuaStateManager::GetSingleton().GetLuaState() );
      SAFE_DELETE( pObj );
      ENG_WARNING( "Create C++ class from lua obj failed" );
      }

   return pObj->m_LuaInstance;
   }

//template< typename T > std::string BaseScriptClass<T>::GetLuaClassTableName( void )
//   {
//   std::string className = typeid( T ).name();
//   className = className.substr( className.find_first_of( " " ) + 1 );
//   return std::string( "cppClassTable_" ) + className;
//   }

template< typename T > LuaPlus::LuaObject BaseScriptClass<T>::GetLuaClassMetatable( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( GetLuaClassTableName< T >().c_str() );
   //ENG_ASSERT( !metaTableObj.IsNil() && "This class is not registered yet" );
   return metaTableObj;
   }

//template< typename T > bool BaseScriptClass<T>::IsBaseClassOf( LuaPlus::LuaObject other )
//   {
//   auto otherMetatableObj = other.Lookup( "__index" );
//   auto selfMetableTableObj = GetLuaClassMetatable();
//   while( !otherMetatableObj.IsNil() )
//      {
//      if( otherMetatableObj == selfMetableTableObj )
//         {
//         return true;
//         }
//      otherMetatableObj = otherMetatableObj.Lookup( "__index" );
//      }
//   return false;
//   }

template< typename D, typename B > void RegisterScriptClass( void )
   {
   if( !std::is_base_of< BaseScriptClass< D >, D >::value || !std::is_base_of< B, D >::value )
      {
      ENG_WARNING( "Register class failed, this class is not derived from BaseScriptClass" );
      return;
      }
   auto baseClassMetaTableObj = BaseScriptClass< B >::GetLuaClassMetatable();
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