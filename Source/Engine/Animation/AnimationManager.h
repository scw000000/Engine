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

class AnimationState;

class IAnimationManager : public ENG_Noncopyable
   {
   public:
      virtual ~IAnimationManager( void ) { };
      virtual void VSetIsRunning( bool isRunning ) = 0;
      virtual bool VGetIsRunning( void ) const = 0;
      virtual void VUpdate( unsigned long elapsedMs ) = 0;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) = 0;
      virtual void VRemoveAnimationState( AnimationStateId stateId ) = 0;
      virtual shared_ptr< AnimationState > VGetAnimationState( AnimationStateId stateId ) const = 0;
           
   private:
      
   };

class AnimationManager : public IAnimationManager
   {
   public:
      static AnimationManager& GetSingleton( void );
      virtual void VSetIsRunning( bool isRunning ) override { m_IsRunning = isRunning; }
      virtual bool VGetIsRunning( void ) const { return m_IsRunning; };
      virtual void VUpdate( unsigned long elapsedMs ) override;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) override;
      virtual void VRemoveAnimationState( AnimationStateId stateId ) override;
      virtual shared_ptr< AnimationState > VGetAnimationState( AnimationStateId stateId ) const override;
      void NewAnimationRootNodedelegate( IEventPtr pEvent );

   private:
      AnimationManager( void );
      bool m_IsRunning;
      AnimationStateId m_NextStateId;
      std::map< AnimationStateId, shared_ptr< AnimationState > > m_AnimationStates;
   };
