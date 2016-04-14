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
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#pragma comment(lib, "zlibstat.lib")
#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "lua51_debug.lib")



#if defined(_M_IX86)
	#if defined(_DEBUG)
      //#pragma comment(lib, "glew32sd.lib")
      #pragma comment(lib, "glew32d.lib")
      #pragma comment(lib, "CEGUIBase-0_d.lib")
      #pragma comment(lib, "CEGUIOpenGLRenderer-0_d.lib")
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
   Vec3 v1( 1.0f, 0.0f, 0.0f );
   Vec3 v2( 1.0, 1.0, 1.0 );
  // std::cout << ToStr( v1 ) << std::endl;
   Mat4x4 m1( glm::mat4( 1.0f ) );
   Quaternion aaa;
   m1.BuildYawPitchRoll(  DEGREES_TO_RADIANS(10.0f), DEGREES_TO_RADIANS(-20.f), DEGREES_TO_RADIANS(90.f) );
   //std::cout << ToStr( m1 ) << std::endl;
   std::cout << ToStr( m1.Xform( v1 ) ) << std::endl;
  // m1.BuildYawPitchRoll(  DEGREES_TO_RADIANS(10.0f), DEGREES_TO_RADIANS(20.f), DEGREES_TO_RADIANS(30.f) );
   Vec3 v3 = ENG_DEGREES_OVER_RADIANS * ( m1.GetYawPitchRoll() );
   std::cout << ToStr( v3 ) << std::endl;
   /*
   std::cout << ToStr( m1 ) << std::endl;
   m1.BuildTranslation( v1 );
   std::cout << ToStr( m1 ) << std::endl;
   std::cout << ToStr( m1.Xform( v2 ) ) << std::endl;
   m1.SetPosition( m1.Xform( v2 ) );
   std::cout << ToStr( m1 ) << std::endl;
   Mat4x4 m2( m1 );
   std::cout << ToStr( m2 ) << std::endl;
   m1.BuildRotationX( ENG_PI );
   std::cout << ToStr( m1.Xform( Vec3( 0.0f, 1.0f, 0.0f ) ) ) << std::endl;*/
 //  std::cout << ToStr( v1.Cross( v2 ) ) << std::endl;
 //  std::cout << ToStr( v1.Dot( v2 ) ) << std::endl;
//std::cout << ToStr( v1.Length() ) << std::endl;
//std::cout << ToStr( *v1.Normalize() ) << std::endl;
   g_pApp->MainLoop();

   
  // g_pApp->Run();

   Logger::Destroy();
  // ENG_NEW int();

   return 0;
   }