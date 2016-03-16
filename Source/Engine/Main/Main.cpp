////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //  
///////////////////////
#include "EngineStd.h"

#include "Engine.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
//
#if defined(_M_IX86)
	#if defined(_DEBUG)
      #pragma comment(lib, "glew32sd.lib")
	#else

	#endif
#elif defined(_M_X64)
	#if defined(_DEBUG)

	#else

	#endif
#else
	#error Preprocessor defines can't figure out which Bullet library to use.
#endif

#pragma comment(lib, "tinyxml.lib")


EngineApp engineApp;

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

   _CrtSetDbgFlag(tmpDbgFlag);

   Logger::Init("logging.xml");

   g_pApp->m_EngineOptions.Init("PlayerOptions.xml");

   g_pApp->InitInstance( 0, g_pApp->m_EngineOptions.m_ScreenSize.x, g_pApp->m_EngineOptions.m_ScreenSize.y );
   
   g_pApp->MainLoop();

  // g_pApp->Run();

   Logger::Destroy();
  // ENG_NEW int();

   return 0;
   }