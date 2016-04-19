////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MeshSceneNode.h"
#include "..\ResourceCache\MeshResource.h"

MeshSceneNode::MeshSceneNode( const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Resource meshResouce, RenderPass renderPass,  const Mat4x4 *transform )
   : SceneNode( actorId, renderComponent, renderPass, transform ), m_MeshResource( meshResouce.m_name )
   {
   m_Program = 0;
   m_VerTexBuffer = 0;
   m_UVBuffer = 0;
   m_MVPMatrix = 0;
   m_Texture = 0;
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
   if( !m_VertexArray )
      {
      glDeleteVertexArrays( 1, &m_VertexArray );
      }
	glGenVertexArrays( 1, &m_VertexArray );
	glBindVertexArray( m_VertexArray );

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

	glDetachShader( m_Program, m_VertexShader.GetVertexShader() );
	glDetachShader( m_Program, m_FragmentShader.GetFragmentShader() );
	
	glDeleteShader(  m_VertexShader.GetVertexShader() );
	glDeleteShader( m_FragmentShader.GetFragmentShader() );

   // Force the Mesh to reload
   shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &m_MeshResource );  	
	shared_ptr<MeshResourceExtraData> extra = static_pointer_cast<MeshResourceExtraData>( pResourceHandle->GetExtraData() );
   
   std::cout << "Loading Mesh" << std::endl;

   for( unsigned int meshIdx = 0; meshIdx < extra->m_pScene->mNumMeshes; ++meshIdx )
      {
      for( unsigned int vertexIdx = 0; vertexIdx < extra->m_pScene->mMeshes[meshIdx]->mNumVertices; ++vertexIdx )
         {
         auto vertex = extra->m_pScene->mMeshes[meshIdx]->mVertices[vertexIdx];
         Vec3 vec( vertex.x, vertex.y, vertex.z );
         std::cout << ToStr( vec ) << std::endl;
         }
      }

	//SetRadius( CalcBoundingSphere( &extra->m_Mesh11 ) );
   
   // resore all of its children
	SceneNode::VOnRestore( pScene );

	return S_OK;
   }


int MeshSceneNode::VRender( Scene *pScene )
   {
   return S_OK;
   }