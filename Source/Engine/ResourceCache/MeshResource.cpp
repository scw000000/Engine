////////////////////////////////////////////////////////////////////////////////
// Filename: MeshResouce.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MeshResource.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

const char* MESH_LOADER_PATTERNS[] = { "*.obj" };

shared_ptr<IResourceLoader> CreateMeshResourceLoader()
   {
   return shared_ptr<IResourceLoader>( ENG_NEW MeshResourceLoader() );
   }

MeshResourceLoader::MeshResourceLoader( void ) : ResourceLoader( std::vector< std::string >( MESH_LOADER_PATTERNS, std::end( MESH_LOADER_PATTERNS ) ) )
   {
   
   }

unsigned int MeshResourceLoader::VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
   {
   // the size will be calculated after import
   return 0;
   }

bool MeshResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   const char* p_Msg = NULL;
   //aiProcessPreset_TargetRealtime_Quality // aiProcess_JoinIdenticalVertices
   const struct aiScene *p_Scene = aiImportFileFromMemory( rawBuffer, 
                                                           rawSize, 
                                                           aiProcess_Triangulate | aiProcess_SortByPType, 
                                                           p_Msg );
   if( !p_Scene )
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

const aiScene* MeshResourceLoader::LoadAndReturnScene( Resource& resource )
   {
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( &resource );  	
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );
   return pMeshExtra->m_pScene;
   }



