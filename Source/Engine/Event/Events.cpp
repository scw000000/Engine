/*!
 * \file Events.cpp
 * \date 2016/06/14 14:32
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
#include "Events.h"

const EventType BaseEvent<Event_Destroy_Actor>::s_EventType( 0x220b94dd );
const char* BaseEvent<Event_Destroy_Actor>::s_pName( "Event_Destroy_Actor" );

const EventType BaseEvent<Event_Move_Actor>::s_EventType( 0xddf0765c );
const char* BaseEvent<Event_Move_Actor>::s_pName( "Event_Move_Actor" );

const EventType BaseEvent<Event_New_Render_Component_Root>::s_EventType( 0xa75361ae );
const char* BaseEvent<Event_New_Render_Component_Root>::s_pName( "Event_New_Render_Component_Root" );

const EventType BaseEvent<Event_New_Scene_Node>::s_EventType( 0x915c74ec );
const char* BaseEvent<Event_New_Scene_Node>::s_pName( "Event_New_Scene_Node" );

