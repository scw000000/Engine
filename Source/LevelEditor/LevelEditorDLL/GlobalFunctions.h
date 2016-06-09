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

#define DllExport _declspec(dllexport)

extern "C" DllExport int EditorMain( int *pWindow, int screenWidth, int screenHeight );
extern "C" DllExport void SingleLoop( void );
extern "C" DllExport void PushSDLEvent( int* event );
extern "C" DllExport void Shutdown( void );