////////////////////////////////////////////////////////////////////////////////
// Filename: SceneNodes.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "SceneNodes.h"
#include "../Actors/ActorComponent.h"
#include "../Actors/TransformComponent.h"

SceneNodeProperties::SceneNodeProperties( void )
   {
   m_ActorId = INVALID_ACTOR_ID;
   m_Radius = 0;
   m_RenderPass = RenderPass_0;
   //m_AlphaType = AlphaOpaque;
   }

void SceneNodeProperties::Transform( Mat4x4* const toRelative, Mat4x4* const fromRelative ) const
   {
   if( toRelative )
      {
      *toRelative = m_ToRelSpace;
      }
   if( fromRelative )
      {
      *fromRelative = m_FromRelSpace;
      }
   }

SceneNode::~SceneNode()
   {
   }

void SceneNode::VSetRelTransform( const Mat4x4 *toRelative, const Mat4x4 *fromRelative )
   {
   m_Props.m_ToRelSpace = *toRelative;
   if( fromRelative )
      {
      m_Props.m_FromRelSpace = *fromRelative;
      }
   else
      {
      m_Props.m_FromRelSpace = toRelative->Inverse();
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
   pScene->PushAndSetMatrix( m_Props.m_ToRelSpace );
   return S_OK;
   }

bool SceneNode::VIsVisible( Scene *pScene ) const
   {
   Mat4x4 toWorld;
   Mat4x4 fromWorld;
  // pScene->GetCamera()->VGetProperties()->Transform( &toWorld, &fromWorld );
   Vec3 pos = GetWorldPosition();
   // transform to camera's local space
   pos = fromWorld.Xform( pos );
   const Frustum &frustum = pScene->GetCamera()->GetFrustum();
   return frustum.Inside( pos, VGetProperties()->GetRadius() );
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
            }
         it->VRenderChildren( pScene );
         }
      it->VPostRender( pScene );
      }
   }

int SceneNode::VPostRender( Scene *pScene )
   {
   pScene->PopMatrix();
   return S_OK;
   }

bool SceneNode::VAddChild( shared_ptr<ISceneNode> child )
   {
   m_Children.push_back( child );
   // child position in relative space
   Vec3 childPos = child->GetRelPosition();
   float newRadius = childPos.Length() + child->VGetProperties()->GetRadius();
   m_Props.m_Radius = std::max( m_Props.m_Radius, newRadius );
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
   Vec3 pos = GetRelPosition();
	if (m_pParent)
	   {
		pos += m_pParent->GetWorldPosition();
	   }
	return pos;
   }

/*
RootNode::RootNode(): SceneNode( INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, &Mat4x4::g_Identity )
   {
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Static, &Mat4x4::g_Identity));
	m_Children.push_back(staticGroup);	// RenderPass_Static = 0

	shared_ptr<SceneNode> actorGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Actor, &Mat4x4::g_Identity));
	m_Children.push_back(actorGroup);	// RenderPass_Actor = 1

	shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_Sky, &Mat4x4::g_Identity));
	m_Children.push_back(skyGroup);	// RenderPass_Sky = 2

	shared_ptr<SceneNode> invisibleGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,  WeakBaseRenderComponentPtr(),  RenderPass_NotRendered, &Mat4x4::g_Identity));
	m_Children.push_back(invisibleGroup);	// RenderPass_NotRendered = 3
   }
*/

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









