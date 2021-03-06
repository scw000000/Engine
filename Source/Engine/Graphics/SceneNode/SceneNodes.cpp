/*!
 * \file SceneNodes.cpp
 * \date 2016/06/08 15:09
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */

#include "EngineStd.h"
#include "SceneNodes.h"
#include "..\Scene.h"
#include "..\Actors\ActorComponent.h"
#include "..\Actors\RenderComponent.h"

SceneNodeProperties::SceneNodeProperties( void ) : m_pTransform(), m_pGlobalTransform( ENG_NEW Transform() )
   {
   m_ActorId = INVALID_ACTOR_ID;
   m_Radius = 0;
   m_RenderGroup = RenderGroup_0;
   m_EnableShadow = false;
   }

TransformPtr SceneNodeProperties::GetTransformPtr( void ) const
   {
   return m_pTransform;
   }

Transform SceneNodeProperties::GetLocalTransform( void ) const
   {
   return *m_pTransform;
   }

void SceneNodeProperties::SetAlpha( const float alpha )
   {
   /*if( m_pMaterial )
      {
      m_pMaterial->SetAlpha( alpha );
      }*/
   }

float SceneNodeProperties::GetAlpha( void ) const 
   { 
   /*if( m_pMaterial )
      { 
      return m_pMaterial->GetAlpha(); 
      } */
   return fOPAQUE; 
   }

SceneNode::SceneNode( ActorId actorId, IRenderComponent* pRenderComponent, RenderGroup renderPass, TransformPtr pNewTransform )
   {
   m_pParent = NULL;
	m_Props.m_ActorId = actorId;
   m_Props.m_Name = ( pRenderComponent ) ? pRenderComponent->VGetName() : "SceneNode";
	m_Props.m_RenderGroup = renderPass;
	//m_Props.m_AlphaType = AlphaOpaque;
   m_pRenderComponent = pRenderComponent;
   m_Props.m_pTransform = pNewTransform;
   *m_Props.m_pGlobalTransform = *pNewTransform; // copy value from local transform
   m_Props.m_Radius = 0.f;
   }

SceneNode::~SceneNode()
   {
   }

void SceneNode::VSetTransformPtr( TransformPtr pNewTransform )
   {
   m_Props.m_pTransform = pNewTransform;
   }

void SceneNode::VSetTransform( const Transform& newTransform )
   {
   *m_Props.m_pTransform = newTransform;
   }

TransformPtr SceneNode::VGetGlobalTransformPtr( void ) const
   {
   return m_Props.m_pGlobalTransform;
   /*if( !m_pParent )
      {
      return m_Props.GetLocalTransform();
      }
   Transform& ret = m_pParent->VGetGlobalTransform();
   ret = ret * m_Props.GetLocalTransform();
   return ret;*/
   }

int SceneNode::VOnRestore( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      it->VOnRestore( pScene );
      }
   return S_OK;
   }

int SceneNode::VPreUpdate( Scene *pScene )
   {
   // No parent, copy its local transform to global transform directly
   if( !m_pParent )
      {
      ( *m_Props.m_pGlobalTransform ) = ( *( m_Props.m_pTransform ) );
      }
   else
      {
      ( *m_Props.m_pGlobalTransform ) = ( *m_pParent->VGetGlobalTransformPtr() ) * ( *( m_Props.m_pTransform ) );
      }

   int ret = S_OK;

   for( auto it : m_Children )
      {
      ret = it->VPreUpdate( pScene );
      if( ret != S_OK )
         {
         return ret;
         }
      }

   return ret;
   }

int SceneNode::VOnUpdate( Scene* pScene, float deltaSeconds )
   {
   int ret = VDelegateUpdate( pScene, deltaSeconds );
 
   if( ret != S_OK )
      {
      return ret;
      }

   for( auto it : m_Children )
      {
      ret = it->VOnUpdate( pScene, deltaSeconds );
      if( ret != S_OK )
         {
         return ret;
         }
      }
   return S_OK;
   }

int SceneNode::VPreRender( Scene *pScene )
   {
  // pScene->PushAndSetTransform( m_Props.m_pTransform );
   return S_OK;
   }

bool SceneNode::VIsVisible( Scene *pScene )
   {
   auto camTransform = pScene->GetCamera()->VGetGlobalTransformPtr();
   Vec3 nodeInCamWorldPos = VGetGlobalPosition();
   // transform to camera's local space
   nodeInCamWorldPos = camTransform->GetFromWorld().Xform( nodeInCamWorldPos );;
   const PerspectiveFrustum &frustum = pScene->GetCamera()->GetFrustum();
   //return true;
   return frustum.VInside( nodeInCamWorldPos, m_Props.GetRadius() );
   }


int SceneNode::VRenderChildren( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      if( it->VPreRender( pScene ) == S_OK )
         {
         if( it->VIsVisible( pScene ) )
            {
            float alpha = it->VGetProperties().GetAlpha( );
            if( alpha == fOPAQUE )
               {
               it->VRender( pScene );
               }
            else if( alpha != fTRANSPARENT )
               {
               // The object isn't totally transparent...
               AlphaSceneNode *asn = ENG_NEW AlphaSceneNode;
               ENG_ASSERT( asn );
               asn->m_pNode = it;
               asn->m_GlobalToWorld = VGetGlobalTransformPtr()->GetToWorld();

               Vec4 worldPos( asn->m_GlobalToWorld.GetToWorldPosition() );

               Mat4x4 fromWorld = pScene->GetCamera()->VGetGlobalTransformPtr()->GetFromWorld();

               Vec4 screenPos = fromWorld.Xform( worldPos );

               asn->m_ScreenZ = screenPos.z;

               pScene->AddAlphaSceneNode( asn );
               }
            }
         it->VRenderChildren( pScene );
         }
      it->VPostRender( pScene );
      }
   return S_OK;
   }

int SceneNode::VPostRender( Scene *pScene )
   {
 //  pScene->PopTransform();
   return S_OK;
   }

bool SceneNode::VAddChild( shared_ptr<ISceneNode> child )
   {
   m_Children.push_back( child );
   // child position in parent's space
   Vec3 childPos = child->GetToWorldPosition();
   float newRadius = childPos.Length() + child->VGetProperties().GetRadius();
   m_Props.m_Radius = std::max( m_Props.m_Radius, newRadius );
   return true; 
   }

bool SceneNode::VRemoveChild( ActorId id )
   {
   bool success = false;
   for( auto it = m_Children.begin(); it != m_Children.end(); /*++it */)
	   {
		ActorId childId = (*it)->VGetProperties().GetActorId();
      if( childId != INVALID_ACTOR_ID && childId == id )
		   {
         shared_ptr< IEvent > pEvent( ENG_NEW Event_Destroy_Scene_Node( *it ) );
         EventManager::GetSingleton()->VTriggerEvent( pEvent );
         ( *it )->VRemoveChild( id );
			it = m_Children.erase( it );	//this can be expensive for vectors
         success = true;
		   }
      else 
         {
         ++it;
         }
	   }
	return success;
   }

int SceneNode::VOnLostDevice( Scene *pScene )
   {
   for( auto it : m_Children )
      {
      it->VOnLostDevice( pScene );
      }
   return S_OK;
   }

void SceneNode::SetAlpha( float alpha )
   {
   for( auto it : m_Children )
      {
      // downward cast, is it safe?
      shared_ptr<SceneNode> sceneNode = static_pointer_cast<SceneNode>( it );
      sceneNode->SetAlpha( alpha );
      }
   m_Props.SetAlpha( alpha );
   }

// Sum up relative position from child to root node in order to get position in world space
Vec3 SceneNode::VGetGlobalPosition( void ) const
   {
   return VGetGlobalTransformPtr()->GetToWorldPosition();
   }


RootNode::RootNode() : SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_0 )
   {
	m_Children.reserve(RenderGroup_Last);

   shared_ptr<SceneNode> staticGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_Static ) );
	m_Children.push_back(staticGroup);	// RenderGroup_Static = 0

   shared_ptr<SceneNode> actorGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_Actor ) );
	m_Children.push_back(actorGroup);	// RenderGroup_Actor = 1

   shared_ptr<SceneNode> skyGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_Sky ) );
	m_Children.push_back(skyGroup);	// RenderGroup_Sky = 2

   shared_ptr<SceneNode> invisibleGroup( ENG_NEW SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_NotRendered ) );
	m_Children.push_back(invisibleGroup);	// RenderGroup_NotRendered = 3
   }
   

// add child into corresponded render group node
bool RootNode::VAddChild( shared_ptr<ISceneNode> child )
   {
   RenderGroup pass = child->VGetProperties().GetRenderGroup();
	if ( (unsigned)pass >= m_Children.size() || !m_Children[pass] )
	   {
		ENG_ASSERT(0 && _T("There is no such render pass"));
		return false;
	   }

	return m_Children[pass]->VAddChild( child );
   }

int RootNode::VRenderChildren( Scene *pScene )
   {
   // the child node of Root nodes are empty node, so there's no need calling VRender for them
   // only their children should be rendered
   for (int pass = RenderGroup_0; pass < RenderGroup_Last; ++pass)
	   {
		switch(pass)
		   {
			case RenderGroup_Static:
			case RenderGroup_Actor:
 				m_Children[pass]->VRenderChildren(pScene);
				break;

			case RenderGroup_Sky:
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

CameraNode::CameraNode( TransformPtr pTransform, PerspectiveFrustum const &frustum ) 
	      : SceneNode( INVALID_ACTOR_ID, NULL, RenderGroup_0, pTransform ),
	      m_Frustum( frustum ),
	      m_IsActive( true ),
	      m_IsDebugCamera( false ),
         m_pTarget( shared_ptr<SceneNode>() ),
	      m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ),
         m_View( Mat4x4::LookAt( pTransform->GetToWorldPosition(), pTransform->GetToWorldPosition() + pTransform->GetForward(), g_Up ) )
   {
   }

CameraNode::CameraNode( const Vec3& eye, const Vec3& center, const Vec3& up, PerspectiveFrustum const &frustum ) 
	      : SceneNode( INVALID_ACTOR_ID, 
                      NULL,
                      RenderGroup_0, 
                      TransformPtr( ENG_NEW Transform( Mat4x4::LookAtToTransform( eye, center, up ).Inverse() ) )
                      ),
            m_Frustum( frustum ),
	         m_IsActive( true ),
	         m_IsDebugCamera( false ),
            m_pTarget( shared_ptr<SceneNode>() ),
	         m_CamOffsetVector( 0.0f, 1.0f, -10.0f, 0.0f ),
            m_View( Mat4x4::LookAt( eye, center, up ) )
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
   m_Projection.BuildProjection( m_Frustum.m_FovY, m_Frustum.m_Aspect, m_Frustum.m_NearDis, m_Frustum.m_FarDis );
	return S_OK;
   }

void CameraNode::VSetTransform( const Transform& newTransform ) 
   { 
   SceneNode::VSetTransform( newTransform ); 
  // auto globalTransformPtr = VGetGlobalTransformPtr();
   //m_View = Mat4x4::LookAt( globalTransformPtr->GetToWorldPosition(), globalTransformPtr->GetToWorldPosition() + globalTransformPtr->GetForward(), globalTransformPtr->GetUp() );

   }

//Mat4x4 CameraNode::GetWorldViewProjection( Scene *pScene )
//   {
//   const Mat4x4& world = pScene->GetTopMatrix();
//	return m_Projection * m_View * world;
//   }

// TODO: check if it's correct
// I think atWorld should be set by GetWorldPosition
int CameraNode::SetViewTransform(  Scene *pScene )
   {
   auto globalTransformPtr = VGetGlobalTransformPtr();
   m_View = Mat4x4::LookAt( globalTransformPtr->GetToWorldPosition(), globalTransformPtr->GetToWorldPosition() + globalTransformPtr->GetForward(), globalTransformPtr->GetUp() );

   // this code simulates camera pole effect, camera is sticked to target's position +  m_CamOffsetVector
	if( m_pTarget )
	   {
      // Get target's local transform
		//Mat4x4 targetInParentSpace = m_pTarget->VGetProperties()->GetToWorld();
		//Vec4 at = m_CamOffsetVector;
		//Vec4 atWorld = targetInParentSpace.Xform( at );
  //    // 
		//Vec3 pos = m_pTarget->GetToWorldPosition() + at;
		//targetInParentSpace.SetToWorldPosition( pos );
		//VSetTransform( &targetInParentSpace );
	   }

	//m_View = VGetProperties()->GetFromWorld();

	return S_OK;
   }

void CameraNode::GetScreenProjectPoint( Vec3& start, Vec3& end, const Point& screenPosition, float distance )
   {
   float revScreenLocX = ( ( float ) screenPosition.GetX() / ( float ) g_pApp->GetScreenSize().GetX() - 0.5f ) * 2.0f;
   float revScreenLocY = ( ( float ) screenPosition.GetY() / ( float ) g_pApp->GetScreenSize().GetY() - 0.5f ) * -2.0f;
   // The near plane maps to Z=-1 in Normalized Device Coordinates
   
   Mat4x4 invV = m_View.Inverse();
   Mat4x4 invP = m_Projection.Inverse();
   
   Mat4x4 inv_MP = Mat4x4( m_Projection * m_View ).Inverse();

   Vec4 rayStart_NDC = Vec4( revScreenLocX, revScreenLocY, -1.0f, 1.0f );
   Vec4 rayStart_Wor = inv_MP.Xform( rayStart_NDC );
   rayStart_Wor /= rayStart_Wor.w;
   start = rayStart_Wor;

   Vec4 rayEnd_NDC = Vec4( revScreenLocX, revScreenLocY, 0.0f, 1.0f );
   Vec4 rayEnd_Wor = inv_MP.Xform( rayEnd_NDC );
   rayEnd_Wor /= rayEnd_Wor.w;
   Vec3 dir = rayEnd_Wor - rayStart_Wor;
   dir.Normalize();
   end = start + dir * distance;
   }









