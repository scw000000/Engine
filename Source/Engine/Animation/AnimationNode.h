#pragma once
/*!
 * \file AnimationNode.h
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
class MeshResourceExtraData;
struct aiAnimation;

class BaseAnimationNode : public IAnimationNode
   {
   public:
      BaseAnimationNode( void );
      virtual void VUpdate( unsigned long elapsedMs ) override;
      virtual void VSetTimePosition( float timePos ) override;
      virtual float VGetTimePosition( void ) const override;

      /**
       * @brief take the scaled offset as input and set time position to 0 ~ 1
       *
       * @param float offset
       * @return void
       */
      virtual void VAddTimeOffset( float offset ) override;
      virtual void VSetPlayBackRate( float newRate ) override;
      virtual float VGetPlayBackRate( void ) const override;
      virtual void VSetIsRunning( bool isRunning ) override;
      virtual bool VGetIsRunning( void ) const override;
      virtual void VSetShouldLoop( bool shouldLoop ) override;
      virtual bool VGetShouldLoop( void ) const override;
      virtual void VSetLoopCount( unsigned int count ) override;

   protected:
      float m_PlaybackRate;
      float m_TimePosition;
      float m_DurationInMs;
      bool m_IsRunning;
      unsigned int m_LoopCount;
   };


