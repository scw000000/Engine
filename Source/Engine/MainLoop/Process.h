#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Process.h
////////////////////////////////////////////////////////////////////////////////

class Process;
typedef shared_ptr<Process> StrongProcessPtr;
typedef weak_ptr<Process> WeakProcessPtr;

// This class is actually a linked list
class Process
   {
   friend class ProcessManager;

   public:
      enum State
         { 
         // Processes that are neither dead nor alive
		   UNINITIALIZED = 0,  // created but not running
		   REMOVED,  // removed from the process list but not destroyed; this can happen when a process that is already running is parented to another process

		   // Living processes
		   RUNNING,  // initialized and running
		   PAUSED,  // initialized but paused
		
		   // Dead processes
		   SUCCEEDED,  // completed successfully
		   FAILED,  // failed to complete
		   ABORTED,  // aborted; may not have started
         };

      Process( void );
      virtual ~Process( void );

      // Functions for ending the process
      inline void Succeed( void );
      inline void Fail( void );

      inline void Pause( void );
      inline void UnPause( void );

      // accessors
      State GetState( void ) const { return m_state; }
      bool IsAlive( void ) const { return  ( m_state == RUNNING || m_state == PAUSED ); }
      bool IsDead( void ) const { return ( m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED ); }
      bool IsRemoved( void ) const { return ( m_state == REMOVED ); }
      bool IsPaused( void ) const { return m_state == PAUSED; }

      inline void AttachChild( StrongProcessPtr pChild );
      StrongProcessPtr RemoveChild( void );
      StrongProcessPtr PeekCHild( void ) { return m_pChild; }

   protected:
      virtual void VOnInit( void ) { m_state = RUNNING; }
      virtual void VOnUpdate( unsigned long deltaMs ) = 0;
      virtual void VOnSuccess( void ) { }
      virtual void VOnFail( void ) { }
      virtual void VOnAbort( void ) { }

   private:
      // This function can called by ProcessManager
      void SetState( State newState ) { m_state = newState; }

   private:
      State m_state;
      StrongProcessPtr m_pChild;
   };

inline void Process::Succeed( void )
   {
   ENG_ASSERT( m_state == RUNNING || m_state == PAUSED );
   m_state = SUCCEEDED;
   }

inline void Process::Fail( void )
   {
   ENG_ASSERT( m_state == RUNNING || m_state == PAUSED );
   m_state = FAILED;
   }

inline void Process::Pause( void)
   {
   if (m_state == RUNNING)
      {
      m_state = PAUSED;
      }
	else
      {
	   ENG_WARNING("Attempting to pause a process that isn't running");
      }
   }

inline void Process::UnPause(void)
   {
	if ( m_state == PAUSED )
      {
		m_state = RUNNING;
      }
	else
      {
		ENG_WARNING("Attempting to unpause a process that isn't paused");
      }
   }

// Attach child recursively
inline void Process::AttachChild( StrongProcessPtr pChild )
   {
   if( m_pChild )
      {
      m_pChild->AttachChild( pChild );
      }
   else
      {
      m_pChild = pChild;
      }
   }