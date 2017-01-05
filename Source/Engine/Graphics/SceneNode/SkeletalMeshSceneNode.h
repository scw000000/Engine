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
#include "..\Shaders.h"


class MeshResourceExtraData;

struct aiAnimation;
struct aiNodeAnim;

class AnimationState;

class SkeletalMeshSceneNode : public SceneNode
   {
   public:
      SkeletalMeshSceneNode( const ActorId actorId,
                     IRenderComponent* pRenderComponent,
                     shared_ptr<Resource> pMeshResouce,
                     shared_ptr<Resource> pAnimScriptResource,
                     MaterialPtr pMaterial,
                     RenderGroup renderGroup,
                     TransformPtr pTransform );
      ~SkeletalMeshSceneNode( void );
      virtual int VOnRestore( Scene *pScene ) override;
      virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }

      virtual int VRender( Scene *pScene )  override;

      GLuint GetProgram( void ) { return m_Program; };

   protected:
      void ReleaseResource( void );
      /*void LoadBones( shared_ptr<MeshResourceExtraData> pMeshExtra );*/
      void UpdateAnimationBones( shared_ptr<MeshResourceExtraData> pMeshExtra, float aiAnimTicks, aiAnimation* pAnimation, aiNode* pAiNode, const aiMatrix4x4& parentTransfrom );
      aiAnimation* FindAnimation( const std::string& animationName, const aiScene* pAiScene ) const;
      aiNodeAnim* FindNodeAnim( const std::string& boneName, const aiAnimation* pAnimation ) const;
      unsigned int FindPosition( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindRotation( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;
      unsigned int FindScaling( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedPosition( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;
      aiQuaternion CalcInterpolatedRotation( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;
      aiVector3D CalcInterpolatedScaling( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const;

   protected:
      shared_ptr<Resource>	 m_pMeshResource;
      shared_ptr< Resource > m_pAnimScriptResource;

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

      GLuint            m_MVPUni;
      GLuint            m_MeshTextureObj;
      GLuint            m_NeshTextureUni;

      GLuint            m_MUni;
      GLuint            m_LightPosWorldSpaceUni;
      GLuint            m_LigthDirWorldSpaceUni;
      GLuint            m_LightColorUni;
      GLuint            m_LightPowerUni;
      GLuint            m_LightNumberUni;

      GLuint            m_EyePosWorldSpaceUni;

      GLuint            m_MaterialAmbientUni;
      GLuint            m_MaterialDiffuseUni;
      GLuint            m_MaterialSpecularUni;

      GLuint            m_BoneTransformUni;

      unsigned long     m_VerticesIndexCount;

      shared_ptr< AnimationState > m_pAnimationState;
      MaterialPtr       m_pMaterial;
   };