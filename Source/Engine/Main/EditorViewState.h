#pragma once
/*!
 * \file EditorViewState.h
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
#include "EngineState.h"

class EditorViewState : public IEngineState
   {
   public:
      virtual void VInit( void ) override;
      virtual void VOnUpdate( float time, float elapsedTime ) override;
   };