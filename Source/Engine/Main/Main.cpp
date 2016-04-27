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

   Vec3 v1( 1.0f, 1.0f, -1.0f );
   
   Quaternion quat;
   quat.BuildPitchYawRollReg( -45.f, 45.f, 0.0f );


   /*Transform transform( &Vec3( 1.0f, 2.0f, 3.0f ), &quat );
   std::cout <<  std::endl << ToStr( transform.GetToWorld().GetForward() ) << std::endl<<  std::endl;
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;
   quat = transform.GetQuaternion();
   Quaternion quat2;
   Vec3 up =  transform.GetToWorld().GetUp();
   quat2.BuildAxisRad( g_Up, DEGREES_TO_RADIANS( 47 ) );
   transform.AddRotation( quat2 );
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;


   transform.AddPitchYawRollDeg( 0.f, 47.0f, 0.0f );
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;
   transform.AddPitchYawRollDeg( 0.f, -47.0f, 0.0f );
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;*/

  /* transform.AddPitchYawRollDeg( Vec3( 0.f, 47.0f, 0.0f ) );
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;
   transform.AddPitchYawRollDeg(  Vec3(  0.f, -47.0f, 0.0f ) );
   std::cout <<  std::endl << ToStr( transform.GetPitchYawRollDeg() ) << std::endl<<  std::endl;*/
   //Mat4x4 camMat = glm::lookAt(
			//	glm::vec3(1, 0, -1), // Camera is at (4,3,-3), in World Space
			//	glm::vec3(2, 0, 0), // and looks at the origin
			//	glm::vec3(0,1,0));  // Head is up (set to 0,-1,0 to look upside-down)
   //Mat4x4 inv = camMat.Inverse();
   // std::cout << "camMat: matrix"<<  std::endl;
   //std::cout <<  std::endl << ToStr( camMat ) << std::endl<<  std::endl;
   //std::cout << "camMat: xform"<<  std::endl;
   //std::cout << ToStr( camMat.Xform( v1 ) ) <<  std::endl;
   //std::cout << "camMat: position"<<  std::endl;
   //std::cout << ToStr( camMat.GetPosition() ) <<  std::endl;
   //std::cout << "camMat: direction"<<  std::endl;
   //std::cout << ToStr( camMat.GetForward() ) <<  std::endl<<  std::endl;

   //std::cout << ToStr( inv ) << std::endl<<  std::endl;

   //std::cout << ToStr( inv.Xform( v1 ) ) <<  std::endl;
   //std::cout << ToStr( inv.GetPosition() ) <<  std::endl;
   //std::cout << ToStr( inv.GetForward() ) <<  std::endl<<  std::endl;

   //std::cout << ToStr( inv*camMat ) <<  std::endl;
   g_pApp->MainLoop();


   Logger::Destroy();

   return 0;
   }