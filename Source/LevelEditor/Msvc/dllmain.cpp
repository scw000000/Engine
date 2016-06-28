// dllmain.cpp : Defines the entry point for the DLL application.
#include "EditorStd.h"

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

#if defined(_M_IX86)
   #if defined(_DEBUG)

   #else

   #endif
#elif defined(_M_X64)
   #if defined(_DEBUG)

   #else

   #endif
#else
   #error Preprocessor defines can't figure out which Bullet library to use.
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

