////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLRenderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "OpenGLRenderer.h"
#include "..\ResourceCache\TextureResource.h"
#include "..\ResourceCache\MeshResource.h"

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
   {
   }

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

void OpenGLRenderer::LoadTexture( GLuint* textureId, Resource& textureResource )
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

void OpenGLRenderer::LoadMesh( GLuint* vertexBuffer, float* radius, GLuint* uvBuffer, GLuint* indexBuffer, GLuint* normalBuffer, Resource& meshResource )
   {
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( meshResource );
   auto pMesh = pAiScene->mMeshes[0];

   glGenBuffers( 1, vertexBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, *vertexBuffer );
   glBufferData( GL_ARRAY_BUFFER,
                 pMesh->mNumVertices * sizeof( aiVector3t<float> ),
                 &pMesh->mVertices[0],
                 GL_STATIC_DRAW );

   *radius = -1.0f;

   // UV coordinates loaded in assimp is 3-dimensions
   // But GLSL expect 2-dimension coordinates
   // So we have to translate them
   aiVector2t<float> *uvVertices = NULL;



   if( uvBuffer )
      {
      uvVertices = ENG_NEW aiVector2t<float>[pMesh->mNumVertices];
      glGenBuffers( 1, uvBuffer );
      }

   for( unsigned int vertex = 0; vertex < pMesh->mNumVertices; vertex++ )
      {
      auto curSquareLength = pMesh->mVertices[vertex].SquareLength( );
      *radius = std::max( *radius, curSquareLength );
      if( uvBuffer )
         {
         memcpy( &uvVertices[vertex], &pMesh->mTextureCoords[0][vertex], sizeof( aiVector2t<float> ) );
         }
      }

   if( uvBuffer )
      {
      glBindBuffer( GL_ARRAY_BUFFER, *uvBuffer );
      glBufferData( GL_ARRAY_BUFFER,
                   pMesh->mNumVertices * sizeof( aiVector2t<float> ),
                    &uvVertices[0],
                    GL_STATIC_DRAW );
      SAFE_DELETE_ARRAY( uvVertices );
      }
   // Loading UV Buffer & calculate bounding sphere radius


   if( normalBuffer )
      {
      glGenBuffers( 1, normalBuffer );
      glBindBuffer( GL_ARRAY_BUFFER, *normalBuffer );
      glBufferData( GL_ARRAY_BUFFER,
                    pMesh->mNumVertices * sizeof( aiVector3t<float> ),
                    &pMesh->mNormals[0],
                    GL_STATIC_DRAW );

      }

   if( indexBuffer )
      {
      std::vector<unsigned int> indexes( pMesh->mNumFaces * 3, 0 );
      for( unsigned int t = 0; t < pMesh->mNumFaces; ++t )
         {
         const aiFace& face = pMesh->mFaces[t];
         ENG_ASSERT( face.mNumIndices == 3 && "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." );
         indexes[ t * 3 + 0] = face.mIndices[0];
         indexes[ t * 3 + 1] = face.mIndices[1];
         indexes[ t * 3 + 2] = face.mIndices[2];
         }
      glGenBuffers( 1, indexBuffer );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *indexBuffer );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                    indexes.size() * sizeof( unsigned int ),
                    &indexes[0],
                    GL_STATIC_DRAW );
      }
    /*
      for( unsigned int n = 0; n < pAiScene->mNumMeshes; ++n )
      {
      const struct aiMesh* mesh = pAiScene->mMeshes[n];

      if( mesh->mNormals == NULL )
      {
      glDisable( GL_LIGHTING );
      }
      else
      {
      glEnable( GL_LIGHTING );
      }

      for( unsigned int t = 0; t < mesh->mNumFaces; ++t )
      {

      auto face = &mesh->mFaces[t];
      GLenum face_mode;
      std::cout << "face: " << face->mNumIndices << "prime " << mesh->mPrimitiveTypes << std::endl;
      switch( face->mNumIndices )
      {
      case 1: face_mode = GL_POINTS; break;
      case 2: face_mode = GL_LINES; break;
      case 3: face_mode = GL_TRIANGLES; break;
      default: face_mode = GL_POLYGON; break;
      }

      for( unsigned int i = 0; i < face->mNumIndices; i++ )
      {
      int index = face->mIndices[i];
      auto vertex = mesh->mVertices[index];
      Vec3 vec( vertex.x, vertex.y, vertex.z );
      std::cout << "index " << index << ToStr( vec ) << std::endl;
      }
      std::cout << std::endl;
      }
      }

      std::cout << "index num :" << pAiScene->mMeshes[0]->mNumVertices << std::endl;
      for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
      {
      for( unsigned int vertexIdx = 0; vertexIdx < pAiScene->mMeshes[meshIdx]->mNumVertices; ++vertexIdx )
      {
      auto vertex = pAiScene->mMeshes[meshIdx]->mVertices[vertexIdx];
      Vec3 vec( vertex.x, vertex.y, vertex.z );
      std::cout << "vertex : " << vertexIdx << std::endl;
      std::cout << ToStr( vec ) << std::endl;
      auto vertexCord = pAiScene->mMeshes[meshIdx]->mTextureCoords[0][vertexIdx];
      Vec3 vecC( vertexCord.x, vertexCord.y, vertexCord.z );
      std::cout << ToStr( vecC ) << std::endl << std::endl;
      }
      }*/
   }

GLuint OpenGLRenderer::CompileShader( const GLchar* const* pSrcData, const GLuint shaderID )
   {
   glShaderSource( shaderID, 1, pSrcData, NULL);
	glCompileShader( shaderID );

   GLint result = GL_FALSE;
	int infoLogLength;
	// Check Vertex Shader compliing
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if ( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ infoLogLength + 1];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
	   }
   return result;
   }

GLuint OpenGLRenderer::GenerateProgram( GLuint vertexShader, GLuint fragmentShader )
   {
   GLint result = GL_FALSE;
   GLuint program = glCreateProgram( );


   result = glGetError( );
   if( result != GL_NO_ERROR )
      {
      ENG_ASSERT( 0 && "Program create failed " );
      return 0;
      }

   // Link the program
   glAttachShader( program, vertexShader );
   glAttachShader( program, fragmentShader );
   glLinkProgram( program );


   int infoLogLength;
   // Check the program
   glGetProgramiv( program, GL_LINK_STATUS, &result );
   glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );

   if( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[infoLogLength + 1];
      glGetProgramInfoLog( program, infoLogLength, NULL, p_ErrMsg );
      ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
      return 0;
      }

   return program;
   }