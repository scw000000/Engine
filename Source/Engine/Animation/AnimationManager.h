#pragma once
/*!
 * \file AnimationManager.h
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

class IAnimationManager
   {
   public:
      virtual ~IAnimationManager( void ) { };
      virtual void VSetRunning( bool isRunning ) = 0;
      virtual void VUpdate( unsigned long elapsedMs ) = 0;

   private:
      
   };

class AnimationState;

class AnimationManager : public IAnimationManager
   {
   public:
      
      virtual void VSetRunning( bool isRunning ) override;
      virtual void VUpdate( unsigned long elapsedMs ) override;

   private:
      bool isRunning;
      std::list< shared_ptr< AnimationState > > m_AnimationStates;
   };
