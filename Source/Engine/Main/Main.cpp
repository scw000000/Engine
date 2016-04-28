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
	#else

	#endif
#elif defined(_M_X64)
	#if defined(_DEBUG)

	#else

	#endif
#else
	#error Preprocessor defines can't figure out which Bullet library to use.
#endif
  

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

   _CrtSetDbgFlag( tmpDbgFlag );

   Logger::Init( "logging.xml" );

   g_pApp->m_EngineOptions.Init( "PlayerOptions.xml" );

   g_pApp->InitInstance( 0, g_pApp->m_EngineOptions.m_ScreenSize.x, g_pApp->m_EngineOptions.m_ScreenSize.y );

   Transform transform( Vec3( 12.0f, 68.0f, -16.0f ), Vec3( 21.0f, 31.0f, 61.0f ), Quaternion( Vec3( 30.9f, 50.f ,68.f ) ) );
  /* auto tmt = Transform( transform.GetToWorld() );
    std::cout << "tm :  "<< std::endl << ToStr( transform.GetToWorld() ) << std::endl<< std::endl;
     std::cout << "tmtm :  "<< std::endl << ToStr( tmt.GetToWorld() ) << std::endl<< std::endl;*/

   Transform transform2( Vec3( 12.0f, 68.0f, -16.0f ), Vec3( 21.0f, 31.0f, 61.0f ), Quaternion( Vec3( 33.9f, 125.f ,-68.f ) ) );

   auto m1 = transform.GetToWorld();
   auto m2 = transform2.GetToWorld();
  
   auto m12t = Transform( m1 * m2 );

    std::cout << "tm12 :  "<< std::endl << ToStr( m1 * m2 ) << std::endl<< std::endl;
   auto t12 = ( transform * transform2 );
   auto t12m = t12.GetToWorld();
   std::cout << "t12m :  "<< std::endl << ToStr( t12m ) << std::endl<< std::endl;
   std::cout << "t12m inv:  "<< std::endl << ToStr( t12m.Inverse() ) << std::endl<< std::endl;
   auto t21 = ( transform2 * transform );
   auto t21m = t21.GetToWorld();

   std::cout << "t21m :  "<< std::endl << ToStr( t21m ) << std::endl<< std::endl;
   std::cout << "t21m inv:  "<< std::endl << ToStr( t21m.Inverse() ) << std::endl<< std::endl;
   g_pApp->MainLoop();


   Logger::Destroy();

   return 0;
   }