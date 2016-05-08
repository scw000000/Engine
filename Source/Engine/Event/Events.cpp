////////////////////////////////////////////////////////////////////////////////
// Filename: Events.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Events.h"

const EventType BaseEvent<EvtData_Destroy_Actor>::s_EventType( 0x220b94dd );
const char* BaseEvent<EvtData_Destroy_Actor>::s_pName( "EvtData_Destroy_Actor" );

const EventType BaseEvent<EvtData_Move_Actor>::s_EventType( 0xddf0765c );
const char* BaseEvent<EvtData_Move_Actor>::s_pName( "EvtData_Move_Actor" );

const EventType BaseEvent<EvtData_New_Render_Component>::s_EventType( 0xa75361ae );
const char* BaseEvent<EvtData_New_Render_Component>::s_pName( "EvtData_New_Render_Component" );



 