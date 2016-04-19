////////////////////////////////////////////////////////////////////////////////
// Filename: MeshResouce.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MeshResource.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

shared_ptr<IResourceLoader> CreateObjMeshResourceLoader()
   {
   return shared_ptr<IResourceLoader>( ENG_NEW ObjMeshResourceLoader() );
   }

unsigned int MeshResourceLoader::VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
   {
   // the size will be calculated after import
   return 0;
   }

bool MeshResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   const char* p_Msg = NULL;
   const struct aiScene *p_Scene = aiImportFileFromMemory( rawBuffer, rawSize, aiProcessPreset_TargetRealtime_Quality, p_Msg );
   if( p_Scene )
      {
      ENG_ERROR( p_Msg );
      return false;
      }
   shared_ptr< MeshResourceExtraData > extra( ENG_NEW MeshResourceExtraData );
   handle->SetExtraData( extra );
   extra->m_pScene = p_Scene;
   struct aiMemoryInfo memInfo;
   aiGetMemoryRequirements( p_Scene, &memInfo );
   handle->SetSize( memInfo.total );
   return true;
   }



