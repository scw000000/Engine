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
	glBufferData( GL_ARRAY_BUFFER, 
                 pMeshExtra->m_pScene->mMeshes[0]->mNumVertices * sizeof( aiVector3t<float> ), 
                 &pMeshExtra->m_pScene->mMeshes[0]->mVertices[0], 
                 GL_STATIC_DRAW );

   glGenBuffers( 1, &m_UVBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, m_UVBuffer );
	glBufferData( GL_ARRAY_BUFFER, 
                 pMeshExtra->m_pScene->mMeshes[0]->mNumVertices * sizeof( aiVector3t<float> ), 
                 &pMeshExtra->m_pScene->mMeshes[0]->mTextureCoords[0][0], 
                 GL_STATIC_DRAW );

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
      }

   m_MVPMatrixUni = glGetUniformLocation( m_Program, "MVP");
   m_TextureUni = glGetUniformLocation( m_Program, "myTextureSampler");

	glGenTextures( 1, &m_Texture );

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );	

   shared_ptr< SDLTextureResourceExtraData > pTextureExtra = static_pointer_cast< SDLTextureResourceExtraData >( pTextureResHandle->GetExtraData() );
   auto pSurface = pTextureExtra->m_pSurface;
   int Mode = GL_RGB;
 
   if( pSurface->format->BytesPerPixel == 4 ) 
      {
      Mode = GL_RGBA;
      }
 
   glTexImage2D( GL_TEXTURE_2D, 0, Mode, pSurface->w, pSurface->h, 0, Mode, GL_UNSIGNED_BYTE, pSurface->pixels );
   
   //SetRadius( CalcBoundingSphere( &extra->m_Mesh11 ) );
   
   // resore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }


int MeshSceneNode::VRender( Scene *pScene )
   {
   glEnable( GL_COLOR_ARRAY );
   glBegin( GL_TRIANGLES );
   glColor3f( 1.0, 0.0f, 0.0f );
   glVertex2f( 0,0 );
   glVertex2f( 0, 500 );
   glVertex2f( 500, 500 );
   glEnd();
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
	glUniform1i( m_MVPMatrixUni, 0);

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

      // Force the Mesh to reload
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( &m_MeshResource );  	
   if( !pMeshResHandle )
      {
      return S_FALSE;
      }
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast<MeshResourceExtraData>( pMeshResHandle->GetExtraData() );
   pMeshExtra->m_pScene->mMeshes[0]->mPrimitiveTypes;
		// Draw the triangle !
//	glDrawArrays( GL_TRIANGLES, 0, pMeshExtra->m_pScene->mMeshes[0]->mNumVertices );

//	glDisableVertexAttribArray(0);//
//	glDisableVertexAttribArray(1);

   return S_OK;
   }