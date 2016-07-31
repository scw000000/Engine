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

class AnimationClipNode : public IAnimationNode
   {
   public:
      AnimationClipNode( shared_ptr< MeshResourceExtraData > pMeshExtra, const std::string& clipName );
      virtual void VUpdate( unsigned long elapsedMs ) override;
      virtual void VSetTimePosition( float timePos ) override;
      virtual float VGetTimePosition( void ) const override;
      virtual void VAddTimeOffset( float offset ) override;
      virtual void VSetPlayBackRate( float newRate ) override;
      virtual float VGetPlayBackRate( void ) const override;
      virtual void VSetIsRunning( bool isRunning ) override;
      virtual bool VGetIsRunning( void ) const override;
      virtual void VSetShouldLoop( bool shouldLoop ) override;
      virtual bool VGetShouldLoop( void ) const override;
      virtual BoneTransform VGetLocalBoneTransform( const std::string& boneName ) const override;

   private:
      shared_ptr< MeshResourceExtraData > m_pMeshExtraData;
      float m_AiTicksPerMs;
      float m_PlaybackRate;
      float m_TimePosition;
      bool m_IsRunning;
      bool m_ShouldLoop;
      
      std::string m_ClipName;
      aiAnimation* m_pAnimation;
   };

