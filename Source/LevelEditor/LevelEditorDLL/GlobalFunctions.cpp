/*!
* \file GlobalFunctions.cpp
* \date 2016/05/26 8:49
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

#include "EditorStd.h"
#include "GlobalFunctions.h"
#include "..\Main\Engine.h"

EngineApp engineApp;

int EditorMain( int *pWindow, int screenWidth, int screenHeight )
   {
   // Set up checks for memory leaks.
   // Game Coding Complete reference - Chapter 21, page 834
   //
   int tmpDbgFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

   // set this flag to keep memory blocks around
   // tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

   // perform memory check for each alloc/dealloc
   //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

   /*
   _CRTDBG_LEAK_CHECK_DF is used at program initialization to force a
   //   leak check just before program exit. This is important because
   //   some classes may dynamically allocate memory in globally constructed
   //   objects.
   */
   tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

   _CrtSetDbgFlag( tmpDbgFlag );

   Logger::Init( "logging.xml" );

   g_pApp->m_EngineOptions.Init( "EditorOptions.xml" );
   g_pApp->m_EngineOptions.m_ScreenSize.x = screenWidth;
   g_pApp->m_EngineOptions.m_ScreenSize.y = screenHeight;

   g_pApp->InitInstance( ( SDL_Window* )pWindow, g_pApp->m_EngineOptions.m_ScreenSize.x, g_pApp->m_EngineOptions.m_ScreenSize.y );

  // g_pApp->MainLoop();

  // Logger::Destroy();

   return true;
   }

void SingleLoop( void )
   {
   g_pApp->SingleLoop();
   }
