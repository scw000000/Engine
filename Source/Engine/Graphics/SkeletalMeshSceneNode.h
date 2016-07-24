#pragma once
/*!
 * \file SkeletalMeshSceneNode.h
 * \date 2016/07/23 10:56
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
#include "Shaders.h"


class BoneData
   {
   public:
      BoneData( const Transform& transform ) { m_BoneOffset = transform;  }
      
   public:
      Transform m_FinalTransform;
      Transform m_BoneOffset;
   
   private:
   };

typedef std::unordered_map< std::string, BoneData > BoneMappingData;

class MeshResourceExtraData;

struct aiAnimation;
struct aiNodeAnim;

class SkeletalMeshSceneNode : public SceneNode
   {
   public:
      SkeletalMeshSceneNode( const ActorId actorId,
                     IRenderComponent* pRenderComponent,
                     shared_ptr<Resource> pMeshResouce,
                     MaterialPtr pMaterial,
                     RenderPass renderPass,
                     TransformPtr pTransform );
      ~SkeletalMeshSceneNode( void );
      virtual int VOnRestore( Scene *pScene ) override;
      virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }

      virtual int VRender( Scene *pScene )  override;
      virtual int VOnUpdate( Scene *pScene, unsigned long elapsedMs ) override;
      

      GLuint GetProgram( void ) { return m_Program; };

   protected:
      void ReleaseResource( void );
      void LoadBones( shared_ptr<MeshResourceExtraData> pMeshExtra );
      void UpdateAnimationBones( float aiAnimTicks, aiAnimation* pAnimation, aiNode* pAiNode, const Transform& parentTransfrom );
      aiAnimation* FindAnimation( const std::string& animationName, const aiScene* pAiScene ) const;
      aiNodeAnim* FindNodeAnim( const std::string& boneName, const aiAnimation* pAnimation ) const;
      unsigned int FindPosition( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindRotation( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindScaling( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedPosition( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;
      aiQuaternion CalcInterpolatedRotation( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedScaling( float AnimationTime, const aiNodeAnim* pNodeAnim ) const;

   protected:
      shared_ptr<Resource>	 m_pMeshResource;

      GLuint            m_Program;
      VertexShader		m_VertexShader;
      FragmentShader		m_FragmentShader;

      GLuint            m_VertexArrayObj;

      enum VB_TYPES
         {
         Vertex_Buffer,
         UV_Buffer,
         Normal_Buffer,
         Bone_Buffer,
         Index_Buffer,
         Num_Buffers
         };

      GLuint            m_Buffers[ Num_Buffers ];

      GLuint            m_MVPMatrix;
      GLuint            m_Texture;
      GLuint            m_TextureUni;

      GLuint            m_ToWorldMatrix;
      GLuint            m_LightPosWorldSpace;
      GLuint            m_LigthDirection;
      GLuint            m_LightColor;
      GLuint            m_LightPower;
      GLuint            m_LightAmbient;
      GLuint            m_LightNumber;

      GLuint            m_EyeDirWorldSpace;

      GLuint            m_MaterialAmbient;
      GLuint            m_MaterialDiffuse;

      unsigned long     m_VerticesIndexCount;

      BoneMappingData m_BoneMappingData;
      std::string      m_CurrentAnimation;
   };
