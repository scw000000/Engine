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
#include "..\Renderer\RendererLoader.h"
#include "..\ResourceCache\ScriptResource.h"
#include "..\LuaScripting\LuaStateManager.h"
#include "..\Animation\AnimationState.h"
#include "..\Animation\AnimationManager.h"

#define VERTEX_LOCATION    0
#define UV_LOCATION        1
#define NORMAL_LOCATION    2
#define BONE_ID_LOCATION    3
#define BONE_WEIGHT_LOCATION    4

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\SKMeshShader.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\SKMeshShader.fs.glsl";

Vec3 aiVector3DToVec3( const aiVector3D& aiVector )
   {
   return Vec3( aiVector.x, aiVector.y, aiVector.z );
   }

Quaternion aiQuateronionToQuat( const aiQuaternion& aiQuat )
   {
   return Quaternion( aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z );
   }


SkeletalMeshSceneNode::SkeletalMeshSceneNode(
   const ActorId actorId, IRenderComponent* pRenderComponent, shared_ptr<Resource> pMeshResouce, shared_ptr<Resource> pAnimScriptResource, MaterialPtr pMaterial, RenderGroup renderPass, TransformPtr pTransform )
   : SceneNode( actorId, pRenderComponent, renderPass, pTransform ),
   m_pMeshResource( pMeshResouce ),
   m_pAnimScriptResource( pAnimScriptResource ),
   m_pMaterial( pMaterial ),
   // m_pMaterial(  ),
   m_VertexShader( Resource( VERTEX_SHADER_FILE_NAME ) ),
   m_FragmentShader( Resource( FRAGMENT_SHADER_FILE_NAME ) )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   m_BoneTransformUni = -1;
   m_MVPUni = -1;
   m_MeshTextureObj = 0;
   m_NeshTextureUni = -1;
   m_VertexArrayObj = -1;

   m_MUni = -1;
   m_LightPosWorldSpaceUni = -1;
   m_LigthDirWorldSpaceUni = -1;
   m_LightColorUni = -1;
   m_LightPowerUni = -1;
   m_LightNumberUni = -1;
   m_EyePosWorldSpaceUni = -1;
   m_MaterialAmbientUni = -1;
   m_MaterialDiffuseUni = -1;
   m_MaterialSpecularUni = -1;

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

   m_VertexShader.VOnRestore();
   m_FragmentShader.VOnRestore();

   m_Program = OpenGLRendererLoader::GenerateProgram( { m_VertexShader.GetShaderObject(), m_FragmentShader.GetShaderObject() } );
  // m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.VGetShaderObject(), m_FragmentShader.VGetShaderObject() );

   m_VertexShader.VReleaseShader( m_Program );
   m_FragmentShader.VReleaseShader( m_Program );

   OpenGLRendererLoader::LoadTexture2D( &m_MeshTextureObj, m_pMaterial->GetTextureResource() );

   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( *m_pMeshResource );
   shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;
  // SetRadius( pMeshExtra->m_Radius );

   OpenGLRendererLoader::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], &m_Buffers[ Normal_Buffer ], pMeshResHandle );
   OpenGLRendererLoader::LoadBones( &m_Buffers[ Bone_Buffer ], pMeshResHandle );

   shared_ptr<ResHandle> pScriptResHandle = g_pApp->m_pResCache->GetHandle( *m_pAnimScriptResource );
   if( pScriptResHandle )
      {
      auto luaAnimState = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( "scriptRet" );
      ENG_ASSERT( luaAnimState.IsTable() && IsBaseClassOf< AnimationState >( luaAnimState ) );
      m_pAnimationState.reset( GetObjUserDataPtr< AnimationState >( luaAnimState ) );
      m_pAnimationState->SetMeshResourcePtr( pMeshResHandle );
      }
   else
      {
      m_pAnimationState.reset( ENG_NEW AnimationState( pMeshResHandle, NULL ) );
      }
   ENG_ASSERT( m_pAnimationState->Init() );
   m_pAnimationState->SetOwner( m_pRenderComponent->VGetOwner().lock() );
   AnimationManager::GetSingleton().VAddAnimationState( m_pAnimationState );

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
      sizeof( VertexToBoneMapping ), 
      ( const GLvoid* ) 0 );

   glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
   glVertexAttribPointer( 
      BONE_WEIGHT_LOCATION, 
      MAXIMUM_BONES_PER_VEREX,
      GL_FLOAT, GL_FALSE, 
      sizeof( VertexToBoneMapping ), 
      ( const GLvoid* ) ( sizeof( unsigned int ) * MAXIMUM_BONES_PER_VEREX ) );

   m_MVPUni = glGetUniformLocation( m_Program, "uMVP" );
   m_NeshTextureUni = glGetUniformLocation( m_Program, "uMeshTexture" );

   m_MUni = glGetUniformLocation( m_Program, "uM" );
   m_LightPosWorldSpaceUni = glGetUniformLocation( m_Program, "uLightPosition_WorldSpace" );
   m_LigthDirWorldSpaceUni = glGetUniformLocation( m_Program, "uLighDirection_WorldSpace" );
   m_LightColorUni = glGetUniformLocation( m_Program, "uLightColor" );
   m_LightPowerUni = glGetUniformLocation( m_Program, "uLightPower" );
   m_LightNumberUni = glGetUniformLocation( m_Program, "uLightNumber" );

   m_EyePosWorldSpaceUni = glGetUniformLocation( m_Program, "uEyePosition_WorldSpace" );

   m_MaterialDiffuseUni = glGetUniformLocation( m_Program, "uMaterialDiffuse" );
   m_MaterialAmbientUni = glGetUniformLocation( m_Program, "uMaterialAmbient" );
   m_MaterialSpecularUni = glGetUniformLocation( m_Program, "uMaterialSpecular" );

   m_BoneTransformUni = glGetUniformLocation( m_Program, "uBoneTransform" );

   // restore all of its children
   SceneNode::VOnRestore( pScene );

   return S_OK;
   }

int SkeletalMeshSceneNode::VRender( Scene *pScene )
   {
   // Use our shader
   glUseProgram( m_Program );
   glBindVertexArray( m_VertexArrayObj );

   Mat4x4 globalToWorld = VGetGlobalTransformPtr()->GetToWorld();

   // Get the projection & view matrix from the camera class
   Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * globalToWorld;
   // Send our transformation to the currently bound shader, 
   // in the "MVP" uniform
   // 1-> how many matrix, GL_FALSE->should transpose or not
   glUniformMatrix4fv( m_MVPUni, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );

   glUniformMatrix4fv( m_MUni, 1, GL_FALSE, &( globalToWorld[ 0 ][ 0 ] ) );

   auto pLightManager = pScene->GetLightManagerPtr();

   glUniform3fv( m_LightPosWorldSpaceUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPosWorldSpace() );
   glUniform3fv( m_LigthDirWorldSpaceUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightDirection() );
   glUniform3fv( m_LightColorUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightColor() );
   glUniform1fv( m_LightPowerUni, MAXIMUM_LIGHTS_SUPPORTED, ( const GLfloat* ) pLightManager->GetLightPower() );
   glUniform1i( m_LightNumberUni, pLightManager->GetActiveLightCount() );

   glUniform3fv( m_EyePosWorldSpaceUni, 1, ( const GLfloat* ) &pScene->GetCamera()->GetToWorldPosition() );

   glUniform4fv( m_MaterialDiffuseUni, 1, ( const GLfloat* ) m_pMaterial->GetDiffuse() );
   glUniform3fv( m_MaterialAmbientUni, 1, ( const GLfloat* ) m_pMaterial->GetAmbient() );
   glUniform3fv( m_MaterialSpecularUni, 1, ( const GLfloat* ) m_pMaterial->GetSpecular() );

   const auto& globalBoneTransform = m_pAnimationState->m_GlobalBoneTransform;
   glUniformMatrix4fv( m_BoneTransformUni, globalBoneTransform.size(), GL_TRUE, &( globalBoneTransform[ 0 ][ 0 ][ 0 ] ) );
   
   // Bind our texture in Texture Unit 0
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_MeshTextureObj );
   // Set our "myTextureSampler" sampler to user Texture Unit 0
   glUniform1i( m_NeshTextureUni, 0 );

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
   if( m_VertexArrayObj )
      {
      glDeleteVertexArrays( 1, &m_VertexArrayObj );
      m_VertexArrayObj = 0;
      }

   glDeleteBuffers( ENG_ARRAY_SIZE_IN_ELEMENTS( m_Buffers ), &m_Buffers[ 0 ] );
   ENG_ZERO_MEM( m_Buffers );

   if( m_NeshTextureUni )
      {
      glDeleteTextures( 1, &m_MeshTextureObj );
      m_MeshTextureObj = 0;
      }

   if( m_Program )
      {
      glDeleteProgram( m_Program );
      m_Program = 0;
      }
   if( m_pAnimationState )
      {
      AnimationManager::GetSingleton().VRemoveAnimationState( m_pAnimationState->m_pOwner->GetId() );
      m_pAnimationState.reset();
      }
   }

void SkeletalMeshSceneNode::UpdateAnimationBones( shared_ptr<MeshResourceExtraData> pMeshExtra, float aiAnimTicks, aiAnimation* pAnimation, aiNode* pAiNode, const aiMatrix4x4& parentTransfrom )
   {
   //std::string nodeName( pAiNode->mName.C_Str() );

   //aiMatrix4x4 localBonePoseTransform;
   //// find the corresponding aiNodeAnim of this node
   //// each aiAnimation has multiple channels, which should map to a aiNode( bone )
   //const aiNodeAnim* pNodeAnim = FindNodeAnim( nodeName, pAnimation );

   //if( pNodeAnim )
   //   {
   //   localBonePoseTransform = aiMatrix4x4( CalcInterpolatedScaling( aiAnimTicks, pNodeAnim ), CalcInterpolatedRotation( aiAnimTicks, pNodeAnim ), CalcInterpolatedPosition( aiAnimTicks, pNodeAnim ) );
   //   }
   //else
   //   {
   //   // cannot find animation data, use T pose transform instead
   //   localBonePoseTransform = pAiNode->mTransformation; 
   //   }

   //aiMatrix4x4 globalBonePoseTransform = parentTransfrom * localBonePoseTransform;
   //BoneMappingData& boneMappingData = pMeshExtra->m_BoneMappingData;
   //if( boneMappingData.find( nodeName ) != boneMappingData.end() )
   //   {
   //   BoneData& boneData = boneMappingData[ nodeName ];
   //   m_BoneAnimationData[ boneData.m_BoneId ].m_AnimationTransform = globalBonePoseTransform * boneData.m_BoneOffset;
   //   }

   //for( unsigned int i = 0; i < pAiNode->mNumChildren; ++i )
   //   {
   //   UpdateAnimationBones( pMeshExtra, aiAnimTicks, pAnimation, pAiNode->mChildren[ i ], globalBonePoseTransform );
   //   }
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