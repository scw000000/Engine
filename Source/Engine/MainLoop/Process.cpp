////////////////////////////////////////////////////////////////////////////////
// Filename: Process.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "EngineStd.h"
#include "Process.h"

Process::Process( void )
   {
   m_state = UNINITIALIZED;
   }

Process::~Process( void )
   {
   if( m_pChild )
      {
      m_pChild->VOnAbort();
      }
   }

// Release m_pChild pointer and return it
StrongProcessPtr Process::RemoveChild( void )
   {
   if (m_pChild)
	   {
      StrongProcessPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
      // shared_ptr::reset -> set this pointer to NULL
		m_pChild.reset();
		return pChild;
	   }
	return StrongProcessPtr();
   }
