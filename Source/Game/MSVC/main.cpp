/*!
* \file main.cpp
* \date 2016/05/22 13:09
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
#include "GameStd.h"
#include "..\Main\Libs.h"

#pragma comment( lib, "Engine.lib" )

EngineApp engineApp;

int main( int argc, char *argv[] )
   {

#ifdef _DEBUG
   // Set up checks for memory leaks.
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
#endif

   EngLogger::Init( "logging.xml" );

   g_pApp->m_EngineOptions.Init( "EngineOptions.xml" );
   Point screenSize = g_pApp->m_EngineOptions.GetScreenSize();
   g_pApp->InitInstance( 0, screenSize.x, screenSize.y );

   g_pApp->MainLoop();

   EngLogger::Destroy();

   return 0;
   }