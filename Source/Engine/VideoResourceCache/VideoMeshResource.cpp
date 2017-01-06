/*!
 * \file VideoMeshResource.cpp
 * \date 2017/01/02 14:31
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "VideoMeshResource.h"
#include "..\Graphics\Renderer\RenderManager.h"
#include "VideoTextureResource.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

const std::vector< std::string > MESH_LOADER_PATTERNS( { "*.obj", "*.fbx", "*.3ds", "*.fbx", "*.blend" } );

VideoMeshResourceExtraData::VideoMeshResourceExtraData( unsigned int meshCount ) :
   m_BufferObjects( meshCount ),
   m_MeshCount( meshCount ),
   m_Radius( meshCount, 0.f )
   {
   
   }

VideoMeshResourceExtraData::~VideoMeshResourceExtraData( void )
   {
   if( m_BufferObjects.size() )
      {
      unsigned int arrLen = ENG_ARRAY_SIZE_IN_ELEMENTS( m_BufferObjects[ 0 ] );
         for( auto arry : m_BufferObjects )
            {
            glDeleteBuffers( arrLen, arry );
            }
      }
   }

VideoMeshResourceLoader::VideoMeshResourceLoader( void ) : VideoResourceLoader( MESH_LOADER_PATTERNS )
   {
   
   }

int VideoMeshResourceLoader::VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle )
   {
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( handle->GetExtraData() );
   auto pAiScene = pMeshExtra->m_pScene;
  // auto pAiScene = aiImportFile( "F:\\Workspace\\Engine\\Game\\Assets\\Art\\sponza\\sponza.obj",
  //                                         aiProcess_Triangulate | aiProcess_LimitBoneWeights | aiProcessPreset_TargetRealtime_Fast /*| aiProcess_SortByPType*/ );
  // ENG_ASSERT( pAiScene );
   if( pAiScene->mNumMeshes == 0u )
      {
      ENG_ASSERT( 0 && "No mesh inside this file" );
      return S_FALSE;
      }
   VideoMeshResourceExtraData* pData = ENG_NEW VideoMeshResourceExtraData( pAiScene->mNumMeshes );
   ENG_ASSERT( pData );
   shared_ptr< VideoMeshResourceExtraData > pExtraData( pData );

   // load Material
 //  pData->m_MaterialTexHandles.resize( pAiScene->mNumMaterials, std::vector<shared_ptr< VideoResourceHandle > >( VideoMeshResourceExtraData::MateralTexture_Num ) );
 //  std::string filePath = handle->GetResource().GetPath();
 //  for( unsigned int i = 0; i < pAiScene->mNumMaterials; ++i )
 //     {
 //     auto pAiMaterial = pAiScene->mMaterials[ i ];
 //     pData->m_MaterialTexHandles[ i ][ VideoMeshResourceExtraData::MateralTexture_Diffuse ] = 
 //        LoadTexture( pAiMaterial, aiTextureType_DIFFUSE, filePath );  // used in sponza
 // //    LoadTexture( pAiMaterial, aiTextureType_SPECULAR );
 ////     LoadTexture( pAiMaterial, aiTextureType_AMBIENT );
 ////     LoadTexture( pAiMaterial, aiTextureType_EMISSIVE );
 // //    LoadTexture( pAiMaterial, aiTextureType_HEIGHT );
 //     pData->m_MaterialTexHandles[ i ][ VideoMeshResourceExtraData::MateralTexture_Normal ] =
 //        LoadTexture( pAiMaterial, aiTextureType_NORMALS, filePath ); // used in sponza
 // //    LoadTexture( pAiMaterial, aiTextureType_SHININESS );
 ////     LoadTexture( pAiMaterial, aiTextureType_OPACITY );
 ////     LoadTexture( pAiMaterial, aiTextureType_DISPLACEMENT );
 // //    LoadTexture( pAiMaterial, aiTextureType_LIGHTMAP );
 // //    LoadTexture( pAiMaterial, aiTextureType_REFLECTION );
 //     }

   unsigned int arrayOfBuffersLen = ENG_ARRAY_SIZE_IN_ELEMENTS( pData->m_BufferObjects[ 0 ] );
   for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
      {
      glGenBuffers( arrayOfBuffersLen, pData->m_BufferObjects[ meshIdx ] );
      auto pMesh = pAiScene->mMeshes[ meshIdx ];
      ENG_ASSERT( pMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE );

      unsigned int vertexCount = pMesh->mNumVertices;
      unsigned int indicesCOunt = pMesh->mNumFaces * 3u;
      pData->m_MeshCount[ meshIdx ][ VideoMeshResourceExtraData::MeshCount_Vertex ] = vertexCount;
      pData->m_MeshCount[ meshIdx ][ VideoMeshResourceExtraData::MeshCount_Index ] = indicesCOunt;

      for( unsigned int vertexIdx = 0; vertexIdx < pMesh->mNumVertices; ++vertexIdx )
         {
         auto curSquareLength = pMesh->mVertices[ vertexIdx ].SquareLength();
         pData->m_Radius[ meshIdx ] = std::max( pData->m_Radius[ meshIdx ], curSquareLength );
         }
      pData->m_Radius[ meshIdx ] = std::sqrt( pData->m_Radius[ meshIdx ] );

      ENG_ASSERT( pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Vertex ] );
      glBindBuffer( GL_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Vertex ] );
      glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector3t<float> ), pMesh->mVertices, GL_STATIC_DRAW );

      ENG_ASSERT( pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Normal ] );
      glBindBuffer( GL_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Normal ] );
      glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector3t<float> ), pMesh->mNormals, GL_STATIC_DRAW );

      glBindBuffer( GL_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Tangent ] );
      glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector3t<float> ), pMesh->mTangents, GL_STATIC_DRAW );

      glBindBuffer( GL_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Bitangent ] );
      glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector3t<float> ), pMesh->mBitangents, GL_STATIC_DRAW );

      ENG_ASSERT( pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_UV ] );
      glBindBuffer( GL_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_UV ] );
      if( pMesh->mNumUVComponents[ 0 ] == 3u )
         {
         glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector2t<float> ), NULL, GL_STATIC_DRAW );
         unsigned int vec2DSize = sizeof( aiVector2D );
         for( unsigned int uvIdx = 0; uvIdx < vertexCount; ++uvIdx )
            {
            glBufferSubData( GL_ARRAY_BUFFER,
                             uvIdx * vec2DSize,
                             vec2DSize,
                             &pMesh->mTextureCoords[ 0 ][ uvIdx ] );
            }
         ENG_ASSERT( 0 );
        // glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector3t<float> ), pMesh->mTextureCoords[ 0 ], GL_STATIC_DRAW );
         }
      else if( pMesh->mNumUVComponents[ 0 ] == 2u )
         {
         glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof( aiVector2t<float> ), NULL, GL_STATIC_DRAW );
         unsigned int vec2DSize = sizeof( aiVector2D );
         for( unsigned int uvIdx = 0; uvIdx < vertexCount; ++uvIdx )
            {
            glBufferSubData( GL_ARRAY_BUFFER,
                             uvIdx * vec2DSize,
                             vec2DSize,
                             &pMesh->mTextureCoords[ 0 ][ uvIdx ] );
       //     ENG_ASSERT( pMesh->mTextureCoords[ 0 ][ uvIdx ].x >= 0.f && pMesh->mTextureCoords[ 0 ][ uvIdx ].x <= 1.f );
        //    ENG_ASSERT( pMesh->mTextureCoords[ 0 ][ uvIdx ].y >= 0.f && pMesh->mTextureCoords[ 0 ][ uvIdx ].y <= 1.f );
            }
         }
      else
         {
         ENG_ASSERT( "not suportted UV component Num" );
         }


      ENG_ASSERT( pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Index ] );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Index ] );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesCOunt * sizeof( unsigned int ), NULL, GL_STATIC_DRAW );

      unsigned int indexsSize = sizeof( unsigned int );
      for( unsigned int faceIdx = 0; faceIdx < pMesh->mNumFaces; ++faceIdx )
         {
         glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
                          faceIdx * 3u * indexsSize,
                          3u * indexsSize,
                          pMesh->mFaces[ faceIdx ].mIndices );
         }

      }
   videoHandle->SetExtraData( pExtraData );
   OpenGLRenderManager::CheckError();
   return S_OK;
   }

VideoMeshResourceExtraData* VideoMeshResourceLoader::LoadAndReturnMeshResourceExtraData( const Resource& resource )
   {
   auto pVideoHandle = g_pApp->m_pVideoResCache->GetHandle( resource );
   auto pRet = static_pointer_cast< VideoMeshResourceExtraData >( pVideoHandle->GetExtraData() );
   return pRet.get();
 
   }

shared_ptr< VideoResourceHandle > VideoMeshResourceLoader::LoadTexture( aiMaterial* pMaterial, aiTextureType type, const std::string& filePath ) const
   {
   unsigned int typeCount = pMaterial->GetTextureCount( type );
   ENG_ASSERT( typeCount <= 1 );
   for( unsigned int i = 0; i < typeCount; ++i )
      {
      aiString relFilePath;
      pMaterial->GetTexture( type, i, &relFilePath );
      std::string fullPath( filePath );
      fullPath.append( relFilePath.C_Str() );
      return g_pApp->m_pVideoResCache->GetHandle( Resource( fullPath ) );
      }
   return shared_ptr< VideoResourceHandle >();
   }