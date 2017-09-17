/*!
 * \file Physics.cpp
 * \date 2016/05/07 10:44
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
#include "Physics.h"

#include "btBulletDynamicsCommon.h"
#include "PhysicsDebugDrawer.h"
#include "CollisionTable.h"
#include "Raycast.h"

#include "..\ResourceCache\XmlResource.h"
#include "..\Event\Events.h"
#include "..\Event\PhysicsEvents.h"
#include "..\Event\EventManager.h"

IGamePhysics* g_pGamePhysics = NULL;

IGamePhysics::IGamePhysics( void )
   {
   ENG_ASSERT( !g_pGamePhysics );
   g_pGamePhysics = this;
   }

IGamePhysics& IGamePhysics::GetSingleton( void )
   {
   return *g_pGamePhysics;
   }

void IGamePhysics::Shutdown( void )
   {
   SAFE_DELETE( g_pGamePhysics );
   }
