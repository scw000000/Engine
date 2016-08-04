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

struct BoneTransform;
typedef unsigned int BoneId;

class IAnimationNode
   {
   public:
      virtual ~IAnimationNode( void ) {}
      virtual EventType GetAnimNodeType( void ) const = 0;
      virtual const char* GetName( void ) const = 0;
      virtual void VUpdate( unsigned long elapsedMs ) = 0;
      virtual void VSetTimePosition( float timePos ) = 0;
      virtual float VGetTimePosition( void ) const = 0;
      virtual void VAddTimeOffset( float offset ) = 0;
      virtual void VSetPlayBackRate( float newRate ) = 0;
      virtual float VGetPlayBackRate( void ) const = 0;
      virtual void VSetIsRunning( bool isRunning ) = 0;
      virtual bool VGetIsRunning( void ) const = 0;
      virtual void VSetShouldLoop( bool shouldLoop ) = 0;
      virtual bool VGetShouldLoop( void ) const = 0;
      virtual void VSetLoopCount( unsigned int count ) = 0;
      virtual bool VGetLocalBoneTransform( BoneTransform& boneTransform, BoneId boneId ) const = 0;
   };

class MeshResourceExtraData;
struct aiAnimation;

typedef unsigned long AnimNodeType;

template <typename T>class BaseAnimationNode : public IAnimationNode
   {
   public:
      BaseAnimationNode( void );
      virtual EventType GetAnimNodeType( void ) const override { return s_AnimNodeType; };
      virtual const char* GetName( void ) const override { return s_pName; }
      virtual void VUpdate( unsigned long elapsedMs ) override {};
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
      const static AnimNodeType  s_AnimNodeType;
      const static char*      s_pName;

      float m_PlaybackRate;
      float m_TimePosition;
      float m_DurationInMs;
      bool m_IsRunning;
      unsigned int m_LoopCount;
   };

template <typename T> BaseAnimationNode<T>::BaseAnimationNode( void )
   {
   m_PlaybackRate = 1.f;
   m_TimePosition = 0.f;
   m_DurationInMs = 1.f;
   m_IsRunning = false;
   m_LoopCount = 0;
   }

template <typename T> void BaseAnimationNode<T>::VSetTimePosition( float timePos )
   {
   m_TimePosition = std::max( 0.f, std::min( 1.0f, timePos ) );
   }

template <typename T> float BaseAnimationNode<T>::VGetTimePosition( void ) const
   {
   return m_TimePosition;
   }

template <typename T> void BaseAnimationNode<T>::VAddTimeOffset( float offset )
   {
   if( !m_LoopCount ) // infinite loop
      {
      VSetTimePosition( fmod( m_TimePosition + offset, m_DurationInMs ) );
      }
   else
      {
      VSetTimePosition( fmod( std::max( 0.f, std::min( m_DurationInMs * m_LoopCount, m_TimePosition + offset ) ), m_DurationInMs ) );
      }
   }

template <typename T> void BaseAnimationNode<T>::VSetPlayBackRate( float newRate )
   {
   m_PlaybackRate = newRate;
   }

template <typename T> float BaseAnimationNode<T>::VGetPlayBackRate( void ) const
   {
   return m_PlaybackRate;
   }

template <typename T> void BaseAnimationNode<T>::VSetIsRunning( bool isRunning )
   {
   m_IsRunning = isRunning;
   }

template <typename T> bool BaseAnimationNode<T>::VGetIsRunning( void ) const
   {
   return m_IsRunning;
   }

template <typename T> void BaseAnimationNode<T>::VSetShouldLoop( bool shouldLoop )
   {
   m_LoopCount = shouldLoop ? 0 : 1;
   }

template <typename T> bool BaseAnimationNode<T>::VGetShouldLoop( void ) const
   {
   return m_LoopCount == 0;
   }

template <typename T> void BaseAnimationNode<T>::VSetLoopCount( unsigned int count )
   {
   m_LoopCount = count;
   }


