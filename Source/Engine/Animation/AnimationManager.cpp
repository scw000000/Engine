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
   for( auto actorAnimStateIt : m_AnimationStates )
      {
      for( auto animationState : actorAnimStateIt.second  )
         {
         animationState->SetIsRunning( isRunning );
         }
      }
   }

void AnimationManager::VUpdate( float deltaSeconds ) 
   {
   if( m_IsRunning )
      {
      for( auto actorAnimStateIt : m_AnimationStates )
         {
         for( auto animationState : actorAnimStateIt.second )
            {
            animationState->Update( deltaSeconds );
            }
         }
      }
   }

void AnimationManager::VAddAnimationState( shared_ptr< AnimationState > pNewState )
   {
   auto actorId = pNewState->m_pOwner->GetId();
   auto actorAnimStatesIt = m_AnimationStates.find( actorId );
   if( actorAnimStatesIt == m_AnimationStates.end() || actorAnimStatesIt->second.size() <= pNewState->m_MeshIdx )
      {
      m_AnimationStates[ actorId ].resize( pNewState->m_MeshIdx + 1 );
      }
   ENG_ASSERT( m_AnimationStates[ actorId ][ pNewState->m_MeshIdx ].get() == nullptr );
   m_AnimationStates[ actorId ][ pNewState->m_MeshIdx ] = pNewState;
   }

void AnimationManager::VAddAnimationState( LuaPlus::LuaObject luaObj )
   {
   ENG_ASSERT( luaObj.IsTable() && IsBaseClassOf< AnimationState >( luaObj ) );
   // Need to add luaObj to lua instance as well
   auto pAnimState = GetObjUserDataPtr< AnimationState >( luaObj );

   // m_LuaInstance
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

std::vector< shared_ptr< AnimationState > > AnimationManager::VGetAnimationStates( ActorId actorId ) const
   {
   auto animStateIt = m_AnimationStates.find( actorId );
   if( animStateIt == m_AnimationStates.end() )
      {
      return std::vector< shared_ptr< AnimationState > >();
      }
   return animStateIt->second;
   }

bool AnimationManager::VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData )
   {
   // should not require any initial data for now
   
   return true;
   }

void AnimationManager::VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) 
   {
   
   }

