#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ProcessManager.h
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Process.h"

class ProcessManager
   {
   typedef std::list<StrongProcessPtr> ProcessList;
   public:
      ~ProcessManager( void );

      unsigned int UpdateProcesses( float deltaSeconds );
      WeakProcessPtr AttachProcess( StrongProcessPtr pProcess );
      void AbortAllProcesses( bool immediate );
      // acccessors
      unsigned int GetProcessCount( void ) const { return m_processList.size(); }

   private:  
      void ClearAllProcesses( void );

   private:
      ProcessList m_processList;
   };