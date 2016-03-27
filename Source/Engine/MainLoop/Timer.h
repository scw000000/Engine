#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Timer.h
////////////////////////////////////////////////////////////////////////////////



class Timer
   {
   public:
      Timer();

      void            Reset(); // resets the timer
      void            Start(); // starts the timer
      void            Stop();  // stop (or pause) the timer
      void            Advance(); // advance the timer by 0.1 seconds
      double          GetAbsoluteTime(); // get the absolute system time
      double          GetTime(); // get the current time
      float           GetElapsedTime(); // get the time that elapsed between Get*ElapsedTime() calls
      void            GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime ); // get all time values at once
      bool            IsStopped(); // returns true if timer stopped

   protected:
      Uint64   GetAdjustedCurrentTime();

      bool m_bUsingQPF;
      bool m_bTimerStopped;
      Uint64 m_u64TicksPerSec;

      Uint64 m_llStopTime; // If timer is stopped, m_llStopTime = times when last valid stop() is called, else = 0
      Uint64 m_llLastElapsedTime;
      Uint64 m_llBaseTime; // for calculating total app time, which should equal to [ last reset time, current time ] - [ total stopped time ]
   };

Timer* GetGlobalTimer();