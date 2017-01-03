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

const std::vector< std::string > MESH_LOADER_PATTERNS( { "*.obj", "*.fbx" } );

VideoMeshResourceExtraData::~VideoMeshResourceExtraData( void )
   {
   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_MeshBuffers ), m_MeshBuffers );
   }

VideoMeshResourceLoader::VideoMeshResourceLoader( void ) : VideoResourceLoader( MESH_LOADER_PATTERNS )
   {
   
   }

int VideoMeshResourceLoader::VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle )
   {
   /*VideoMeshResourceExtraData* pData = ENG_NEW VideoMeshResourceExtraData();
   ENG_ASSERT( pData );
   shared_ptr< VideoMeshResourceExtraData > pExtraData( pData );
   
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( handle->GetExtraData() );

   auto pAiScene = pMeshExtra->m_pScene;

   glGenBuffers( 1, &m_MeshBuffers[ VideoMeshResourceExtraData::MeshBufferData_Vretex ] );
   glBindBuffer( GL_ARRAY_BUFFER, *m_MeshBuffers[ VideoMeshResourceExtraData::MeshBufferData_Vretex ] );
   glBufferData( GL_ARRAY_BUFFER, pMeshExtra->m_NumVertices * sizeof( aiVector3t<float> ), NULL, GL_STATIC_DRAW );

   if( pUvBuffer )
      {
      glGenBuffers( 1, pUvBuffer );
      glBindBuffer( GL_ARRAY_BUFFER, *pUvBuffer );
      glBufferData( GL_ARRAY_BUFFER, pMeshExtra->m_NumVertices * sizeof( aiVector2t<float> ), NULL, GL_STATIC_DRAW );
      }

   if( pNormalBuffer )
      {
      glGenBuffers( 1, pNormalBuffer );
      glBindBuffer( GL_ARRAY_BUFFER, *pNormalBuffer );
      glBufferData( GL_ARRAY_BUFFER, pMeshExtra->m_NumVertices * sizeof( aiVector3t<float> ), NULL, GL_STATIC_DRAW );
      }

   if( pIndexBuffer )
      {
      glGenBuffers( 1, pIndexBuffer );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pIndexBuffer );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, pMeshExtra->m_NumVertexIndex * sizeof( unsigned int ), NULL, GL_STATIC_DRAW );
      }

   unsigned int verticesIndexOffset = 0;
   std::vector< unsigned int > indexBuffer;
   if( pIndexBuffer )
      {
      indexBuffer.reserve( pMeshExtra->m_NumVertexIndex );
      }
   for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
      {
      auto pMesh = pAiScene->mMeshes[ meshIdx ];
      pAiScene->mTextures[0]->
      glBindBuffer( GL_ARRAY_BUFFER, *pVertexBuffer );
      glBufferSubData( GL_ARRAY_BUFFER,
                       verticesIndexOffset * sizeof( aiVector3t<float> ),
                       pMesh->mNumVertices * sizeof( aiVector3t<float> ),
                       &pMesh->mVertices[ 0 ] );

      if( pUvBuffer )
         {
         aiVector2t<float> *uvVertices = NULL;
         uvVertices = ENG_NEW aiVector2t<float>[ pMesh->mNumVertices ];
         for( unsigned int vertex = 0; vertex < pMesh->mNumVertices; vertex++ )
            {
            memcpy( &uvVertices[ vertex ], &pMesh->mTextureCoords[ 0 ][ vertex ], sizeof( aiVector2t<float> ) );
            }
         glBindBuffer( GL_ARRAY_BUFFER, *pUvBuffer );
         glBufferSubData( GL_ARRAY_BUFFER,
                          verticesIndexOffset * sizeof( aiVector2t<float> ),
                          pMesh->mNumVertices * sizeof( aiVector2t<float> ),
                          &uvVertices[ 0 ] );
         SAFE_DELETE_ARRAY( uvVertices );
         }

      if( pNormalBuffer )
         {
         glBindBuffer( GL_ARRAY_BUFFER, *pNormalBuffer );
         glBufferSubData( GL_ARRAY_BUFFER,
                          verticesIndexOffset * sizeof( aiVector3t<float> ),
                          pMesh->mNumVertices * sizeof( aiVector3t<float> ),
                          &pMesh->mNormals[ 0 ] );
         }

      if( pIndexBuffer )
         {
         for( unsigned int faceIdx = 0; faceIdx < pMesh->mNumFaces; ++faceIdx )
            {
            const aiFace& face = pMesh->mFaces[ faceIdx ];
            ENG_ASSERT( face.mNumIndices == 3 && "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." );
            indexBuffer.push_back( face.mIndices[ 0 ] + verticesIndexOffset );
            indexBuffer.push_back( face.mIndices[ 1 ] + verticesIndexOffset );
            indexBuffer.push_back( face.mIndices[ 2 ] + verticesIndexOffset );
            }
         }

      verticesIndexOffset += pMesh->mNumVertices;
      }
   ENG_ASSERT( pMeshExtra->m_NumVertices == verticesIndexOffset );
   if( pIndexBuffer )
      {
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pIndexBuffer );
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
                       0,
                       indexBuffer.size() * sizeof( indexBuffer[ 0 ] ),
                       &indexBuffer[ 0 ] );
      }
   ENG_ASSERT( pMeshExtra->m_NumVertexIndex == indexBuffer.size() );*/

   return S_OK;
   }