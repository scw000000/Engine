/*!
 * \file ScriptProcess.cpp
 * \date 2016/06/27 20:13
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

#include "EngineStd.h"
#include "ScriptProcess.h"

const char* SCRIPT_PROCESS_NAME = "ScriptProcess";

void ScriptProcess::RegisterScriptClass( void )
   {
   LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().CreateTable( SCRIPT_PROCESS_NAME );
   metaTableObj.SetObject( "__index", metaTableObj ); // set itself as index table
   metaTableObj.SetObject( "base", metaTableObj );
   metaTableObj.SetBoolean( "cpp", true );
   RegisterScriptClassFunctions( metaTableObj );
   metaTableObj.RegisterDirect( "Create", &ScriptProcess::CreateFromScript ); // Let user can call create function to generate instance from lua 
   }

void ScriptProcess::VOnInit( void ) 
   {
   Process::VOnInit();
   if( !m_ScriptInitFunction.IsNil() )
      {
      LuaPlus::LuaFunction< void* > func( m_ScriptInitFunction );
      func( m_LuaInstance );
      }
   if ( !m_ScriptInitFunction.IsFunction() )
      {
		Fail();
      }
   }

void ScriptProcess::VOnUpdate( float deltaSeconds )
   {
   m_Time += deltaSeconds;
   if( m_Time >= m_Frequency )
      {
      LuaPlus::LuaFunction< void* > func( m_ScriptUpdateFunction );
      func( m_LuaInstance, m_Time );
      }
   }  

void ScriptProcess::VOnSuccess( void ) 
   {
   if ( !m_ScriptSuccessFunction.IsNil() )
	   {
		LuaPlus::LuaFunction< void* > func(m_ScriptSuccessFunction);
		func( m_LuaInstance );
	   }
   }

void ScriptProcess::VOnFail( void ) 
   {
   if (!m_ScriptFailFunction.IsNil())
	   {
		LuaPlus::LuaFunction< void* > func(m_ScriptFailFunction);
		func( m_LuaInstance );
	   }
   }

void ScriptProcess::VOnAbort( void ) 
   {
   if (!m_ScriptAbortFunction.IsNil())
	   {
		LuaPlus::LuaFunction< void* > func(m_ScriptAbortFunction);
		func( m_LuaInstance );
	   }
   }

void ScriptProcess::RegisterScriptClassFunctions( LuaPlus::LuaObject& metaTableObj )
   {
   metaTableObj.RegisterObjectDirect( "Succeed", (Process*)0, &Process::Succeed );
   metaTableObj.RegisterObjectDirect( "Fail", (Process*)0, &Process::Fail );
   metaTableObj.RegisterObjectDirect( "Pause", (Process*)0, &Process::Pause );
   metaTableObj.RegisterObjectDirect( "UnPause", (Process*)0, &Process::UnPause );

   // IsAlive & is IsDead are const member functnion, lua doesn't know how to deal with it, so we 
   // Use a simple non-const menber function wrapper
   metaTableObj.RegisterObjectDirect( "IsAlive", (ScriptProcess*)0, &ScriptProcess::ScriptIsAlive );
   metaTableObj.RegisterObjectDirect( "IsDead", (ScriptProcess*)0, &ScriptProcess::ScriptIsDead );
   metaTableObj.RegisterObjectDirect( "IsPaused", (ScriptProcess*)0, &ScriptProcess::ScriptIsPaused );
   metaTableObj.RegisterObjectDirect( "AttachChild", (ScriptProcess*)0, &ScriptProcess::ScriptAttachChild );
   }

LuaPlus::LuaObject ScriptProcess::CreateFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass )
   {
   ScriptProcess *pObj = ENG_NEW ScriptProcess;
   pObj->m_LuaInstance.AssignNewTable( LuaStateManager::GetSingleton().GetLuaState() );  
   if( pObj->VBuildCppDataFromScript( originalSubClass, constructionData ) )
      {
      // Find class defination (table) in global variables 
      LuaPlus::LuaObject metaTableObj = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( SCRIPT_PROCESS_NAME );
      ENG_ASSERT( !metaTableObj.IsNil() );
      // Set C++ pointer in local tabale, because this variable is belong to instance, to class 
      pObj->m_LuaInstance.SetLightUserdata( "__object", pObj );
      // But still set metaTable to Class definition
      pObj->m_LuaInstance.SetMetatable( metaTableObj );
      }
   else // create fail, delete all data
      {
      pObj->m_LuaInstance.AssignNil( LuaStateManager::GetSingleton().GetLuaState() );
      SAFE_DELETE( pObj );
      }
   return pObj->m_LuaInstance;
   }


bool ScriptProcess::VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData )
   {
   if( scriptClass.IsTable() )
      {
      LuaPlus::LuaObject temp = scriptClass.GetByName( "OnInit" );
      if( temp.IsFunction() )
         {
         m_ScriptInitFunction = temp;
         }
      temp = scriptClass.GetByName( "OnUpdate" );
      if( temp.IsFunction() )
         {
         m_ScriptUpdateFunction = temp;
         }
      else
         {
         ENG_ERROR( "No OnUpdate() found in script process; type = " + std::string( temp.TypeName() ) );
         return false;    
         }    

      temp = scriptClass.GetByName( "OnSuccess" );
      if( temp.IsFunction() )
         {
         m_ScriptSuccessFunction = temp;
         }

      temp = scriptClass.GetByName( "OnFail" );
      if( temp.IsFunction() )   
         {
         m_ScriptFailFunction = temp;
         }

      temp = scriptClass.GetByName( "OnAbort" );
         {
         m_ScriptAbortFunction = temp;
         }

      }
   else
      {
      ENG_ERROR( "ScriptClass is not a talbe in ScriptProcess::BuildCppDataFromScript()" );
      return false;
      }
   // loading initial value for this instance
   if( constructionData.IsTable() )
      {
      for( LuaPlus::LuaTableIterator constructionDataIt( constructionData ); constructionDataIt; constructionDataIt.Next() )
         {
         const char* key = constructionDataIt.GetKey().GetString();
         LuaPlus::LuaObject val = constructionDataIt.GetValue();
         if( strcmp( key, "frequency" ) == 0 && val.IsInteger() ) // this is a menber variable which is also in c++ class and we wnat to know, so we have to catch and restore it
            {
            m_Frequency = val.GetInteger();
            }
         else
            {
            m_LuaInstance.SetObject( key, val );
            }
         }
      }
   return true;
   }

void ScriptProcess::ScriptAttachChild( LuaPlus::LuaObject child )
   {
   if( child.IsTable() )
      {
      LuaPlus::LuaObject obj = child.GetByName( "__object" );
      if( !obj.IsNil() )
         {
         shared_ptr<Process> pProcess( static_cast< Process* >( obj.GetLightUserdata() ) );
         ENG_ASSERT( pProcess );
         AttachChild( pProcess );
         }
      else // child is a table, but it doesn't have C++ pointer
         {
         ENG_ERROR( "Attempting to attach child with no valid object" );
         }
      }
   else // child is not table, cannot retrieve C++ pointer back
      {
      ENG_ERROR( "Invalid object type passed into \
                 ScriptProcess::ScriptAttachChild(); type = " + std::string( child.TypeName() ) );
      }
   }

ScriptProcess::ScriptProcess( void )
   {
   LuaPlus::LuaState* pLuaState = LuaStateManager::GetSingleton().GetLuaState();

	m_Frequency = 0;
	m_Time = 0;
	m_ScriptInitFunction.AssignNil(pLuaState);
	m_ScriptUpdateFunction.AssignNil(pLuaState);
	m_ScriptSuccessFunction.AssignNil(pLuaState);
	m_ScriptFailFunction.AssignNil(pLuaState);
	m_ScriptAbortFunction.AssignNil(pLuaState);
   }
