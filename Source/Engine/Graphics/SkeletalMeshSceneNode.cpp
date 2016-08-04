/*!
 * \file SkeletalMeshSceneNode.cpp
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

#include "EngineStd.h"
#include "SkeletalMeshSceneNode.h"
#include "..\ResourceCache\MeshResource.h"
#include "..\ResourceCache\TextureResource.h"
#include "OpenGLRenderer.h"
#include "..\Animation\AnimationClipNode.h"

#define VERTEX_LOCATION    0
#define UV_LOCATION        1
#define NORMAL_LOCATION    2
#define BONE_ID_LOCATION    3
#define BONE_WEIGHT_LOCATION    4

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\SKMeshFragmentShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\SKMeshFragmentShader.fragmentshader";

Vec3 aiVector3DToVec3( const aiVector3D& aiVector )
   {
   return Vec3( aiVector.x, aiVector.y, aiVector.z );
   }

Quaternion aiQuateronionToQuat( const aiQuaternion& aiQuat )
   {
   return Quaternion( aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z );
   }


SkeletalMeshSceneNode::SkeletalMeshSceneNode(
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResouce, MaterialPtr pMaterial, RenderPass renderPass, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderPass, pTransform, pMaterial ),
   m_pMeshResource( pMeshResouce ),
   // m_pMaterial(  ),
   m_VertexShader( VERTEX_SHADER_FILE_NAME ),
   m_FragmentShader( FRAGMENT_SHADER_FILE_NAME )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   m_BoneTransform = 0;
   m_MVPMatrix = 0;
   m_Texture = 0;
   m_TextureUni = 0;
   m_VertexArrayObj = 0;

   m_ToWorldMatrix = 0;
   m_LightPosWorldSpace = 0;
   m_LigthDirection = 0;
   m_LightColor = 0;
   m_LightPower = 0;
   m_LightAmbient = 0;
   m_LightNumber = 0;
   m_MaterialAmbient = 0;
   m_MaterialDiffuse = 0;

   m_VerticesIndexCount = 0;
   }

SkeletalMeshSceneNode::~SkeletalMeshSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int SkeletalMeshSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

   glGenVertexArrays( 1, &m_VertexArrayObj );
   glBindVertexArray( m_VertexArrayObj );

   m_VertexShader.OnRestore( pScene );
   m_FragmentShader.OnRestore( pScene );

   m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.GetVertexShader(), m_FragmentShader.GetFragmentShader() );

   m_VertexShader.ReleaseShader( m_Program );
   m_FragmentShader.ReleaseShader( m_Program );

   OpenGLRenderer::LoadTexture( &m_Texture, m_Props.GetMaterialPtr()->GetTextureResource() );

   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;
   SetRadius( pMeshExtra->m_Radius );

   m_CurrentAnimation = pMeshExtra->m_pScene->mAnimations[ 0 ]->mName.C_Str();
   /*LoadBones( pMeshExtra );*/

   m_BoneAnimationData.resize( pMeshExtra->m_NumBones );

   OpenGLRenderer::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], &m_Buffers[ Normal_Buffer ], pMeshResHandle );
   OpenGLRenderer::LoadBones( &m_Buffers[ Bone_Buffer ], pMeshResHandle );

   
   // 1st attribute buffer : vertices
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Vertex_Buffer ] );
   glEnableVertexAttribArray( VERTEX_LOCATION );
   glVertexAttribPointer(
      VERTEX_LOCATION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( const GLvoid* ) 0            // array buffer offset
      );

   // 2nd attribute buffer : UVs
   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ UV_Buffer ] );
   glEnableVertexAttribArray( UV_LOCATION );
   glVertexAttribPointer(
      UV_LOCATION,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( const GLvoid* ) 0                          // array buffer offset
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Normal_Buffer ] );
   glEnableVertexAttribArray( NORMAL_LOCATION );
   glVertexAttribPointer(
      NORMAL_LOCATION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( const GLvoid* ) 0            // array buffer offset
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Bone_Buffer ] );

   glEnableVertexAttribArray( BONE_ID_LOCATION );
   glVertexAttribIPointer( 
      BONE_ID_LOCATION, 
      MAXIMUM_BONES_PER_VEREX,
      GL_UNSIGNED_INT, 
      sizeof( OpenGLRenderer::VertexToBoneMapping ), 
      ( const GLvoid* ) 0 );

   glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
   glVertexAttribPointer( 
      BONE_WEIGHT_LOCATION, 
      MAXIMUM_BONES_PER_VEREX,
      GL_FLOAT, GL_FALSE, 
      sizeof( OpenGLRenderer::VertexToBoneMapping ), 
      ( const GLvoid* ) ( sizeof( unsigned int ) * MAXIMUM_BONES_PER_VEREX ) );

   m_MVPMatrix = glGetUniformLocation( m_Program, "MVP" );
   m_TextureUni = glGetUniformLocation( m_Program, "myTextureSampler" );

   m_ToWorldMatrix = glGetUniformLocation( m_Program, "M" );
   m_LightPosWorldSpace = glGetUniformLocation( m_Program, "LightPosition_WorldSpace" );
   m_LigthDirection = glGetUniformLocation( m_Program, "LighDirection" );
   m_LightColor = glGetUniformLocation( m_Program, "LightColor" );
   m_LightPower = glGetUniformLocation( m_Program, "LightPower" );
   m_LightAmbient = glGetUniformLocation( m_Program, "LightAmbient" );
   m_LightNumber = glGetUniformLocation( m_Program, "LightNumber" );

   m_EyeDirWorldSpace = glGetUniformLocation( m_Program, "EyeDirection_WorldSpace" );

   m_MaterialDiffuse = glGetUniformLocation( m_Program, "MaterialDiffuse" );
   m_MaterialAmbient = glGetUniformLocation( m_Program, "MaterialAmbient" );

   m_BoneTransform = glGetUniformLocation( m_Program, "BoneTransform" );

   // restore all of its children
   SceneNode::VOnRestore( pScene );

   return S_OK;
   }

int SkeletalMeshSceneNode::VRender( Scene *pScene )
   {
   // Use our shader
   glUseProgram( m_Program );
   glBindVertexArray( m_VertexArrayObj );

   // Get the projection & view matrix from the camera class
   Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetWorldViewProjection( pScene );
   // Send our transformation to the currently bound shader, 
   // in the "MVP" uniform
   // 1-> how many matrix, GL_FALSE->should transpose or not
   glUniformMatrix4fv( m_MVPMatrix, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );

   glUniformMatrix4fv( m_ToWorldMatrix, 1, GL_FALSE, &pScene->GetTopMatrix()[ 0 ][ 0 ] );

   auto pLightManager = pScene->GetLightManagerPtr();

   glUniform3fv( m_LightPosWorldSpace, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPosWorldSpace() );
   glUniform3fv( m_LigthDirection, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightDirection() );
   glUniform3fv( m_LightColor, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightColor() );
   glUniform1fv( m_LightPower, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPower() );
   glUniform3fv( m_LightAmbient, 1, ( const GLfloat* ) pLightManager->GetLightAmbient() );
   glUniform1i( m_LightNumber, pLightManager->GetActiveLightCount() );


   glUniform3fv( m_EyeDirWorldSpace, 1, ( const GLfloat* ) &pScene->GetCamera()->GetForward() );

   glUniform4fv( m_MaterialDiffuse, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetDiffuse() );
   glUniform3fv( m_MaterialAmbient, 1, ( const GLfloat* ) m_Props.GetMaterialPtr()->GetAmbient() );


   glUniformMatrix4fv( m_BoneTransform, m_BoneAnimationData.size(), GL_TRUE, &( m_BoneAnimationData[ 0 ].m_AnimationTransform[ 0 ][ 0 ] ) );

   // Bind our texture in Texture Unit 0
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_Texture );
   // Set our "myTextureSampler" sampler to user Texture Unit 0
   glUniform1i( m_TextureUni, 0 );

   // Draw the triangles !
   glDrawElements(
      GL_TRIANGLES,      // mode
      m_VerticesIndexCount,    // count
      GL_UNSIGNED_INT,   // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );

   return S_OK;
   }

int SkeletalMeshSceneNode::VOnUpdate( Scene * pScene, unsigned long elapsedMs )
   {
   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );
   auto pAiScene = pMeshExtra->m_pScene;
   auto pAnimation = FindAnimation( m_CurrentAnimation, pMeshExtra->m_pScene );
   static float aiAnimTicks = 0.f;
   if( pAnimation )
      {
      // The unit of the tick in this animation data
      float aiTicksPerSecond = ( float ) ( pAnimation->mTicksPerSecond != 0 ? pAnimation->mTicksPerSecond : 25.0f );
      // how many ticks have passed 
      float aiTimeInTicks = (float) elapsedMs * aiTicksPerSecond / 1000.f;
      // If the time is larger than the animation, mod it
      aiAnimTicks = fmod( aiAnimTicks + aiTimeInTicks, ( float ) pAnimation->mDuration );

      UpdateAnimationBones( pMeshExtra, aiAnimTicks, pAnimation, pAiScene->mRootNode, pMeshExtra->m_GlobalInverseTransform );
      }
   // for updates its children
   SceneNode::VOnUpdate( pScene, elapsedMs );
   return S_OK;
   }

//void SkeletalMeshSceneNode::LoadBones( shared_ptr<MeshResourceExtraData> pMeshExtra )
//   {
//   
//   for( auto boneDataIt : pMeshExtra->m_BoneMappingData )
//      {
//      m_BoneAnimationData[ boneDataIt.second.m_BoneId ] 
//      }
//   }

void SkeletalMeshSceneNode::ReleaseResource( void )
   {
   m_BoneAnimationData.clear();

   if( m_VertexArrayObj )
      {
      glDeleteVertexArrays( 1, &m_VertexArrayObj );
      m_VertexArrayObj = 0;
      }

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_Buffers ), &m_Buffers[ 0 ] );
   ENG_ZERO_MEM( m_Buffers );

   if( m_TextureUni )
      {
      glDeleteTextures( 1, &m_Texture );
      m_Texture = 0;
      }

   if( m_Program )
      {
      glDeleteProgram( m_Program );
      m_Program = 0;
      }
   }

void SkeletalMeshSceneNode::UpdateAnimationBones( shared_ptr<MeshResourceExtraData> pMeshExtra, float aiAnimTicks, aiAnimation* pAnimation, aiNode* pAiNode, const aiMatrix4x4& parentTransfrom )
   {
   std::string nodeName( pAiNode->mName.C_Str() );

   aiMatrix4x4 localBonePoseTransform;
   // find the corresponding aiNodeAnim of this node
   // each aiAnimation has multiple channels, which should map to a aiNode( bone )
   const aiNodeAnim* pNodeAnim = FindNodeAnim( nodeName, pAnimation );

   if( pNodeAnim )
      {
      localBonePoseTransform = aiMatrix4x4( CalcInterpolatedScaling( aiAnimTicks, pNodeAnim ), CalcInterpolatedRotation( aiAnimTicks, pNodeAnim ), CalcInterpolatedPosition( aiAnimTicks, pNodeAnim ) );
      }
   else
      {
      // cannot find animation data, use T pose transform instead
      localBonePoseTransform = pAiNode->mTransformation; 
      }

   aiMatrix4x4 globalBonePoseTransform = parentTransfrom * localBonePoseTransform;
   BoneMappingData& boneMappingData = pMeshExtra->m_BoneMappingData;
   if( boneMappingData.find( nodeName ) != boneMappingData.end() )
      {
      BoneData& boneData = boneMappingData[ nodeName ];
      m_BoneAnimationData[ boneData.m_BoneId ].m_AnimationTransform = globalBonePoseTransform * boneData.m_BoneOffset;
      }

   for( unsigned int i = 0; i < pAiNode->mNumChildren; ++i )
      {
      UpdateAnimationBones( pMeshExtra, aiAnimTicks, pAnimation, pAiNode->mChildren[ i ], globalBonePoseTransform );
      }
   }


unsigned int SkeletalMeshSceneNode::FindPosition( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   for( unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++ )
      {
      if( aiAnimTicks < ( float ) pNodeAnim->mPositionKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }


unsigned int SkeletalMeshSceneNode::FindRotation( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumRotationKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++ )
      {
      if( aiAnimTicks < ( float ) pNodeAnim->mRotationKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }


unsigned int SkeletalMeshSceneNode::FindScaling( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumScalingKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++ )
      {
      if( aiAnimTicks < ( float ) pNodeAnim->mScalingKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }

aiAnimation* SkeletalMeshSceneNode::FindAnimation( const std::string& animationName, const aiScene* pAiScene ) const
   {
   for( unsigned int animIdx = 0; animIdx < pAiScene->mNumAnimations; ++animIdx )
      {
      if( !std::strcmp( animationName.c_str(), pAiScene->mAnimations[ animIdx ]->mName.C_Str() ) )
         {
         return pAiScene->mAnimations[ animIdx ];
         }
      }
   return NULL;
   }

aiNodeAnim* SkeletalMeshSceneNode::FindNodeAnim( const std::string& boneName, const aiAnimation* pAnimation ) const
   {
   for( unsigned int nodeIdx = 0; nodeIdx < pAnimation->mNumChannels; ++ nodeIdx )
      {
      if( !std::strcmp( boneName.c_str(), pAnimation->mChannels[ nodeIdx ]->mNodeName.C_Str() ) )
         {
         return pAnimation->mChannels[ nodeIdx ];
         }
      }
   return NULL;
   }

aiVector3D SkeletalMeshSceneNode::CalcInterpolatedPosition( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   if( pNodeAnim->mNumPositionKeys == 1 )
      {
      return pNodeAnim->mPositionKeys[ 0 ].mValue;
      }
   unsigned int positionIdx = FindPosition( aiAnimTicks, pNodeAnim );
   unsigned int nxtPositionIdx = ( positionIdx + 1 );
   ENG_ASSERT( nxtPositionIdx < pNodeAnim->mNumPositionKeys );
   float deltaTime = ( float ) ( pNodeAnim->mPositionKeys[ nxtPositionIdx ].mTime - pNodeAnim->mPositionKeys[ positionIdx ].mTime );
   float factor = ( aiAnimTicks - ( float ) pNodeAnim->mPositionKeys[ positionIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mPositionKeys[ positionIdx ].mValue;
   const aiVector3D& end = pNodeAnim->mPositionKeys[ nxtPositionIdx ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }


aiQuaternion SkeletalMeshSceneNode::CalcInterpolatedRotation( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   // we need at least two values to interpolate...
   if( pNodeAnim->mNumRotationKeys == 1 )
      {
      return pNodeAnim->mRotationKeys[ 0 ].mValue;
      }
   unsigned int rotationIdx = FindRotation( aiAnimTicks, pNodeAnim );
   unsigned int nxtRotationIdx = ( rotationIdx + 1 );
   ENG_ASSERT( nxtRotationIdx < pNodeAnim->mNumRotationKeys );
   float deltaTime = ( float ) ( pNodeAnim->mRotationKeys[ nxtRotationIdx ].mTime - pNodeAnim->mRotationKeys[ rotationIdx ].mTime );
   float factor = ( aiAnimTicks - ( float ) pNodeAnim->mRotationKeys[ rotationIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[ rotationIdx ].mValue;
   const aiQuaternion& endRotationQ = pNodeAnim->mRotationKeys[ nxtRotationIdx ].mValue;
   aiQuaternion ret;
   aiQuaternion::Interpolate( ret, startRotationQ, endRotationQ, factor );
   ret.Normalize();
   return ret;
   }


aiVector3D SkeletalMeshSceneNode::CalcInterpolatedScaling( float aiAnimTicks, const aiNodeAnim* pNodeAnim ) const
   {
   if( pNodeAnim->mNumScalingKeys == 1 )
      {
      return pNodeAnim->mScalingKeys[ 0 ].mValue;
      }
   // find the last key frame that its time is smaller than specified animation time
   unsigned int scaleIdx = FindScaling( aiAnimTicks, pNodeAnim );
   unsigned int nxtScaleIdx = ( scaleIdx + 1 );
   ENG_ASSERT( nxtScaleIdx < pNodeAnim->mNumScalingKeys );
   // time between these two key frames
   float deltaTime = ( float ) ( pNodeAnim->mScalingKeys[ nxtScaleIdx ].mTime - pNodeAnim->mScalingKeys[ scaleIdx ].mTime );
   float factor = ( aiAnimTicks - ( float ) pNodeAnim->mScalingKeys[ scaleIdx ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mScalingKeys[ scaleIdx ].mValue;
   const aiVector3D& end = pNodeAnim->mScalingKeys[ nxtScaleIdx ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }