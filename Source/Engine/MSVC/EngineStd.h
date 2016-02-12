#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: EngineStd.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ENGINESTD_H_
#define _ENGINESTD_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////

//Windows Header Files: 
#define NOMINMAX        //prevent conflict between windows.h & STL
#include <windows.h>
#include <windowsx.h>
#include <crtdbg.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>



using std::shared_ptr;
using std::weak_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

#endif