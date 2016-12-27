/*!
 * \file RendererLoader.cpp
 * \date 2016/12/26 13:36
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
#include "RendererLoader.h"
#include "..\ResourceCache\TextureResource.h"
#include "..\ResourceCache\MeshResource.h"

void VertexToBoneMapping::AddBoneData( BoneId boneID, float boneWeight )
   {
   for( unsigned i = 0; i < MAXIMUM_BONES_PER_VEREX; i++ )
      {
      if( m_BoneWeights[ i ] == 0.0 )
         {
         m_BoneIDs[ i ] = boneID;
         m_BoneWeights[ i ] = boneWeight;
         return;
         }
      }

   // should never get here - more bones than we have space for
   ENG_ASSERT( 0 );
   }

void OpenGLRendererLoader::LoadTexture2D( GLuint* textureId, const Resource& textureResource )
   {
   glGenTextures( 1, textureId );

   //"Bind" the newly created texture : all future texture functions will modify this texture
   glBindTexture( GL_TEXTURE_2D, *textureId );
   // No worry for row size that is not evenly divided by 4
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

   auto pSurface = TextureResourceLoader::LoadAndReturnSurface( textureResource );

   int Mode = GL_RGB;
   if( pSurface->format->BytesPerPixel == 4 )
      {
      Mode = GL_RGBA;
      }
   glTexImage2D( GL_TEXTURE_2D, 0, Mode, pSurface->w, pSurface->h, 0, Mode, GL_UNSIGNED_BYTE, pSurface->pixels );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   }

void OpenGLRendererLoader::LoadMesh( GLuint* pVertexBuffer, GLuint* pUvBuffer, GLuint* pIndexBuffer, GLuint* pNormalBuffer, shared_ptr<ResHandle> pMeshResHandle )
   {
   ENG_ASSERT( pVertexBuffer && pMeshResHandle );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   auto pAiScene = pMeshExtra->m_pScene;

   glGenBuffers( 1, pVertexBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, *pVertexBuffer );
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
   ENG_ASSERT( pMeshExtra->m_NumVertexIndex == indexBuffer.size() );
   }

void OpenGLRendererLoader::LoadBones( GLuint* pBoneBuffer, shared_ptr<ResHandle> pMeshResHandle )
   {
   ENG_ASSERT( pBoneBuffer && pMeshResHandle );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );
   auto pAiScene = pMeshExtra->m_pScene;
   unsigned int vertexIdOffest = 0;
   std::vector< VertexToBoneMapping > vertexToBoneMappings( pMeshExtra->m_NumVertices );
   for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
      {
      auto pMesh = pAiScene->mMeshes[ meshIdx ];
      for( unsigned int boneIdx = 0; boneIdx < pMesh->mNumBones; boneIdx++ )
         {
         auto pBone = pMesh->mBones[ boneIdx ];
         ENG_ASSERT( pMeshExtra->m_BoneMappingData.find( pBone->mName.C_Str() ) != pMeshExtra->m_BoneMappingData.end() );
         BoneId boneId = pMeshExtra->m_BoneMappingData[ pBone->mName.C_Str() ].m_BoneId;
         for( unsigned int weightIdx = 0; weightIdx < pBone->mNumWeights; weightIdx++ )
            {
            const aiVertexWeight& boneWeight = pBone->mWeights[ weightIdx ];
            vertexToBoneMappings[ vertexIdOffest + boneWeight.mVertexId ].AddBoneData( boneId, boneWeight.mWeight );
            }
         }
      vertexIdOffest += pMesh->mNumVertices;
      }
   ENG_ASSERT( vertexIdOffest == pMeshExtra->m_NumVertices );
   glGenBuffers( 1, pBoneBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, *pBoneBuffer );
   glBufferData( GL_ARRAY_BUFFER,
                 vertexToBoneMappings.size() * sizeof( vertexToBoneMappings[ 0 ] ),
                 &vertexToBoneMappings[ 0 ],
                 GL_STATIC_DRAW );
   }

void OpenGLRendererLoader::CompileAndLoadShader( GLuint& shaderObj, const Resource& shaderRes, GLuint shaderType )
   {
   shaderObj = glCreateShader( shaderType );

   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( shaderRes );  // this actually loads the shader file from the zip file

   if( !pResourceHandle )
      {
      ENG_ERROR( "Invalid shader file path" );
      }
   // Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling shader: " + shaderRes.m_Name );

   GLchar* p_shaderText = ( GLchar* ) pResourceHandle->GetBuffer();

   glShaderSource( shaderObj, 1, &p_shaderText, NULL );
   glCompileShader( shaderObj );

   GLint result = GL_FALSE;

   // Check Vertex Shader compiling
   glGetShaderiv( shaderObj, GL_COMPILE_STATUS, &result );
   if( result == GL_FALSE )
      {
      int infoLogLength;
      glGetShaderiv( shaderObj, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* p_ErrMsg = ENG_NEW GLchar[ infoLogLength + 1 ];
      glGetShaderInfoLog( shaderObj, infoLogLength, NULL, p_ErrMsg );
      ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      glDeleteShader( shaderObj ); // Don't leak the shader.
      shaderObj = 0;
      }
   }

GLuint OpenGLRendererLoader::GenerateProgram( const std::vector< GLuint >& shderObjs )
   {
   GLint result = GL_FALSE;
   GLuint program = glCreateProgram();


   result = glGetError();
   if( result != GL_NO_ERROR )
      {
      ENG_ASSERT( 0 && "Program create failed " );
      return 0;
      }

   // Link the program
   for( auto obj : shderObjs )
      {
      glAttachShader( program, obj );
      }

   // glAttachShader( program, fragmentShader );
   glLinkProgram( program );

   // Check the program
   glGetProgramiv( program, GL_LINK_STATUS, &result );
   if( result == GL_FALSE )
      {
      int infoLogLength;
      glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* p_ErrMsg = ENG_NEW GLchar[ infoLogLength + 1 ];
      glGetProgramInfoLog( program, infoLogLength, NULL, p_ErrMsg );
      ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      glDeleteProgram( program );
      return 0;
      }

   return program;
   }