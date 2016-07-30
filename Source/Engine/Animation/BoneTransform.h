/*!
 * \file BoneTransform.h
 * \date 2016/07/30 22:17
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

struct BoneTransform
   {
   public:
      BoneTransform( void ){}
      BoneTransform( const aiVector3D& scale, const aiQuaternion& rotation, const aiVector3D& translation ) 
         {
         m_Scale = scale; 
         m_Rotation = rotation; 
         m_Translation = translation;
         }

      BoneTransform Interpolate( const BoneTransform& other, float factor );
      aiVector3D m_Scale;
      aiVector3D m_Translation;
      aiQuaternion m_Rotation;
   };