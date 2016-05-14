/*!
 * \file EditorViewState.cpp
 * \date 2016/05/14 21:41
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
#include "EditorViewState.h"
#include "..\UserInterface\HumanView.h"

void EditorViewState::VInit( void ) 
   {
   if( !g_pApp->VLoadLevel( ) )
      {
      ENG_ERROR( "The game failed to load." );
      g_pApp->AbortGame( );
      }
   // shared_ptr<IGameView> menuView;
   shared_ptr<IGameView> menuView( ENG_NEW HumanView( g_pApp->m_pRenderer ) );
   g_pApp->m_pGame->VAddView( menuView );
   }

void EditorViewState::VOnUpdate( float time, float elapsedTime )
   {
   
   }