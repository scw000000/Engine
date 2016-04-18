////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Mesh.h"


unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
   {
   
   }

bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
   {
   
   }

MeshSceneNode::MeshSceneNode( const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Resource meshResouce, RenderPass renderPass,  const Mat4x4 *transform )
   : SceneNode( actorId, renderComponent, renderPass, transform ), m_MeshResource( meshResouce.m_name )
   {

   }


int MeshSceneNode::VOnRestore( Scene *pScene )
   {
   HRESULT hr;

	SceneNode::VOnRestore(pScene);

	m_VertexShader.OnRestore(pScene);
	m_FragmentShader.OnRestore(pScene);

    // Force the Mesh to reload

   shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &m_MeshResource );  	
	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>( pResourceHandle->GetExtra() );

	SetRadius( CalcBoundingSphere( &extra->m_Mesh11 ) );

	return S_OK;
   }

int MeshSceneNode::VRender( Scene *pScene )
   {
   
   }