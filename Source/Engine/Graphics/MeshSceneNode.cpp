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
	printf("Linking program\n");
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

   // Force the Mesh to reload
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( &m_MeshResource );  	
   if( !pMeshResHandle )
      {
      return S_FALSE;
      }

   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );
   std::cout << "Loading Mesh" << std::endl;

   glGenBuffers( 1, &m_VerTexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_VerTexBuffer );
   static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};
   // One color for each vertex. They were generated randomly.
	
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

   /*
	glBufferData( GL_ARRAY_BUFFER, 
                 pMeshExtra->m_pScene->mMeshes[0]->mNumVertices * sizeof( aiVector3t<float> ), 
                 &pMeshExtra->m_pScene->mMeshes[0]->mVertices[0], 
                 GL_STATIC_DRAW );
                 */

   
   glGenBuffers( 1, &m_UVBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_UVBuffer );
   static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};
   //glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glBufferData( GL_ARRAY_BUFFER, 
                 pMeshExtra->m_pScene->mMeshes[0]->mNumVertices * sizeof( aiVector3t<float> ), 
                 &pMeshExtra->m_pScene->mMeshes[0]->mTextureCoords[0][0], 
                 GL_STATIC_DRAW );
   /*
      for( unsigned int meshIdx = 0; meshIdx < pMeshExtra->m_pScene->mNumMeshes; ++meshIdx )
         {
         for( unsigned int vertexIdx = 0; vertexIdx < pMeshExtra->m_pScene->mMeshes[meshIdx]->mNumVertices; ++vertexIdx )
            {
            auto vertex = pMeshExtra->m_pScene->mMeshes[meshIdx]->mVertices[vertexIdx];
            Vec3 vec( vertex.x, vertex.y, vertex.z );
            std::cout << ToStr( vec ) << std::endl;
            }
         }
      std::cout << "Loading Mesh UV" << std::endl;

      for( unsigned int meshIdx = 0; meshIdx < pMeshExtra->m_pScene->mNumMeshes; ++meshIdx )
         {
         for( unsigned int uvChannel = 0; uvChannel < pMeshExtra->m_pScene->mMeshes[meshIdx]->GetNumUVChannels(); ++uvChannel )
            {
            for(  unsigned int vertexIdx = 0; vertexIdx < pMeshExtra->m_pScene->mMeshes[meshIdx]->mNumVertices; ++vertexIdx )
               {
               auto vertex = pMeshExtra->m_pScene->mMeshes[meshIdx]->mTextureCoords[uvChannel][vertexIdx];
               std::cout << pMeshExtra->m_pScene->mMeshes[meshIdx]->GetNumUVChannels() << "" << vertexIdx << std::endl;
               Vec3 vec( vertex.x, vertex.y, 0.f );
               std::cout << ToStr( vec ) << std::endl;
               } 
            }  
         }
   shared_ptr<ResHandle> pTextureResHandle = g_pApp->m_pResCache->GetHandle( &m_TextureResource );
   if( !pTextureResHandle )
      {
      return S_FALSE;
      }*/

   m_MVPMatrixUni = glGetUniformLocation( m_Program, "MVP");
   /*m_TextureUni = glGetUniformLocation( m_Program, "myTextureSampler");

	glGenTextures( 1, &m_Texture );

	//"Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );	

   shared_ptr< SDLTextureResourceExtraData > pTextureExtra = static_pointer_cast< SDLTextureResourceExtraData >( pTextureResHandle->GetExtraData() );
   auto pSurface = pTextureExtra->m_pSurface;
   int Mode = GL_RGB;
 
   if( pSurface->format->BytesPerPixel == 4 ) 
      {
      Mode = GL_RGBA;
      }
 
   glTexImage2D( GL_TEXTURE_2D, 0, Mode, pSurface->w, pSurface->h, 0, Mode, GL_UNSIGNED_BYTE, pSurface->pixels );*/
   
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

   /*
	// Bind our texture in Texture Unit 0
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i( m_MVPMatrixUni, 0);*/

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
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

      // Force the Mesh to reload
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( &m_MeshResource );  	
   if( !pMeshResHandle )
      {
      return S_FALSE;
      }
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );

   // Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 36); // 3 indices starting at 0 -> 1 triangle
		// Draw the triangle !
//	glDrawArrays( GL_TRIANGLES, 0, pMeshExtra->m_pScene->mMeshes[0]->mNumVertices );

	glDisableVertexAttribArray(0);//
	glDisableVertexAttribArray(1);

   return S_OK;
   }