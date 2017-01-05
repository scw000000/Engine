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
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

const char* MESH_LOADER_PATTERNS[] = { "*.obj", "*.fbx", "*.3ds", "*.fbx" };

BoneData::BoneData( const aiMatrix4x4& offset, BoneId id )
   {
   m_BoneOffset = offset;  m_BoneId = id;
   }

extern Mat4x4 aiMat4x4ToMat4( aiMatrix4x4 aiMat44 )
   {
   Mat4x4 toWorld;
   toWorld.SetRow( 0, Vec4( aiMat44.a1, aiMat44.b1, aiMat44.c1, aiMat44.d1 ) );
   toWorld.SetRow( 1, Vec4( aiMat44.a2, aiMat44.b2, aiMat44.c2, aiMat44.d2 ) );
   toWorld.SetRow( 2, Vec4( aiMat44.a3, aiMat44.b3, aiMat44.c3, aiMat44.d3 ) );
   toWorld.SetRow( 3, Vec4( aiMat44.a4, aiMat44.b4, aiMat44.c4, aiMat44.d4 ) );

   return toWorld;
   }

MeshResourceExtraData::MeshResourceExtraData( unsigned int meshNum ) 
   : m_pScene( NULL )
   {
   m_NumVertices = 0; 
   m_NumVertexIndex = 0;
   m_NumBones = 0; 
   }

void MeshResourceExtraData::LoadBones( void )
   {
   BoneId currentBoneId = 0;
   for( unsigned int meshIdx = 0; meshIdx < m_pScene->mNumMeshes; ++meshIdx )
      {
      auto pMesh = m_pScene->mMeshes[ meshIdx ];
      for( unsigned int boneIdx = 0; boneIdx < pMesh->mNumBones; ++boneIdx )
         {
         auto pBone = pMesh->mBones[ boneIdx ];
         std::string boneName = pBone->mName.C_Str();
         if( m_BoneMappingData.find( boneName ) == m_BoneMappingData.end() )
            {
            m_BoneMappingData[ boneName ] = BoneData( pBone->mOffsetMatrix, currentBoneId );
            ++currentBoneId;
            }
         }
      }
   m_NumBones = currentBoneId;
   ENG_ASSERT( m_NumBones <= MAXIMUM_BONES_PER_ACTOR );
   }

aiAnimation* MeshResourceExtraData::FindAnimation( const std::string& animationName ) const
   {
   for( unsigned int animIdx = 0; animIdx < m_pScene->mNumAnimations; ++animIdx )
      {
      if( !std::strcmp( animationName.c_str(), m_pScene->mAnimations[ animIdx ]->mName.C_Str() ) )
         {
         return m_pScene->mAnimations[ animIdx ];
         }
      }
   return NULL;
   }

aiAnimation* MeshResourceExtraData::FindAnimation( unsigned int animIdx ) const
   {
   ENG_ASSERT( animIdx < m_pScene->mNumAnimations );
   return m_pScene->mAnimations[ animIdx ];
   }

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

int MeshResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
   {
   const char* p_Msg = NULL;
   //aiProcessPreset_TargetRealtime_Quality // aiProcess_JoinIdenticalVertices
   const struct aiScene *pAiScene = aiImportFileFromMemory( rawBuffer, 
                                                           rawSize, 
                                                           aiProcess_Triangulate | aiProcess_LimitBoneWeights| aiProcessPreset_TargetRealtime_Fast /*| aiProcess_SortByPType*/,
                                                           p_Msg );
   if( !pAiScene )
      {
      ENG_ERROR( p_Msg );
      return false;
      }
   shared_ptr< MeshResourceExtraData > extra( ENG_NEW MeshResourceExtraData( pAiScene->mNumMeshes ) );
   handle->SetExtraData( extra );
   extra->m_pScene = pAiScene;

   //extra->m_Radius = 0;
   //for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
   //   {
   //   auto pMesh = pAiScene->mMeshes[ meshIdx ];
   //   extra->m_NumVertices += pMesh->mNumVertices;
   //   for( unsigned int vertexId = 0; vertexId < pMesh->mNumVertices; ++vertexId )
   //      {    
   //      auto curSquareLength = pMesh->mVertices[ vertexId ].SquareLength();
   //      extra->m_Radius[ meshIdx ] = std::max( extra->m_Radius[ meshIdx ], curSquareLength );
   //      }
   //   extra->m_Radius[ meshIdx ] = std::sqrt( extra->m_Radius[ meshIdx ] );
   ////   for( unsigned int faceId = 0; faceId < pMesh->mNumFaces; ++faceId )
   ////      {
   ////      extra->m_NumVertexIndex += pMesh->mFaces[ faceId ].mNumIndices;
   ////      }
   //   }
   extra->LoadBones();
   extra->m_GlobalInverseTransform = pAiScene->mRootNode->mTransformation;
   extra->m_GlobalInverseTransform.Inverse();

  // extra->m_Radius = std::sqrt( extra->m_Radius );
   struct aiMemoryInfo memInfo;
   aiGetMemoryRequirements( pAiScene, &memInfo );
   handle->SetSize( memInfo.total );
   return S_OK;
   }

const aiScene* MeshResourceLoader::LoadAndReturnScene( const Resource& resource )
   {
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( resource );  	
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );
   return pMeshExtra->m_pScene;
   }



