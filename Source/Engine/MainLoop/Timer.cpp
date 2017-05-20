/*!
 * \file Timer.cpp
 * \date 2017/03/20 9:26
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "EngineStd.h"
#include "Timer.h"


Timer* GetGlobalTimer()
   {
   // Using an accessor function gives control of the construction order
   static Timer timer;
   return &timer;
   }

Timer::Timer()
   {
   m_bTimerStopped = true;
   // Get how many ticks euqal one second
   m_u64TicksPerSec = SDL_GetPerformanceFrequency();

   m_llStopTime = 0;
   m_llLastElapsedTime = 0;
   m_llBaseTime = 0;
   }

void Timer::Reset()
   {
   Uint64 qwTime = GetAdjustedCurrentTime();

   m_llBaseTime = qwTime;
   m_llLastElapsedTime = qwTime;
   m_llStopTime = 0;
   m_bTimerStopped = FALSE;
   }

void Timer::Start()
   {
   // Get the current time
   Uint64 qwTime = SDL_GetPerformanceCounter();

   // Move forward base time for ( stopped time length ) if timer is stopped
   // Which means the time interval [ stop time, current time ]
   // will not count in future total running time ( fAppTime )
   if( m_bTimerStopped )
      {
      m_llBaseTime += qwTime - m_llStopTime;
      }     
   m_llStopTime = 0;
   m_llLastElapsedTime = qwTime;
   m_bTimerStopped = FALSE;
   }

void Timer::Stop()
   {
   // record stop time
   if( !m_bTimerStopped )
      {
      Uint64 qwTime = SDL_GetPerformanceCounter();
      m_llStopTime = qwTime;
      m_llLastElapsedTime = qwTime;
      m_bTimerStopped = TRUE;
      }
   }

// advance the timer by 0.1 seconds
void Timer::Advance()
   {
   m_llStopTime += m_u64TicksPerSec / 10;
   }

double Timer::GetAbsoluteTime()
   {
   Uint64 qwTime = SDL_GetPerformanceCounter();

   double fTime = qwTime / ( double )m_u64TicksPerSec;

   return fTime;
   }

// Return how long the timer has been running ( not include stop time )
double Timer::GetTime()
   {
   Uint64 qwTime = GetAdjustedCurrentTime();
   
   // If timer is stopped, qwTime will stay in last stopped time
   double fAppTime = ( double )( qwTime - m_llBaseTime ) / ( double )m_u64TicksPerSec;

   return fAppTime;
   }

void Timer::GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime )
   {
   ENG_ASSERT( pfTime && pfAbsoluteTime && pfElapsedTime );

   Uint64 qwTime = GetAdjustedCurrentTime();

   float fElapsedTime = (float) ((double) ( qwTime - m_llLastElapsedTime ) / (double) m_u64TicksPerSec);
   m_llLastElapsedTime = qwTime;

    // Clamp the timer to non-negative values to ensure the timer is accurate.
    // fElapsedTime can be outside this range if processor goes into a 
    // power save mode or we somehow get shuffled to another processor.  
    // However, the main thread should call SetThreadAffinityMask to ensure that 
    // we don't get shuffled to another processor.  Other worker threads should NOT call 
    // SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
    // the main thread.
   if( fElapsedTime < 0.0f )
       fElapsedTime = 0.0f;

   *pfAbsoluteTime = qwTime / ( double )m_u64TicksPerSec;
   *pfTime = ( qwTime - m_llBaseTime ) / ( double )m_u64TicksPerSec;
   *pfElapsedTime = fElapsedTime;
   }

// This function returns how many seconds has elapsed
float Timer::GetElapsedTime()
   {
   Uint64 qwTime = GetAdjustedCurrentTime();

   // If timer is stopped, elapsed time will equal to time interval [ last elapsedTime, stopTime ]
   // else it will equal to [ last elapsedTime, current time ]
   // to make sure that the elapsed time only moves forward when timer is running
   double fElapsedTime = (float) ((double) ( qwTime - m_llLastElapsedTime ) / (double) m_u64TicksPerSec);
   m_llLastElapsedTime = qwTime;

    // See the explanation about clamping in Timer::GetTimeValues()
   if( fElapsedTime < 0.0f )
       fElapsedTime = 0.0f;

   return ( float )fElapsedTime;
   }


//--------------------------------------------------------------------------------------
// If stopped, returns time when stopped otherwise returns current time
// Which means force current time stay in stopped time until the timer call start() again
//--------------------------------------------------------------------------------------
Uint64 Timer::GetAdjustedCurrentTime()
   {
   Uint64 qwTime;
   if( m_llStopTime != 0 )
      {
      qwTime = m_llStopTime;
      }
   else
      {
      qwTime = SDL_GetPerformanceCounter();
      }
   return qwTime;
   }

bool Timer::IsStopped()
   {
    return m_bTimerStopped;
   }
