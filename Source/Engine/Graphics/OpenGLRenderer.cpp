/*!
 * \file OpenGLRenderer.cpp
 *
 * \author SCW
 * \date д╗ды 2016
 *
 *
 */

#include "EngineStd.h"
#include "OpenGLRenderer.h"
#include "..\ResourceCache\TextureResource.h"
#include "..\ResourceCache\MeshResource.h"

void OpenGLRenderer::VertexToBoneMapping::AddBoneData( BoneId boneID, float boneWeight )
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

OpenGLRenderer::OpenGLRenderer( void )
   {

   }

OpenGLRenderer::~OpenGLRenderer( void )
   {

   }

void OpenGLRenderer::VSetBackgroundColor( Color color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }

GLuint OpenGLRenderer::VOnRestore( void )
   {

   return GL_NO_ERROR;
   }

void OpenGLRenderer::VShutdown( void )
   {}

bool OpenGLRenderer::VPreRender( void )
   {
   glClearDepth( 1.0 );
   // use previously setted clearColr to draw background
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   return true;
   }

bool OpenGLRenderer::VPostRender( void )
   {
   SDL_GL_SwapWindow( g_pApp->GetWindow() );
   return true;
   }
/*
void OpenGLRenderer::VSetWorldTransform( const Mat4x4 *m )
{

}

void OpenGLRenderer::VSetViewTransform( const Mat4x4 *m )
{

}

void OpenGLRenderer::VSetProjectionTransform( const Mat4x4 *m )
{

}*/

void OpenGLRenderer::LoadTexture2D( GLuint* textureId, const Resource& textureResource )
   {
   glGenTextures( 1, textureId );

   //"Bind" the newly created texture : all future texture functions will modify this texture
   glBindTexture( GL_TEXTURE_2D, *textureId );
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

void OpenGLRenderer::LoadMesh( GLuint* pVertexBuffer, GLuint* pUvBuffer, GLuint* pIndexBuffer, GLuint* pNormalBuffer, shared_ptr<ResHandle> pMeshResHandle )
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
                          &pMesh->mNormals[ 0 ]);
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
   if( pIndexBuffer )
      {
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pIndexBuffer );
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
                       0,
                       indexBuffer.size() * sizeof( indexBuffer[ 0 ] ),
                       &indexBuffer[ 0 ] );
      }
   }

void OpenGLRenderer::LoadBones( GLuint* pBoneBuffer, shared_ptr<ResHandle> pMeshResHandle )
   {
   ENG_ASSERT( pBoneBuffer && pMeshResHandle );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );
   auto pAiScene = pMeshExtra->m_pScene;
   unsigned int vertexIdOffest = 0;
   std::vector< OpenGLRenderer::VertexToBoneMapping > vertexToBoneMappings( pMeshExtra->m_NumVertices );
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
                 vertexToBoneMappings.size() * sizeof( vertexToBoneMappings[0] ),
                 &vertexToBoneMappings[ 0 ],
                 GL_STATIC_DRAW );
   }

GLuint OpenGLRenderer::GenerateShader( const Resource& shaderRes, GLuint shaderType )
   {
   // Create the shaders
   GLuint shader = glCreateShader( shaderType );


   shared_ptr< ResHandle > pResourceHandle = g_pApp->m_pResCache->GetHandle( shaderRes );  // this actually loads the shader file from the zip file

   if( !pResourceHandle )
      {
      ENG_ERROR( "Invalid shader file path" );
      }
   // Compile Vertex Shader
   ENG_LOG( "Renderer", "Compiling vertex shader: " + shaderRes.m_Name );

   GLchar* p_VSSourcePointer = ( GLchar* ) pResourceHandle->GetBuffer();
   CompileShader( &p_VSSourcePointer, shader );

   return shader;
   }

GLuint OpenGLRenderer::CompileShader( const GLchar* const* pSrcData, const GLuint shaderID )
   {
   glShaderSource( shaderID, 1, pSrcData, NULL );
   glCompileShader( shaderID );

   GLint result = GL_FALSE;
   
   // Check Vertex Shader compiling
   glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
   if( result == GL_FALSE )
      {
      int infoLogLength;
      glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* p_ErrMsg = ENG_NEW GLchar[ infoLogLength + 1 ];
      glGetShaderInfoLog( shaderID, infoLogLength, NULL, p_ErrMsg );
      ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      glDeleteShader( shaderID ); // Don't leak the shader.
      return result;
      }

   return result;
   }

GLuint OpenGLRenderer::GenerateProgram( GLuint vertexShader, GLuint fragmentShader )
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
   glAttachShader( program, vertexShader );
   glAttachShader( program, fragmentShader );
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

void OpenGLRenderer::SetRenderAlpha( bool isAlpha )
   {
   if( isAlpha )
      {
      // Enable blending
      glEnable( GL_BLEND );
      glDisable( GL_CULL_FACE );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      }
   else
      {
      glDisable( GL_BLEND );
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
      glBlendFunc( GL_ONE, GL_ZERO );
      }

   }

void OpenGLRenderer::VDrawLine( const Vec3& from, const Vec3& to, const Color& color ) const
   {
   shared_ptr<Scene> pScene = g_pApp->m_pEngineLogic->m_pWrold;

   auto camera = pScene->GetCamera();
   if( !camera )
      {
      return;
      }

   Vec4 from4( from );
   Vec4 to4( to );
   Mat4x4 mvp = ( camera->GetProjection() * camera->GetView() );
   Vec4 from_Proj = mvp.Xform( from4 );
   Vec4 to_Proj = mvp.Xform( to4 );

   glLineWidth( 2.5 );

   glBegin( GL_LINES );
   glColor3f( color.m_Component.r, color.m_Component.g, color.m_Component.b );
   glVertex3f( from_Proj.x / from_Proj.w, from_Proj.y / from_Proj.w, from_Proj.z / from_Proj.w );
   glVertex3f( to_Proj.x / to_Proj.w, to_Proj.y / to_Proj.w, to_Proj.z / to_Proj.w );
   glEnd();
   }