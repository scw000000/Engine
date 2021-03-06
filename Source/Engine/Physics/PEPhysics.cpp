/*!
 * \file PEPhysics.cpp
 * \date 2017/09/16 22:12
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "EngineStd.h"
#include "PEPhysics.h"
#include "RigidBody.h"
#include "Colliders.h"
#include "PEPhysicsAttributes.h"
#include "RigidBodySolver.h"
#include "BroadPhase.h"
#include "..\Graphics\BasicGeometry.h"
#include "..\ResourceCache\XmlResource.h"

PEPhysics::~PEPhysics( void )
   {
   m_RenderCompToRigidBody.clear();
   m_RigidBodyToRenderComp.clear();
   }

PEPhysics::PEPhysics( void )
   {
   m_IsSimulating = false;

   }

void PEPhysics::LoadConfig()
   {
   // Load the physics config file and grab the root XML node
   shared_ptr< Resource > pPhysicsRes( ENG_NEW Resource( "config\\Physics.xml", g_pApp->m_EngineOptions.GetIsUsingDevDirectory() ) );

   TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( pPhysicsRes );
   ENG_ASSERT( pRoot );

   // load all materials
   TiXmlElement* pParentNode = pRoot->FirstChildElement( "PhysicsMaterials" );
   ENG_ASSERT( pParentNode );
   for( TiXmlElement* pNode = pParentNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
      {
      double restitution = 0;
      double friction = 0;
      pNode->Attribute( "restitution", &restitution );
      pNode->Attribute( "friction", &friction );
      m_MaterialTable.insert( std::make_pair( pNode->Value(), MaterialData( ( float ) restitution, ( float ) friction ) ) );
      }

   // load all densities
   pParentNode = pRoot->FirstChildElement( "DensityTable" );
   ENG_ASSERT( pParentNode );
   for( TiXmlElement* pNode = pParentNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement() )
      {
      // Insert ( substance name, substance density )
      m_DensityTable.insert( std::make_pair( pNode->Value(), ( float ) atof( pNode->FirstChild()->Value() ) ) );
      }
   pParentNode = pRoot->FirstChildElement( "CollisionTable" );
   ENG_ASSERT( pParentNode );

   ENG_ASSERT( CollisionTable::GetSingleton().Init( pParentNode ) );

   }

bool PEPhysics::VInitialize()
   {
   m_pCollisionDetector = shared_ptr< CollisionDetector >( ENG_NEW CollisionDetector() );
   m_pRigidBodySolver = shared_ptr< RigidBodySolver >( ENG_NEW RigidBodySolver() );
   m_pBroadphase = shared_ptr< Broadphase >( ENG_NEW Broadphase() );
   LoadConfig();
   return true;
   }

void PEPhysics::VSyncVisibleScene()
   {

   }
void PEPhysics::VSyncRigidBodyToRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT( pRenderComp );
   auto renderCompToBodyIt = m_RenderCompToRigidBody.find( pRenderComp );
   ENG_ASSERT( renderCompToBodyIt != m_RenderCompToRigidBody.end() );
   TransformPtr pComponentTransform = pRenderComp->VGetTransformPtr();

   renderCompToBodyIt->second->VSetWorldTransform( *pComponentTransform );
   }

void PEPhysics::VOnUpdate( const float deltaSeconds )
   {
   if( !m_IsSimulating )
      {
      return;
      }
   static int it = 0;
   m_pBroadphase->VUpdate( 0.1f );
   // Update manifolds
   // m_Manifolds.clear();
   m_pBroadphase->VCalcualteCollisionPairs();
   auto collisionPairs = m_pBroadphase->VGetCollisionPairs();

#ifdef _DEBUG
   // broad phase correctness test
   if( collisionPairs.size() == 0 )
      {
      // ENG_LOG("Test", " NO");
      ContactPoint cp;
      auto it = m_RigidBodyToRenderComp.begin();

      auto next = it;
      ++next;
      if( m_pCollisionDetector->CollisionDetection(
         it->first ,
         next->first,
         cp
         ) )
         {
         if( cp.m_PenetrationDepth > 0.01f )
            {
            m_pBroadphase->VUpdate( 0.1f );
            m_pBroadphase->VCalcualteCollisionPairs();
            }
         }
      }
#endif
   std::unordered_map< shared_ptr<RigidBody>, std::unordered_map<shared_ptr<RigidBody>, ContactPoint> > currCollisions;
   shared_ptr<RigidBody> pRB;
   for( auto& pair : collisionPairs ) 
      {
      ContactPoint contact;
      // get contact point
      if( m_pCollisionDetector->CollisionDetection(
         pair.first,
         pair.second,
         contact
         ) )
         {
         /*manifold.pRigidBodyA = pair.first;
         manifold.pRigidBodyB = pair.second;
         pRB = manifold.pRigidBodyA;
         manifold.CalculateCombinedRestitution();
         manifold.CalculateCombinedFriction();
         */
         // ENG_LOG( "Test", std::string( "PD: " ) + ToStr( manifold.m_ContactPoints[ 0 ].m_PenetrationDepth ) );
         
         // ENG_LOG( "Test", std::string( "N: " ) + ToStr( manifold.m_ContactPoints[ 0 ].m_Normal ) );
         // ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointA ) );
         //ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointB ) );
         // m_Manifolds.push_back( manifold );
         // Insert the points into manifold, Check if manifold exist, if not create one
         // and push it into it
         // record colliding pairs
         currCollisions[ pair.first ][ pair.second ] = contact;
         }
      }
   // Remove all non-colliding manifolds based on the record
   int validIdx = m_Manifolds.size() - 1;
   int invalidIdx = 0;
   while( invalidIdx <= validIdx )
      {
      // find first invalid idx from left to right
      while(invalidIdx <= validIdx )
         {
         // if the index is invalid, break
         auto findIt = currCollisions.find( m_Manifolds[ invalidIdx ].m_pRigidBodyA );
         if( findIt == currCollisions.end()
             || findIt->second.find( m_Manifolds[ invalidIdx ].m_pRigidBodyB ) == findIt->second.end() )
            {
            break;
            }
         // update and eliminate hash table
         auto& cp = findIt->second[ m_Manifolds[ invalidIdx ].m_pRigidBodyB ];
         m_Manifolds[ invalidIdx ].AddContactPoint( cp );
         findIt->second.erase( m_Manifolds[ invalidIdx ].m_pRigidBodyB );
         if(findIt->second.size() == 0)
            {
            currCollisions.erase( findIt );
            }
         
         ++invalidIdx;
         }

      while( invalidIdx < validIdx )
         {
         // if it's valid, break
         auto findIt = currCollisions.find( m_Manifolds[ validIdx ].m_pRigidBodyA );
         if( findIt != currCollisions.end() && findIt->second.find( m_Manifolds[ validIdx ].m_pRigidBodyB ) != findIt->second.end() )
            {
            // update and eliminate hash table
            auto& cp = findIt->second[ m_Manifolds[ validIdx ].m_pRigidBodyB ];
            m_Manifolds[ validIdx ].AddContactPoint( cp );
            findIt->second.erase( m_Manifolds[ validIdx ].m_pRigidBodyB );
            if( findIt->second.size() == 0 )
               {
               currCollisions.erase( findIt );
               }
            break;
            }
         --validIdx;
         }
      if( invalidIdx >= validIdx )
         {
         m_Manifolds.resize( invalidIdx );
         break;
         }

      std::swap( m_Manifolds[ invalidIdx ], m_Manifolds[ validIdx ] );
      ++invalidIdx;
      --validIdx;
      }
   // for those contacts which still exist, create new manifold for them
   for( auto itA : currCollisions )
      {
      for( auto itCP : itA.second )
         {
         Manifold manifold;
         manifold.AddContactPoint( itCP.second );
         manifold.m_pRigidBodyA = itA.first;
         manifold.m_pRigidBodyB = itCP.first;
         manifold.CalculateCombinedRestitution();
         manifold.CalculateCombinedFriction();
         m_Manifolds.push_back( manifold );
         }
      }
   


   //for( auto leftPair = m_RigidBodyToRenderComp.begin(); leftPair != m_RigidBodyToRenderComp.end(); ++leftPair )
   //   {
   //   for( auto rightPair = std::next( leftPair, 1 ); rightPair != m_RigidBodyToRenderComp.end(); ++rightPair )
   //      {
   //      Manifold manifold;
   //      if( m_pCollisionDetector->CollisionDetection(
   //         leftPair->first,
   //         rightPair->first,
   //         manifold
   //         ) )
   //         {
   //         manifold.pRigidBodyA = leftPair->first;
   //         manifold.pRigidBodyB = rightPair->first;
   //         ENG_LOG( "Test", std::string( "Penetration depth: " ) + ToStr( manifold.m_ContactPoints[ 0 ].m_PenetrationDepth ) );
   //         // ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointA ) );
   //         //ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointB ) );
   //         m_Manifolds.push_back( manifold );
   //         }
   //      /*ENG_ASSERT( m_pCollisionDetector->CollisionDetection(
   //      leftPair->first,
   //      rightPair->first,
   //      manifold
   //      ) == false );*/
   //      }
   //   }

   // update velocity
   // should I move it before collision detection?
   for( auto& pair : m_RigidBodyToRenderComp )
      {
      pair.first->m_Force = Vec3::g_Zero;
      ApplyGravity( pair.first, 0.1f );
      //if( true || i++ == 0 )
      //   {
      //   
      //   }
      //else
      //   {
      //   // ENG_LOG( "Test", std::string("loc: ") + ToStr( pair.first->m_GlobalCentroid) );
      //  // ENG_LOG("Test", ToStr( pair.first->m_LinearVelocity ));
      //   }
      pair.first->VUpdateVelocity( 0.1f );
      }

   // Solve constraint
   m_pRigidBodySolver->SolveConstraint( m_Manifolds, 0.1f );
   if( pRB )
      {
      /*ENG_LOG( "Test", std::string( "V: " ) + ToStr( pRB->m_LinearVelocity ) );
      ENG_LOG( "Test", std::string( "AV: " ) + ToStr( pRB->m_AngularVelocity ) );
      */
      }
   // Update transform
   for( auto& pair : m_RigidBodyToRenderComp )
      {
      // pair.first->MoveForOneTimeStep( deltaSeconds );
      pair.first->VMoveForOneTimeStep( 0.1f );
      }
   ++it;
   }

// Initialization of Physics Objects
void PEPhysics::VAddSphere( float radius, StrongRenderComponentPtr pRenderComp )
   {

   }

void PEPhysics::VAddBox( const Vec3& dimensions, StrongRenderComponentPtr pRenderComp )
   {
  
   }

void PEPhysics::VAddPointCloud( Vec3 *verts,
                             int numPoints,
                             StrongRenderComponentPtr pRenderComp )
   {
 
   }

void PEPhysics::VRemoveRenderComponent( StrongRenderComponentPtr pRenderComp )
   {
   ENG_ASSERT( pRenderComp );
   if( auto pBody = FindRigidBody( pRenderComp ) )
      {
      // destroy the body and all its components
      // RemoveCollisionObject( pBody );
      m_RenderCompToRigidBody.erase( pRenderComp );
      m_RigidBodyToRenderComp.erase( pBody );
      m_pBroadphase->VRemoveRigidBody( pBody );
      }
   }

void PEPhysics::VRenderDiagnostics( void )
   {
   //if( !m_IsSimulating )
   //   {
   // //  return;
   //   }
   //
   auto pScene = g_pApp->m_pEngineLogic->m_pWrold;
   auto pv = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView();
   auto identity = Mat4x4::g_Identity;
   identity.MultScale( Vec3( 0.1f, 0.1f, 0.1f ) );
   for(auto& mapPair : m_RigidBodyToRenderComp)
      {
      auto m = mapPair.first->m_Transform.GetToWorld();
      identity.SetToWorldPosition( mapPair.first->m_GlobalCentroid );

      SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Yellow, pv * identity );
      for(auto& collider : mapPair.first->m_Colliders ){
         collider->VRenderShape( m, pv );
         }
      }

   // Draw contact points
   Mat4x4 m;
   m.MultScale( Vec3( 0.1f, 0.1f, 0.1f ) );
   // Draw contact points
   for( auto& manifold : m_Manifolds )
      {
         // ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_PenetrationDepth ) );
         // ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointA ) );
         //ENG_LOG( "Test", ToStr( manifold.m_ContactPoints[ 0 ].m_SupportPoint.m_PointB ) );

      for( int i = 0; i < manifold.m_ContactPointCount; ++i )
         {
         m.SetToWorldPosition( manifold.m_ContactPoints[ i ].m_PointAWS );
         SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Red, pv * m );
         m.SetToWorldPosition( manifold.m_ContactPoints[ i ].m_PointBWS );
         SBasicGeometry::GetSingleton().RenderGeometry( BasicGeometry::GeometryTypes_Sphere, g_Green, pv * m );
         }
      }
   } 

void PEPhysics::VCreateTrigger( WeakActorPtr pActor, const Vec3 &pos, const float dim ) 
   {

   }

void PEPhysics::VApplyForce( const Vec3 &dir, float newtons, ActorId actorId ) 
   {

   }

void PEPhysics::VApplyTorque( const Vec3 &dir, float newtons, ActorId actorId ) 
   {

   }

bool PEPhysics::VKinematicMove( const Transform &trans, ActorId actorId ) 
   {
   return true; 
   }

void PEPhysics::VRotateY( ActorId actorId, float angleRadians, float time )
   {
   }

float PEPhysics::VGetOrientationY( ActorId actorId ) 
   {
   return 0.f; 
   }

void PEPhysics::VStopActor( ActorId actorId )
   {

   }

Vec3 PEPhysics::VGetVelocity( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Vec3();
      }
   return pRigidBody->m_LinearVelocity;
   }

void PEPhysics::VSetVelocity( ActorId actorId, const Vec3& vel )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->m_LinearVelocity = vel;
   }

Vec3 PEPhysics::VGetAngularVelocity( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Vec3();
      }
   return pRigidBody->m_AngularVelocity;
   }

void PEPhysics::VSetAngularVelocity( ActorId actorId, const Vec3& vel )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->m_AngularVelocity = vel;
   }

void PEPhysics::VTranslate( ActorId actorId, const Vec3& vec )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }

   }

void PEPhysics::VSetTransform( const ActorId actorId, const Transform &trans )
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return;
      }
   pRigidBody->VSetWorldTransform( trans );
   }

Transform PEPhysics::VGetTransform( ActorId actorId ) 
   {
   auto pRigidBody = FindRigidBody( actorId );
   ENG_ASSERT( pRigidBody );
   if( !pRigidBody )
      {
      return Transform();
      }
   return pRigidBody->m_Transform;
   }

void PEPhysics::VLinkRenderCompAttribute( StrongRenderComponentPtr pRenderComp ) 
   {
   auto pPhtsicsAttr = pRenderComp->VGetPhysicsAttributes();
   auto pPhyAttr = dynamic_pointer_cast< PEPhysicsAttributes >( pPhtsicsAttr );
   ENG_ASSERT( pPhyAttr );
   shared_ptr<RigidBody> pRigidBody = FindRigidBody( pRenderComp );
   ENG_ASSERT( pRigidBody );
   pPhyAttr->VSetRigidBody( pRigidBody );
   pPhyAttr->VSetIsLinkedToPhysicsWorld( true );
   }

void PEPhysics::VDestroyRenderCompDelegate( IEventPtr pEvent ) 
   {

   }

shared_ptr<RigidBody> PEPhysics::FindRigidBody( StrongRenderComponentPtr pRenderComp ) const
   {
   auto pIterater = m_RenderCompToRigidBody.find( pRenderComp );
   if( pIterater != m_RenderCompToRigidBody.end() )
      {
      return pIterater->second;
      }
   return NULL;
   }

shared_ptr<RigidBody> PEPhysics::FindRigidBody( ActorId actorId ) const
   {
   StrongActorPtr pStrongActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   ENG_ASSERT( pStrongActor );
   StrongRenderComponentPtr pRenderComp = dynamic_pointer_cast< IRenderComponent >( pStrongActor->GetComponent( 0 ).lock() );
   return FindRigidBody( pRenderComp );
   }

StrongRenderComponentPtr PEPhysics::FindRenderComponent( shared_ptr<RigidBody> pRigidBody ) const
   {
   auto pIterater = m_RigidBodyToRenderComp.find( pRigidBody );
   if( pIterater != m_RigidBodyToRenderComp.end() )
      return pIterater->second;

   return StrongRenderComponentPtr();
   }

void PEPhysics::VAddRigidBody( StrongRenderComponentPtr pRenderComp, shared_ptr<RigidBody> pRB )
   {
   if( m_RenderCompToRigidBody.find( pRenderComp ) != m_RenderCompToRigidBody.end()
       || m_RigidBodyToRenderComp.find( pRB ) != m_RigidBodyToRenderComp.end() )
      {
      ENG_ERROR( "This render comp is already registed!" );
      }

   m_RenderCompToRigidBody[ pRenderComp ] = pRB;
   m_RigidBodyToRenderComp[ pRB ] = pRenderComp;
   m_pBroadphase->VAddRigidBody( pRB );
   pRB->VUpdateRigidBodyInfo();
   VLinkRenderCompAttribute( pRenderComp );
   }

bool PEPhysics::QueryDensity( const std::string& substance, float& returnVal ) const 
   {
   auto findIt = m_DensityTable.find( substance );
   if(findIt == m_DensityTable.end())
      {
      return false;
      }      
   returnVal = findIt->second;

   return true;
   }

bool PEPhysics::QueryMaterialData( const std::string& material, MaterialData& returnVal ) const 
   {
   auto findIt = m_MaterialTable.find( material );
   if( findIt == m_MaterialTable.end() )
      {
      return false;
      }
   returnVal = findIt->second;

   return true;
   }

void PEPhysics::ApplyGravity( shared_ptr<RigidBody> pRigidBody, float deltaSeconds )
   {
   pRigidBody->VApplyForce( pRigidBody->m_GravityScale *  Vec3( 0.f, -9.8f, 0.f ) );
   }