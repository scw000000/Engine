
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

#pragma comment( lib, "zlibstat.lib" )
#pragma comment( lib, "tinyxml.lib" )

#pragma comment( lib, "assimp-vc140-mt.lib" )

#if defined(_M_IX86)
#if defined(_DEBUG)
//#pragma comment(lib, "glew32sd.lib")
#pragma comment( lib, "lua51_debug.lib" )
#pragma comment( lib, "glew32d.lib" )
#pragma comment( lib, "CEGUIBase-0_d.lib" )
#pragma comment( lib, "CEGUIOpenGLRenderer-0_d.lib" )
#pragma comment( lib, "BulletCollision_Debug.lib" )
#pragma comment( lib, "BulletDynamics_Debug.lib" )
#pragma comment( lib, "LinearMath_Debug.lib" )
#else
#pragma comment( lib, "lua51.lib" )
#pragma comment( lib, "glew32.lib" )
#pragma comment( lib, "CEGUIBase-0.lib" )
#pragma comment( lib, "CEGUIOpenGLRenderer-0.lib" )
#pragma comment( lib, "BulletCollision.lib" )
#pragma comment( lib, "BulletDynamics.lib" )
#pragma comment( lib, "LinearMath.lib" )

#endif
#elif defined(_M_X64)
#if defined(_DEBUG)

#else

#endif
#else
#error Preprocessor defines can't figure out which Bullet library to use.
#endif