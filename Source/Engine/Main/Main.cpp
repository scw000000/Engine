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
#include "EngineStd.h"

#include "Engine.h"

#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

#pragma comment( lib, "zlibstat.lib" )
#pragma comment( lib, "tinyxml.lib" )
#pragma comment( lib, "lua51_debug.lib" )

#pragma comment( lib, "assimp-vc120-mtd.lib" )

#if defined(_M_IX86)
	#if defined(_DEBUG)
      //#pragma comment(lib, "glew32sd.lib")
      #pragma comment( lib, "glew32d.lib" )
      #pragma comment( lib, "CEGUIBase-0_d.lib" )
      #pragma comment( lib, "CEGUIOpenGLRenderer-0_d.lib" )
      #pragma comment( lib, "BulletCollision_Debug.lib" )
      #pragma comment( lib, "BulletDynamics_Debug.lib" )
      #pragma comment( lib, "LinearMath_Debug.lib" )
	#else

	#endif
#elif defined(_M_X64)
	#if defined(_DEBUG)

	#else

	#endif
#else
	#error Preprocessor defines can't figure out which Bullet library to use.
#endif
  


//EngineApp engineApp;

int main( int argc, char *argv[] )
   {
   // Set up checks for memory leaks.
	// Game Coding Complete reference - Chapter 21, page 834
	//
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   
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

   g_pApp->m_EngineOptions.Init( "EngineOptions.xml" );
   Point screenSize = g_pApp->m_EngineOptions.GetScreenSize();
   g_pApp->InitInstance( 0, screenSize.x, screenSize.y );

   g_pApp->MainLoop();

   Logger::Destroy();

   return 0;
   }