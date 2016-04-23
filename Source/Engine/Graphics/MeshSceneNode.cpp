////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MeshSceneNode.h"
#include "..\ResourceCache\MeshResource.h"
#include "..\ResourceCache\TextureResource.h"

MeshSceneNode::MeshSceneNode( 
   const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Resource& meshResouce, const Resource& textureResource, RenderPass renderPass,  const Mat4x4 *transform )
   : SceneNode( actorId, renderComponent, renderPass, transform ), m_MeshResource( meshResouce.m_name ), m_TextureResource( textureResource.m_name )
   {
   m_Program = 0;
   m_VerTexBuffer = 0;
   m_UVBuffer = 0;
   m_MVPMatrixUni = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArray = 0;
   }

MeshSceneNode::~MeshSceneNode( void )
   {
   if( m_Program )
      {
      glDeleteProgram( m_Program );
      }
   }

// now load the reouce into VRam
int MeshSceneNode::VOnRestore( Scene *pScene )
   {
   if( m_VertexArray )
      {
      glDeleteVertexArrays( 1, &m_VertexArray );
      m_VertexArray = 0;
      }
	glGenVertexArrays( 1, &m_VertexArray );
	glBindVertexArray( m_VertexArray );

   if( m_VerTexBuffer )
      {
      glDeleteBuffers( 1, &m_VerTexBuffer );
      m_VerTexBuffer = 0;
      }

   if( m_UVBuffer )
      {
      glDeleteBuffers( 1, &m_UVBuffer );
      m_UVBuffer = 0;
      }

   if( m_TextureUni )
      {
      glDeleteTextures( 1, &m_TextureUni );
      m_Texture = 0;
      }

   GLint result = GL_FALSE;
   if( !m_Program )
      {
      m_Program = glCreateProgram();
      result =  glGetError();
      if( result != GL_NO_ERROR )
         {
         return result;
         } 
      }

   m_VertexShader.ReleaseShader( m_Program );
   m_FragmentShader.ReleaseShader( m_Program );

   m_VertexShader.OnRestore( pScene );
   m_FragmentShader.OnRestore( pScene );

   // Link the program
	glAttachShader( m_Program, m_VertexShader.GetVertexShader() );
	glAttachShader( m_Program, m_FragmentShader.GetFragmentShader() );
	glLinkProgram( m_Program );

   
   int infoLogLength;
	// Check the program
	glGetProgramiv( m_Program, GL_LINK_STATUS, &result );
	glGetProgramiv( m_Program, GL_INFO_LOG_LENGTH, &infoLogLength );

	if ( infoLogLength > 0 )
      {
      GLchar* p_ErrMsg = new GLchar[ infoLogLength + 1];
		glGetProgramInfoLog( m_Program, infoLogLength, NULL, p_ErrMsg );
		ENG_ERROR( p_ErrMsg );
      SAFE_DELETE_ARRAY( p_ErrMsg );
	   }

   ////////////////////////////////////
   // Loading Vertex Buffer
   ////////////////////////////////////
   // Force the Mesh to reload
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( m_MeshResource );

   glGenBuffers( 1, &m_VerTexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_VerTexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 
                 pAiScene->mMeshes[0]->mNumVertices * sizeof( aiVector3t<float> ), 
                 &pAiScene->mMeshes[0]->mVertices[0], 
                 GL_STATIC_DRAW );
   ////////////////////////////////////
   // Loading Vertex Buffer
   ////////////////////////////////////
   //auto nd = pMeshExtra->m_pScene;
   for (unsigned int n = 0; n < pAiScene->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pAiScene->mMeshes[n];

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) 
         {

			auto face = &mesh->mFaces[t];
			GLenum face_mode;
          std::cout << "face: " << face->mNumIndices << "prime " << mesh->mPrimitiveTypes << std::endl;
			switch(face->mNumIndices) 
            {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
         
			for(unsigned int i = 0; i < face->mNumIndices; i++) 
            {
				int index = face->mIndices[i];
            auto vertex = mesh->mVertices[index];
            Vec3 vec( vertex.x, vertex.y, vertex.z );
            std::cout << "index " << index  << ToStr( vec ) << std::endl;
			}
         std::cout << std::endl;
		}
      }

   std::cout << "index num :" << pAiScene->mMeshes[0]->mNumVertices << std::endl;
      for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
         {
         for( unsigned int vertexIdx = 0; vertexIdx < pAiScene->mMeshes[meshIdx]->mNumVertices; ++vertexIdx )
            {
            auto vertex =pAiScene->mMeshes[meshIdx]->mVertices[vertexIdx];
            Vec3 vec( vertex.x, vertex.y, vertex.z );
            std::cout << "vertex : " << vertexIdx << std::endl;
            std::cout << ToStr( vec ) << std::endl;
            auto vertexCord = pAiScene->mMeshes[meshIdx]->mTextureCoords[0][vertexIdx];
            Vec3 vecC( vertexCord.x, vertexCord.y, vertexCord.z );
            std::cout << ToStr( vecC ) << std::endl << std::endl;
            }
         }
 
   ////////////////////////////////////
   // Loading UV Buffer
   ////////////////////////////////////
   glGenBuffers( 1, &m_UVBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_UVBuffer );
   
   // UV coordinates loaded in assimp is 3-dimensions
   // But GLSL expect 2-dimension coordinates
   // So we have to translate them
   aiVector2t<float> *uvBuffer = ENG_NEW aiVector2t<float>[pAiScene->mMeshes[0]->mNumVertices ];
   for ( unsigned int vertex = 0; vertex < pAiScene->mMeshes[0]->mNumVertices; vertex++)
      {
      memcpy( &uvBuffer[vertex], &pAiScene->mMeshes[0]->mTextureCoords[0][vertex], sizeof( aiVector2t<float> ) );
      }
	glBufferData( GL_ARRAY_BUFFER, 
                 pAiScene->mMeshes[0]->mNumVertices * sizeof( aiVector2t<float> ), 
                 &uvBuffer[0], 
                 GL_STATIC_DRAW );
   SAFE_DELETE_ARRAY( uvBuffer );
   ////////////////////////////////////
   // Loading UV Buffer
   ////////////////////////////////////
   

   ////////////////////////////////////
   // Loading Texture
   ////////////////////////////////////
   glGenTextures( 1, &m_Texture );

	//"Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );	

   auto pSurface = TextureResourceLoader::LoadAndReturnSurface( m_TextureResource );

   int Mode = GL_RGB;
   if( pSurface->format->BytesPerPixel == 4 ) 
      {
      Mode = GL_RGBA;
      }
 
   glTexImage2D( GL_TEXTURE_2D, 0, Mode, pSurface->w, pSurface->h, 0, Mode, GL_UNSIGNED_BYTE, pSurface->pixels );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   ////////////////////////////////////
   // Loading Texture
   ////////////////////////////////////

   m_MVPMatrixUni = glGetUniformLocation( m_Program, "MVP");
   m_TextureUni = glGetUniformLocation( m_Program, "myTextureSampler");

   //SetRadius( CalcBoundingSphere( &extra->m_Mesh11 ) );
   
   // resore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }

int MeshSceneNode::VRender( Scene *pScene )
   {
	// Use our shader
	glUseProgram( m_Program );
   

   // Get the projection & view matrix from the camera class
	Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetWorldViewProjection( pScene );
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv( m_MVPMatrixUni, 1, GL_FALSE, &mWorldViewProjection[0][0]);

   
	// Bind our texture in Texture Unit 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i( m_TextureUni, 0);

		// 1rst attribute buffer : vertices
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, m_VerTexBuffer );
	glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
   
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray( 1 );
	glBindBuffer( GL_ARRAY_BUFFER, m_UVBuffer );
	glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
   // Force the Mesh to reload to getting vertex number
   auto pAiScene = MeshResourceLoader::LoadAndReturnScene( m_MeshResource );
      
	// Draw the triangle !
   // Beware of vertex numbers, I may have to use index buffer
	glDrawArrays( GL_TRIANGLES, 0, pAiScene->mMeshes[0]->mNumVertices );

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );

   return S_OK;
   }