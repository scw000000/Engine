////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Scene.h"
#include "..\Event\EventManager.h"
#include "FastDelegate.h"

Scene::Scene( shared_ptr<IRenderer> renderer )
   {
   m_Root = shared_ptr< SceneNode > ( ENG_NEW RootNode() );
	m_pRenderer = renderer;
	//m_LightManager = GCC_NEW LightManager;
   
   IEventManager* pEventMgr = IEventManager::GetSingleton();
   pEventMgr->VAddListener( fastdelegate::MakeDelegate( this, &Scene::NewRenderComponentDelegate ), EvtData_New_Render_Component::sk_EventType );
   //pEventMgr->VAddListener(fastdelegate::MakeDelegate(this, &Scene::DestroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
   //pEventMgr->VAddListener(fastdelegate::MakeDelegate(this, &Scene::MoveActorDelegate), EvtData_Move_Actor::sk_EventType);
   //pEventMgr->VAddListener(fastdelegate::MakeDelegate(this, &Scene::ModifiedRenderComponentDelegate), EvtData_Modified_Render_Component::sk_EventType);
   }

Scene::~Scene()
   {
   
   }

// TODO: move the post render to humanview, because it has GUIManager
int Scene::OnRender()
   {
   if ( m_Root && m_Camera )
	   {
		// The scene root could be anything, but it
		// is usually a SceneNode with the identity
		// matrix
		m_Camera->SetViewTransform( this );

	//	m_LightManager->CalcLighting(this);

		if ( m_Root->VPreRender( this ) == S_OK )
		   {
			m_Root->VRender( this );
			m_Root->VRenderChildren( this );
			m_Root->VPostRender( this );
		   }
		RenderAlphaPass();
	   }
	return S_OK;
   }

int Scene::OnRestore()
   {
   if (!m_Root)
		return S_OK;

	//m_Renderer->VOnRestore() ;

	return m_Root->VOnRestore(this);
   }

int Scene::OnLostDevice()
   {
   if (m_Root)
	   {
		return m_Root->VOnLostDevice(this);
	   }
	return S_OK;
   }

int Scene::OnUpdate( const unsigned long deltaMs )
   {
   if (!m_Root)
		return S_OK;

   static double lastTime = GetGlobalTimer()->GetTime();
   double now = GetGlobalTimer()->GetTime();
	unsigned long elapsedTime = (unsigned long) ( ( now - lastTime )* 1000.0 );
	lastTime = now;

	return m_Root->VOnUpdate( this, elapsedTime );
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

bool Scene::AddChild( ActorId id, shared_ptr< ISceneNode > sceneId )
   {
   if( id != INVALID_ACTOR_ID )
      {
      m_ActorMap[id] = sceneId;
      }
   return m_Root->VAddChild( sceneId );
   }


bool Scene::RemoveChild( ActorId id )
   {
   if( id == INVALID_ACTOR_ID )
      {
      return false;
      }

   shared_ptr<ISceneNode> childNode = FindSceneNode( id );
   m_ActorMap.erase( id );
   return m_Root->VRemoveChild( id );
   }

void Scene::RenderAlphaPass()
   {
 //  shared_ptr<IRenderState> alphaPass = m_Renderer->VPrepareAlphaPass();

	m_AlphaSceneNodes.sort();
   // rendering from back to front in order to make visual effort of transparant objects
	while ( !m_AlphaSceneNodes.empty() )
	   {
		AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
		PushAndSetMatrix( (*i)->m_Concat );
		(*i)->m_pNode->VRender(this);
		delete (*i);
		PopMatrix();
		m_AlphaSceneNodes.pop_back();
	   }
   }

void Scene::NewRenderComponentDelegate( IEventDataPtr pEventData )
   {
   shared_ptr<EvtData_New_Render_Component> pCastEventData = static_pointer_cast<EvtData_New_Render_Component>( pEventData );

    ActorId actorId = pCastEventData->GetActorId();
    shared_ptr<SceneNode> pSceneNode( pCastEventData->GetSceneNode() );

    // TODO: implement and call VOnInit instead of VOnRestore
    // because it will be called again in HumanView
    if ( pSceneNode->VOnRestore(this) != S_OK )
      {
		std::string error = "Failed to restore scene node to the scene for actorid " + ToStr(actorId);
      ENG_ERROR( error );
      return;
      }

   AddChild( actorId, pSceneNode );
   }