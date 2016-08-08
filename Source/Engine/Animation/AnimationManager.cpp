/*!
 * \file AnimationManager.cpp
 * \date 2016/07/30 21:31
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
#include "AnimationManager.h"
#include "AnimationState.h"
#include "..\Event\EventManager.h"
#include "..\Event\Events.h"

AnimationManager::AnimationManager( void )
   {
   m_IsRunning = false;

   }

AnimationManager& AnimationManager::GetSingleton( void )
   {
   static AnimationManager animationManager;
   return animationManager;
   }

void AnimationManager::VSetIsRunning( bool isRunning )
   {
   m_IsRunning = isRunning; 
   for( auto pAnimStateIt : m_AnimationStates )
      {
      pAnimStateIt.second->SetIsRunning( isRunning );
      }
   }

void AnimationManager::VUpdate( unsigned long elapsedMs ) 
   {
   if( m_IsRunning )
      {
      for( auto pAnimStateIt : m_AnimationStates )
         {
         pAnimStateIt.second->Update( elapsedMs );
         }
      }
   }

void AnimationManager::VAddAnimationState( shared_ptr< AnimationState > pNewState )
   {
   auto actorId = pNewState->m_pOwner->GetId();
   ENG_ASSERT( m_AnimationStates.find( actorId ) == m_AnimationStates.end() );
   m_AnimationStates[ actorId ] = pNewState;
   }

void AnimationManager::VRemoveAnimationState( ActorId actorId )
   {
   auto animStateIt = m_AnimationStates.find( actorId );
   if( animStateIt == m_AnimationStates.end() )
      {
      return;
      }
   m_AnimationStates.erase( animStateIt );
   }

shared_ptr< AnimationState > AnimationManager::VGetAnimationState( ActorId actorId ) const
   {
   auto animStateIt = m_AnimationStates.find( actorId );
   if( animStateIt == m_AnimationStates.end() )
      {
      return shared_ptr< AnimationState >();
      }
   return animStateIt->second;
   }

