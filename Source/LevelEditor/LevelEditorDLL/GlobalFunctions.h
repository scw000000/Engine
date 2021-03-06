#pragma once
/*!
 * \file GlobalFunctions.h
 * \date 2016/05/26 8:58
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
#include "wtypes.h"

#define DllExport _declspec(dllexport)

extern "C" DllExport int EditorMain( int *pWindow, int screenWidth, int screenHeight );
extern "C" DllExport void SingleLoop( void );
extern "C" DllExport void Shutdown( void );
extern "C" DllExport void LoadLevel( BSTR levelPath );
extern "C" DllExport ActorId CreateActor( BSTR classFilePath );
extern "C" DllExport unsigned int GetActorXmlSize( ActorId actorId );
extern "C" DllExport void GetActorXML( int *actorXMLAddress, ActorId actorId );
extern "C" DllExport ActorId PickActor( void );
extern "C" DllExport void ModifyActor( ActorId actorId, BSTR bstrActorModificationXML );
extern "C" DllExport void StartAndResumeEngine( void );
extern "C" DllExport void StopEngine( void );
extern "C" DllExport void SaveAllActors( void );
extern "C" DllExport void SaveActor( ActorId actorId );
extern "C" DllExport void SaveWorld( void );