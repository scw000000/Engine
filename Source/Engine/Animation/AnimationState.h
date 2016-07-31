/*!
 * \file AnimationState.h
 * \date 2016/07/30 21:54
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
#include <assimp/scene.h>

class ResHandle;
class MeshResourceExtraData;
class IAnimationNode;

class AnimationState
   {
   public:
      AnimationState( shared_ptr< ResHandle > pMeshRes );

      float GetTimePosition( void ) const;
      void SetTimePosition( float newTimePos ); 
      void AddTimeOffset( float timeOffset );
      void Update( unsigned long elapsedMs );
      void UpdatetGlobalBoneTransform( aiNode* pAiNode, const aiMatrix4x4& parentTransfrom );
      void SetIsRunning( bool isRunning );
      bool GetIsRunning( void ) const;
      void SetShouldLoop( bool shouldLoop );
      bool GetShouldLoop( void ) const;
      

   private:
      shared_ptr< ResHandle > m_pMeshResource;
      shared_ptr< MeshResourceExtraData > m_pMeshExtraData;
      shared_ptr< IAnimationNode > m_pRootAnimNode;
      std::vector< aiMatrix4x4 > m_GlobalBoneTransform;
   };