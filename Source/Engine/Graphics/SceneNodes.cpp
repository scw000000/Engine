////////////////////////////////////////////////////////////////////////////////
// Filename: SceneNodes.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "SceneNodes.h"
#include "Scene.h"
#include "..\Actors\ActorComponent.h"
#include "..\Actors\TransformComponent.h"
#include "..\Actors\RenderComponent.h"

SceneNodeProperties::SceneNodeProperties( void )
   {
   m_ActorId = INVALID_ACTOR_ID;
   m_Radius = 0;
   m_RenderPass = RenderPass_0;
   //m_AlphaType = AlphaOpaque;
   }

void SceneNodeProperties::GetTransform( Mat4x4* ToWorld, Mat4x4* pFromWorld ) const
   {
   if( ToWorld )
      {
      *ToWorld = m_ToWorld;
      }
   if( pFromWorld )
      {
      *pFromWorld = m_FromWorld;
      }
   }

SceneNode::SceneNode( ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Mat4x4 *pToWorld, const Mat4x4 *pFromWorld )
   {
   m_pParent= NULL;
	m_Props.m_ActorId = actorId;
	m_Props.m_Name = ( renderComponent ) ? renderComponent->VGetName(): "SceneNode";
	m_Props.m_RenderPass = renderPass;
	//m_Props.m_AlphaType = AlphaOpaque;
	m_RenderComponent = renderComponent;
	VSetTransform( pToWorld, pFromWorld ); 
	SetRadius(0);
   }

SceneNode::~SceneNode()
   {
   }

void SceneNode::VSetTransform( const Mat4x4 *pToWorld, const Mat4x4 *pFromWorld )
   {
   m_Props.m_ToWorld = *pToWorld;
   if( pFromWorld )
      {
      m_Props.m_FromWorld = *pFromWorld;
      }
   else
      {
      m_Props.m_FromWorld = pToWorld->Inverse();
      }
   }

int SceneNode::VOnRestore( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      it->VOnRestore( pScene );
      }
   return S_OK;
   }

int SceneNode::VOnUpdate( Scene* pScene, const unsigned long deltaMs )
   {
   for( auto it : m_Children )
      {
      it->VOnUpdate( pScene, deltaMs );
      }
   return S_OK;
   }

int SceneNode::VPreRender( Scene *pScene )
   {
   pScene->PushAndSetMatrix( VGetProperties()->GetToWorld() );
   return S_OK;
   }

bool SceneNode::VIsVisible( Scene *pScene ) const
   {
   Mat4x4 toWorld;
   Mat4x4 fromWorld;
   pScene->GetCamera()->VGetProperties()->GetTransform( &toWorld, &fromWorld );
   Vec3 nodeInCamWorldPos = GetWorldPosition();
   // transform to camera's local space
   nodeInCamWorldPos = fromWorld.Xform( nodeInCamWorldPos );
    Mat4x4 camMat = glm::lookAt(
								glm::vec3(7, 9, 10), // position in World Space
								glm::vec3(0,0,0), // look target
								glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
   // beware! the Z direction of camera in openGL is inversed!
   //nodeInCamWorldPos.z *= -1.0f; 
   const Frustum &frustum = pScene->GetCamera()->GetFrustum();
   //return true;
   return frustum.Inside( nodeInCamWorldPos, VGetProperties()->GetRadius() );
   }


// TODO: finish the implementation
int SceneNode::VRenderChildren( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      if( it->VPreRender( pScene ) == S_OK )
         {
         if( it->VIsVisible( pScene ) )
            {
            it->VRender( pScene );
            }
         it->VRenderChildren( pScene );
         }
      it->VPostRender( pScene );
      }
   return S_OK;
   }

int SceneNode::VPostRender( Scene *pScene )
   {
   pScene->PopMatrix();
   return S_OK;
   }

bool SceneNode::VAddChild( shared_ptr<ISceneNode> child )
   {
   m_Children.push_back( child );
   // child position in parent's space
   Vec3 childPos = child->GetToWorldPosition();
   float newRadius = childPos.Length() + child->VGetProperties()->GetRadius();
   m_Props.m_Radius = std::max( m_Props.m_Radius, newRadius );
   return true; 
   }

bool SceneNode::VRemoveChild( ActorId id )
   {
   for( auto it = m_Children.begin(); it != m_Children.end(); ++it )
	   {
		ActorId childId = (*it)->VGetProperties()->GetActorId();
      if( childId != INVALID_ACTOR_ID && childId == id )
		   {
			m_Children.erase( it );	//this can be expensive for vectors
			return true;
		   }
	   }
	return false;
   }

int SceneNode::VOnLostDevice( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      it->VOnLostDevice( pScene );
      }
   return S_OK;
   }

// TODO: finish inplementation
void SceneNode::SetAlpha( float alpha )
   {
   for( auto it : m_Children )
      {
      // downward cast, is it safe?
      shared_ptr<SceneNode> sceneNode = static_pointer_cast<SceneNode>( it );
      sceneNode->SetAlpha( alpha );
      }
   }

// Sum up relative position from child to root node in order to get position in world space
Vec3 SceneNode::GetWorldPosition( void ) const
   {
   Vec3 pos = GetToWorldPosition();
	if ( m_pParent )
	   {
		pos += m_pParent->GetWorldPosition();
	   }
	return pos;
   }


RootNode::RootNode(): SceneNode( INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, &Mat4x4::g_Identity )
   {
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Static, &Mat4x4::g_Identity) );
	m_Children.push_back(staticGroup);	// RenderPass_Static = 0

	shared_ptr<SceneNode> actorGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Actor, &Mat4x4::g_Identity) );
	m_Children.push_back(actorGroup);	// RenderPass_Actor = 1

	shared_ptr<SceneNode> skyGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Sky, &Mat4x4::g_Identity) );
	m_Children.push_back(skyGroup);	// RenderPass_Sky = 2

	shared_ptr<SceneNode> invisibleGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_NotRendered, &Mat4x4::g_Identity ) );
	m_Children.push_back(invisibleGroup);	// RenderPass_NotRendered = 3
   }
   

// add child into corresponded renderpass node
bool RootNode::VAddChild( shared_ptr<ISceneNode> child )
   {
   RenderPass pass = child->VGetProperties()->GetRenderPass();
	if ( (unsigned)pass >= m_Children.size() || !m_Children[pass] )
	   {
		ENG_ASSERT(0 && _T("There is no such render pass"));
		return false;
	   }

	return m_Children[pass]->VAddChild( child );
   }

// TODO: finish implement
int RootNode::VRenderChildren( Scene *pScene )
   {
   // the child node of Root nodes are empty node, so there's no need calling VRender for them
   // only their children should be rendered
   for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	   {
		switch(pass)
		   {
			case RenderPass_Static:
			case RenderPass_Actor:
				m_Children[pass]->VRenderChildren(pScene);
				break;

			case RenderPass_Sky:
			   {
				//shared_ptr<IRenderState> skyPass = pScene->GetRenderer()->VPrepareSkyBoxPass();
				m_Children[pass]->VRenderChildren(pScene);
				break;
			   }
         }
      }
	return S_OK;
   }

// seartch and remove child from all renderpass nodes
// can I speed up this? search its renderpass first?
bool RootNode::VRemoveChild( ActorId id )
   {
   bool anythingRemoved = false;
	for( auto it : m_Children )
      {
		if( it->VRemoveChild( id ) )
		   {
			anythingRemoved = true;
		   }
	   }
	return anythingRemoved;
   }

CameraNode::CameraNode( Mat4x4 const *t, Frustum const &frustum ) 
	      : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, t),
	      m_Frustum( frustum ),
	      m_IsActive( true ),
	      m_IsDebugCamera( false ),
         m_pTarget( shared_ptr<SceneNode>() ),
	      m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ),
         m_View( Mat4x4::ViewMatrix( t->GetToWorldPosition(), t->GetToWorldPosition() + t->GetForward(), g_Up ) )
   {
   }

CameraNode::CameraNode( const Vec3& eye, const Vec3& center, const Vec3& up, Frustum const &frustum ) 
	      : SceneNode( INVALID_ACTOR_ID, 
                  WeakBaseRenderComponentPtr(), 
                  RenderPass_0, 
                  &Mat4x4::LookAt( eye, center, up ).Inverse() 
                  ),
            m_Frustum( frustum ),
	         m_IsActive( true ),
	         m_IsDebugCamera( false ),
            m_pTarget( shared_ptr<SceneNode>() ),
	         m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ),
            m_View( Mat4x4::ViewMatrix( eye, center, g_Up ) )
   { 
   }

//TODO: implement m_Frustum render
int CameraNode::VRender( Scene *pScene )
   {
   if ( m_IsDebugCamera )
	   {
	//	pScene->PopMatrix();
      // render four plans of m_Frustum
	//	m_Frustum.Render();	

	//	pScene->PushAndSetMatrix( m_Props.GetToRelSpace() );
	   }

	return S_OK;
   }

int CameraNode::VOnRestore( Scene *pScene )
   {
   m_Frustum.SetAspect(  (float) g_pApp->GetScreenSize().GetX() / (float) g_pApp->GetScreenSize().GetY() );
   m_Projection.BuildProjection( m_Frustum.m_Fov, m_Frustum.m_Aspect, m_Frustum.m_NearDis, m_Frustum.m_FarDis );
	return S_OK;
   }

void CameraNode::VSetTransform( const Mat4x4 *pToWorld, const Mat4x4 *pFromWorld ) 
   { 
   SceneNode::VSetTransform( pToWorld, pFromWorld ); 
   m_View = Mat4x4::ViewMatrix( pToWorld->GetToWorldPosition(), pToWorld->GetToWorldPosition() + pToWorld->GetForward(), pToWorld->GetUp() );

   }

Mat4x4 CameraNode::GetWorldViewProjection( Scene *pScene ) const
   {
   Mat4x4 world = pScene->GetTopMatrix();
	Mat4x4 view = VGetProperties()->GetFromWorld();
	return m_Projection * m_View * world;
   }

// TODO: check if it's correct
// I think atWorld should be set by GetWorldPosition
int CameraNode::SetViewTransform(  Scene *pScene )
   {
   // this code simulates camera pole effect, camera is sticked to target's position +  m_CamOffsetVector
	if( m_pTarget )
	   {
      // Get target's local transform
		Mat4x4 targetInParentSpace = m_pTarget->VGetProperties()->GetToWorld();
		Vec4 at = m_CamOffsetVector;
		Vec4 atWorld = targetInParentSpace.Xform( at );
      // 
		Vec3 pos = m_pTarget->GetToWorldPosition() + at;
		targetInParentSpace.SetToWorldPosition( pos );
		VSetTransform( &targetInParentSpace );
	   }

	//m_View = VGetProperties()->GetFromWorld();

	return S_OK;
   }









