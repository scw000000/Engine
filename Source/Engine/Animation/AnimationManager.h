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
      virtual void VUpdate( float deltaSeconds ) = 0;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) = 0;
      virtual void VRemoveAnimationState( ActorId actorId ) = 0;
      virtual shared_ptr< AnimationState > VGetAnimationState( ActorId actorId ) const = 0;
           
   private:
      
   };

class AnimationManager : public IAnimationManager
   {
   public:
      static AnimationManager& GetSingleton( void );
      virtual void VSetIsRunning( bool isRunning ) override;
      virtual bool VGetIsRunning( void ) const override { return m_IsRunning; };
      virtual void VUpdate( float deltaSeconds ) override;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) override;
      virtual void VRemoveAnimationState( ActorId actorId ) override;
      virtual shared_ptr< AnimationState > VGetAnimationState( ActorId actorId ) const override;
      void NewAnimationRootNodedelegate( IEventPtr pEvent );

   private:
      AnimationManager( void );
      bool m_IsRunning;
      std::map< ActorId, shared_ptr< AnimationState > > m_AnimationStates;
   };
