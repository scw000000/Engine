/*!
 * \file MeshResource.cpp
 * \date 2016/06/25 16:41
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
#include "MeshResource.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

const char* MESH_LOADER_PATTERNS[] = { "*.obj", "*.fbx" };

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
   const struct aiScene *p_AiScene = aiImportFileFromMemory( rawBuffer, 
                                                           rawSize, 
                                                           aiProcessPreset_TargetRealtime_Quality | aiProcess_Triangulate | aiProcess_SortByPType, 
                                                           p_Msg );
   if( !p_AiScene )
      {
      ENG_ERROR( p_Msg );
      return false;
      }
   shared_ptr< MeshResourceExtraData > extra( ENG_NEW MeshResourceExtraData );
   handle->SetExtraData( extra );
   extra->m_pScene = p_AiScene;
   extra->m_Radius = 0;

   for( unsigned int meshIdx = 0; meshIdx < p_AiScene->mNumMeshes; ++meshIdx )
      {
      auto pMesh = p_AiScene->mMeshes[ meshIdx ];
      extra->m_NumVertices += pMesh->mNumVertices;
      for( unsigned int vertexId = 0; vertexId < pMesh->mNumVertices; ++vertexId )
         {    
         auto curSquareLength = pMesh->mVertices[ vertexId ].SquareLength();
         extra->m_Radius = std::max( extra->m_Radius, curSquareLength );
         }

      for( unsigned int faceId = 0; faceId < pMesh->mNumFaces; ++faceId )
         {
         extra->m_NumVertexIndex += pMesh->mFaces[ faceId ].mNumIndices;
         }
      }

   extra->m_Radius = std::sqrt( extra->m_Radius );
   struct aiMemoryInfo memInfo;
   aiGetMemoryRequirements( p_AiScene, &memInfo );
   handle->SetSize( memInfo.total );
   return true;
   }

const aiScene* MeshResourceLoader::LoadAndReturnScene( const Resource& resource )
   {
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( resource );  	
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );
   return pMeshExtra->m_pScene;
   }



