#pragma once
/*!
 * \file EngineStd.h
 * \date 2016/05/09 22:12
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


#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <crtdbg.h>

// C RunTime Header Files
#include <stdlib.h>

#include <malloc.h>

#include <tchar.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

/*!
 * \class ENG_Noncopyable
 *
 * \brief Modified version of non copyable class
 *  reference https://msdn.microsoft.com/zh-tw/library/dn457344.aspx
 * \author SCW
 * \date 05 2016
 */
class ENG_Noncopyable
   {  
   public:
      ENG_Noncopyable( void ) { }; // Default constructor 

      ENG_Noncopyable( const ENG_Noncopyable& x ) = delete;  
      ENG_Noncopyable& operator=( const ENG_Noncopyable& x ) = delete;  
   };  

//
//This macro must placed before all of the self-defined headers
//
#if defined(_DEBUG)
#	define ENG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define ENG_NEW new
#endif

// Useful #defines

extern const float fOPAQUE;
extern const int iOPAQUE;
extern const float fTRANSPARENT;
extern const int iTRANSPARENT;

extern const int MEGABYTE;
extern const float SIXTY_HERTZ;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;



#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#ifdef UNICODE
#define _tcssprintf wsprintf
#define tcsplitpath _wsplitpath
#else
#define _tcssprintf sprintf
#define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
// Reference: https://gcc.gnu.org/onlinedocs/cpp/Stringification.html#Stringification
// Macro arguments are completely macro-expanded before they are substituted into a macro body, unless they are stringified or pasted with other tokens.
// This macro is for retrieving the value of x, if x is a macro
// Ex: #define foo 4
//    __STR2__( foo )
//       == > "foo"
//    __STR1__( foo )
//       == > __STR1__( 4 )
//       == > __STR2__( 4 )
//       == > "4"
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "  

#define ENG_ZERO_MEM( x ) memset(x, 0, sizeof( x ))
#define ENG_ARRAY_SIZE_IN_ELEMENTS( x ) (sizeof( x )/sizeof(x[0]))

///////////////////////
//3rd party INCLUDES //
///////////////////////
#include "SDL.h"
#include "SDL_syswm.h"
#include "GL\glew.h"
#include "GL\wglew.h"
#include "tinyxml.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "..\Debugging\Logger.h"  // this should be the first of the gcc includes since it defines ENG_ASSERT()
#include "..\Utilities\types.h"
#include "..\Utilities\templates.h"
#include "..\Math\Geometry.h"
#include "..\Math\Frustum.h"
#include "..\Utilities\Math.h"
#include "..\Utilities\String.h"
#include "..\Utilities\XMLHelper.h"
#include "..\Main\Interfaces.h"
#include "..\ResourceCache\ResourceCache.h"
#include "..\MainLoop\Timer.h"
#include "..\Event\EventFactory.h"
#include "..\Graphics\Scene.h"

extern Color g_White;
extern Color g_Black;
extern Color g_Cyan;
extern Color g_Red;
extern Color g_Green;
extern Color g_Blue;
extern Color g_Yellow;
extern Color g_Gray40;
extern Color g_Gray25;
extern Color g_Gray65;
extern Color g_Transparent;

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;



#include "..\Main\Engine.h"
