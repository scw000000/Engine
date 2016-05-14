#pragma once
/*!
 * \file EngineState.h
 * \date 2016/05/14 21:33
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

class IEngineState
   {
   public:
      virtual void VInit( void ) = 0;
      virtual void VOnUpdate( float time, float elapsedTime ) = 0;
      
   };