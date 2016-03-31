////////////////////////////////////////////////////////////////////////////////
// Filename: ProcessManager.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "EngineStd.h"
#include "ProcessManager.h"


ProcessManager::~ProcessManager(void)
   {
   ClearAllProcesses();
   }

unsigned int ProcessManager::UpdateProcesses( unsigned long deltaMs )
   {
   unsigned short int successCount = 0;
   unsigned short int failCount = 0;
   
   ProcessList::iterator it = m_processList.begin();
   while( it != m_processList.end() )
      {
      // For easier accessing
      StrongProcessPtr pCurrProcess = (*it);

      ProcessList::iterator currIt = it;
      ++it;

      // Ordering the state checking in life cycle order so it can be executed in only one update process
      if( pCurrProcess->GetState() == Process::UNINITIALIZED )
         {
         pCurrProcess->VOnInit();
         }

      if( pCurrProcess->GetState() == Process::RUNNING )
         {
         pCurrProcess->VOnUpdate( deltaMs );
         }
      // This process is SUCCEED or FAILED or ABORTED
      // After necessary processing, remove it from the list
      if( pCurrProcess->IsDead() )
         {
         switch ( pCurrProcess->GetState() )
            {
            
            case Process::SUCCEEDED :
               {
               pCurrProcess->VOnSuccess();
               StrongProcessPtr pChild = pCurrProcess->RemoveChild();
               if( pChild )
                  {
                  // Attach the rest task to the list 
                  AttachProcess( pChild );
                  }
               break;
               }
            case Process::FAILED :
               {
               pCurrProcess->VOnFail();
               ++failCount;
               break;
               }
            case Process::ABORTED :
               {
               pCurrProcess->VOnAbort();
               ++failCount;
               break;
               }
            }
         m_processList.erase( currIt );
         }
      }
   // compact the int with the two counts: successCount & failCount
   return ( successCount << 16 | failCount );
   }


WeakProcessPtr ProcessManager::AttachProcess( StrongProcessPtr pProcess )
   {
	m_processList.push_front(pProcess);
   return WeakProcessPtr(pProcess);
   }

void ProcessManager::ClearAllProcesses( void )
   {
   m_processList.clear();
   }

