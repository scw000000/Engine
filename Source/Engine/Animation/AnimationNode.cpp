/*!
 * \file AnimationNode.cpp
 * \date 2016/07/30 21:42
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

#include "EngineStd.h"
#include "AnimationNode.h"
#include "BoneTransform.h"

BaseAnimationNode::BaseAnimationNode( void )
   {
   m_PlaybackRate = 1.f;
   m_TimePosition = 0.f;
   m_DurationInMs = 1.f;
   m_IsRunning = false;
   m_LoopCount = 0;
   }

void BaseAnimationNode::VSetTimePosition( float timePos ) 
   { 
   m_TimePosition = std::max( 0.f, std::min( 1.0f, timePos ) );
   }

float BaseAnimationNode::VGetTimePosition( void ) const
   {
   return m_TimePosition;
   }

void BaseAnimationNode::VAddTimeOffset( float offset )
   {
   if( !m_LoopCount ) // infinite loop
      {
      VSetTimePosition( fmod( m_TimePosition + offset, m_DurationInMs ) );
      }
   else
      {
      VSetTimePosition( fmod( std::max( 0.f, std::min( m_DurationInMs * m_LoopCount, m_TimePosition + offset ) ), m_DurationInMs ) );
      }
   }

void BaseAnimationNode::VSetPlayBackRate( float newRate )
   {
   m_PlaybackRate = newRate;
   }

float BaseAnimationNode::VGetPlayBackRate( void ) const 
   {
   return m_PlaybackRate;
   }

void BaseAnimationNode::VSetIsRunning( bool isRunning ) 
   {
   m_IsRunning = isRunning;
   }

bool BaseAnimationNode::VGetIsRunning( void ) const 
   {
   return m_IsRunning;
   }

void BaseAnimationNode::VSetShouldLoop( bool shouldLoop ) 
   {
   m_LoopCount = shouldLoop ? 0: 1;
   }

bool BaseAnimationNode::VGetShouldLoop( void ) const 
   {
   return m_LoopCount == 0;
   }

void BaseAnimationNode::VSetLoopCount( unsigned int count )
   {
   m_LoopCount = count;
   }
