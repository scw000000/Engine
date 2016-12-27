/*!
 * \file Scene.cpp
 * \date 2016/05/22 11:19
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
#include "Scene.h"
#include "..\Event\EventManager.h"
#include "FastDelegate.h"

Scene::Scene( shared_ptr< IRenderManager > pRenderManager ) : m_pRoot( ENG_NEW RootNode ), m_pLightManager( ENG_NEW LightManager )
   {
   m_pRenderManager = pRenderManager;
	
   IEventManager* pEventMgr = IEventManager::GetSingleton();
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &Scene::NewRenderComponentDelegate ), Event_New_Render_Root_Component::s_EventType );
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &Scene::DestroyActorDelegate ), Event_Destroy_Actor::s_EventType);
   //pEventMgr->VAddListener(fastdelegate::MakeDelegate(this, &Scene::MoveActorDelegate), EvtData_Move_Actor::sk_EventType);
   //pEventMgr->VAddListener(fastdelegate::MakeDelegate(this, &Scene::ModifiedRenderComponentDelegate), EvtData_Modified_Render_Component::sk_EventType);
   }

//void Scene::Destory( void )
//   {
//   m_Root.reset( );
//   for( auto test : m_ActorMap )
//      {
//      test.second.reset();
//      }
//   m_ActorMap.clear( );
//   m_Camera.reset();
//   m_pRenderer.reset();
//   m_AlphaSceneNodes.clear();
//   m_pLightManager.reset();
//   }

Scene::~Scene()
   {
   
   }

int Scene::PreRender( void )
   {
   //if( m_Root && m_pCamera )
   //   {
   //   // The scene root could be anything, but it
   //   // is usually a SceneNode with the identity
   //   // matrix
   //   m_pCamera->SetViewTransform( this );
   //   m_pLightManager->CalcLighting( this );

   //   if( m_Root->VPreRender( this ) == S_OK )
   //      {
   //      m_Root->VRender( this );
   //      m_Root->VRenderChildren( this );
   //      m_Root->VPostRender( this );
   //      }
   //   RenderAlphaPass();
   //   }
   return S_OK;
   }

int Scene::OnRender( void )
   {
   if ( m_pRoot && m_pCamera )
	   {
		// The scene root could be anything, but it
		// is usually a SceneNode with the identity
		// matrix
		m_pCamera->SetViewTransform( this );
		m_pLightManager->CalcLighting( this );

		if ( m_pRoot->VPreRender( this ) == S_OK )
		   {
			m_pRoot->VRender( this );
			m_pRoot->VRenderChildren( this );
			m_pRoot->VPostRender( this );
		   }
		RenderAlphaPass();
	   }
	return S_OK;
   }

int Scene::OnRestore()
   {
   if ( !m_pRoot )
		return S_OK;

	//m_Renderer->VOnRestore() ;

	return m_pRoot->VOnRestore(this);
   }

int Scene::OnLostDevice()
   {
   if ( m_pRoot )
	   {
		return m_pRoot->VOnLostDevice(this);
	   }
	return S_OK;
   }

int Scene::PreUpdate( void )
   {
   if( !m_pRoot )
      return S_OK;

   return m_pRoot->VPreUpdate( this );
   }

int Scene::OnUpdate( unsigned long deltaMs )
   {
   PreUpdate();

   if ( !m_pRoot )
		return S_OK;

   static double lastTime = GetGlobalTimer()->GetTime();
   double now = GetGlobalTimer()->GetTime();
	unsigned long elapsedTime = (unsigned long) ( ( now - lastTime )* 1000.0 );
	lastTime = now;

	return m_pRoot->VOnUpdate( this, elapsedTime );
   }

shared_ptr< ISceneNode > Scene::FindSceneNode( ActorId id )
   {
   auto i = m_ActorMap.find( id );
	if (i == m_ActorMap.end())
	   {
		return shared_ptr<ISceneNode>();
	   }

	return i->second;
   }

bool Scene::AddChild( ActorId id, shared_ptr< ISceneNode > pNode )
   {
   if( id != INVALID_ACTOR_ID )
      {
      m_ActorMap[id] = pNode;
      }

   ////  If this node is light node, it is not renderable and it should be added to light manager
   //shared_ptr<LightNode> pLight = dynamic_pointer_cast< LightNode >( pNode );
   //if( pLight != NULL )
   //   {
   //   return m_pLightManager->AddLightNode( pLight );
   //   }

   return m_pRoot->VAddChild( pNode );
   }


bool Scene::RemoveChild( ActorId id )
   {
   if( id == INVALID_ACTOR_ID )
      {
      return false;
      }

   shared_ptr<ISceneNode> childNode = FindSceneNode( id );
   m_ActorMap.erase( id );
   return m_pRoot->VRemoveChild( id );
   }

void Scene::RenderAlphaPass()
   {
   m_pRenderManager->VGetMainRenderer().VSetRenderingAlpha( true );
   //MainRenderer::VSetRenderAlpha( true );
   // This it not implemented yet!
	m_AlphaSceneNodes.sort();
   // rendering from back to front in order to make visual effort of transparant objects
	while ( !m_AlphaSceneNodes.empty() )
	   {
		AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
	//	PushAndSetTransform( (*i)->m_GlobalToWorld );
		(*i)->m_pNode->VRender( this );
		SAFE_DELETE( *i );
	//	PopTransform();
		m_AlphaSceneNodes.pop_back();
	   }
   m_pRenderManager->VGetMainRenderer().VSetRenderingAlpha( false );
   }

void Scene::NewRenderComponentDelegate( IEventPtr pEvent )
   {
   shared_ptr<Event_New_Render_Root_Component> pDerivedEvent = dynamic_pointer_cast<Event_New_Render_Root_Component>( pEvent );

    ActorId actorId = pDerivedEvent->GetActorId();
    shared_ptr<SceneNode> pSceneNode( pDerivedEvent->GetSceneNode() );

    if ( pSceneNode->VOnRestore(this ) != S_OK )
      {
		std::string error = "Failed to restore scene node to the scene for actorid " + ToStr(actorId);
      ENG_ERROR( error );
      return;
      }

   AddChild( actorId, pSceneNode );
   }

void Scene::DestroyActorDelegate( IEventPtr pEvent )
   {
   shared_ptr<Event_Destroy_Actor> pDerivedEvent = dynamic_pointer_cast<Event_Destroy_Actor>( pEvent );

   RemoveChild( pDerivedEvent->GetId() );
   }