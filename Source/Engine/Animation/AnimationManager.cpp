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
   IEventManager* pEventMgr = IEventManager::GetSingleton();

   }

AnimationManager& AnimationManager::GetSingleton( void )
   {
   static AnimationManager animationManager;
   return animationManager;
   }

void AnimationManager::VUpdate( unsigned long elapsedMs ) 
   {
   for( auto pAnimStateIt : m_AnimationStates )
      {
      pAnimStateIt.second->Update( elapsedMs );
      }
   }

void AnimationManager::VAddAnimationState( shared_ptr< AnimationState > pNewState )
   {
   auto stateId = pNewState->GetId();
   ENG_ASSERT( m_AnimationStates.find( stateId ) == m_AnimationStates.end() );
   m_AnimationStates[ stateId ] = pNewState;
   }

void AnimationManager::VRemoveAnimationState( AnimationStateId stateId )
   {
   auto animStateIt = m_AnimationStates.find( stateId );
   ENG_ASSERT( animStateIt != m_AnimationStates.end() );
   m_AnimationStates.erase( animStateIt );
   }

shared_ptr< AnimationState > AnimationManager::VGetAnimationState( AnimationStateId stateId ) const
   {
   auto animStateIt = m_AnimationStates.find( stateId );
   if( animStateIt == m_AnimationStates.end() )
      {
      return shared_ptr< AnimationState >();
      }
   return animStateIt->second;
   }


//void AnimationManager::NewAnimationRootNodedelegate( IEventPtr pEvent )
//   {
//   shared_ptr< Event_New_Anim_Root_Node > pDerivedEvt = static_pointer_cast< Event_New_Anim_Root_Node >( pEvent );
//   auto pRootAnimNode = pDerivedEvt->GetAnimRootNodePtr().lock();
//   auto pMeshResource = pDerivedEvt->GetMeshResoucePtr().lock();
//   if( !pRootAnimNode || !pMeshResource )
//      {
//      return;
//      }
//   if( ++m_NextStateId == INVALID_ANIMSTATE_ID )
//      {
//      --m_NextStateId;
//      return;
//      }
//   shared_ptr< AnimationState > pNewStat( ENG_NEW AnimationState( m_NextStateId, pMeshResource, pRootAnimNode ) );
//   VAddAnimationState( pNewStat );
//   }

