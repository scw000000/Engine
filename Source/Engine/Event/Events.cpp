////////////////////////////////////////////////////////////////////////////////
// Filename: Events.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Events.h"

const EventType BaseEventData<EvtData_Destroy_Actor>::s_EventType( 0x220b94dd );
const char* BaseEventData<EvtData_Destroy_Actor>::s_pName( "EvtDat_Destyot_Actor" );

const EventType BaseEventData<EvtData_Move_Actor>::s_EventType( 0xddf0765c );
const char* BaseEventData<EvtData_Move_Actor>::s_pName( "EvtData_Move_Actor" );

const EventType BaseEventData<EvtData_New_Render_Component>::s_EventType( 0xa75361ae );
const char* BaseEventData<EvtData_New_Render_Component>::s_pName( "EvtData_New_Render_Component" );



 