/*!
 * \file PhysicsEvents.cpp
 * \date 2016/05/07 14:03
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
#include "PhysicsEvents.h"
#include "..\LuaScripting\LuaStateManager.h"

const EventType BaseEvent<EvtData_PhysTrigger_Enter>::s_EventType( 0x534fc6f6 );
const char* BaseEvent<EvtData_PhysTrigger_Enter>::s_pName( "EvtData_PhysTrigger_Enter" );

const EventType BaseEvent<EvtData_PhysTrigger_Leave>::s_EventType( 0x9be8c9a5 );
const char* BaseEvent<EvtData_PhysTrigger_Leave>::s_pName( "EvtData_PhysTrigger_Leave" );

const EventType BaseScriptEvent<EvtData_PhysCollision>::s_EventType( 0x1d26a043 );
const char* BaseScriptEvent<EvtData_PhysCollision>::s_pName( "EvtData_PhysCollision" );

const EventType BaseEvent<EvtData_PhysSeparation>::s_EventType( 0x94ab2d35 );
const char* BaseEvent<EvtData_PhysSeparation>::s_pName( "EvtData_PhysSeparation" );

void EvtData_PhysCollision::VBuildScriptEventData( void )
   {
   m_LuaEventData.AssignNewTable( LuaStateManager::GetSingleton( )->GetLuaState( ) );
   m_LuaEventData.SetInteger( "actorA", m_ActorA );
   m_LuaEventData.SetInteger( "actorB", m_ActorB );
   }