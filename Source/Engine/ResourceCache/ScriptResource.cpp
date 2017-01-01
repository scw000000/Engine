/*!
 * \file ScriptResource.cpp
 * \date 2016/08/04 15:21
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
#include "ScriptResource.h"
#include "..\LuaScripting\LuaStateManager.h"

const char* SCRIPT_LOADER_PATTERNS[] = { "*.lua" };

ScriptResourceLoader::ScriptResourceLoader( void ) : ResourceLoader( std::vector< std::string >( SCRIPT_LOADER_PATTERNS, std::end( SCRIPT_LOADER_PATTERNS ) ) )
   {
   
   }

int ScriptResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   if( rawSize <= 0 )
      {
      return S_FALSE;
      }

   LuaStateManager::GetSingleton().VExecuteString( rawBuffer );
     
   return S_OK;
   }