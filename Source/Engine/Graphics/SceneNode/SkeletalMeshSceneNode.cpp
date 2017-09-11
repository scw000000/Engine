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
#include "..\VideoResourceCache\VideoTextureResource.h"
#include "..\VideoResourceCache\VideoMeshResource.h"
#include "..\Renderer\RenderManager.h"
#include "..\Renderer\TBDRMainRenderer.h"

#include "..\Renderer\RendererLoader.h"
#include "..\ResourceCache\ScriptResource.h"
#include "..\LuaScripting\LuaStateManager.h"
#include "..\Animation\AnimationState.h"
#include "..\Animation\AnimationManager.h"

#define VERTEX_LOCATION    0
#define NORMAL_LOCATION    1
#define UV_LOCATION        2

#define TANGENT_LOCATION    3
#define BITANGENT_LOCATION    4
#define BONE_ID_LOCATION    5
#define BONE_WEIGHT_LOCATION    6

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
   m_VertexShader( shared_ptr< Resource >( ENG_NEW Resource( VERTEX_SHADER_FILE_NAME ) ) ),
   m_FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FRAGMENT_SHADER_FILE_NAME ) ) )
   {
   m_Program = 0;

   ENG_ZERO_MEM( m_Buffers );

   /*m_BoneTransformUni = -1;
   m_MVPUni = -1;
   m_MeshTextureObj = 0;
   m_MeshTextureUni = -1;
   m_VAO = -1;

   m_MUni = -1;
   m_LightPosWorldSpaceUni = -1;
   m_LigthDirWorldSpaceUni = -1;
   m_LightColorUni = -1;
   m_LightPowerUni = -1;
   m_LightNumberUni = -1;
   m_EyePosWorldSpaceUni = -1;
   m_MaterialAmbientUni = -1;
   m_MaterialDiffuseUni = -1;
   m_MaterialSpecularUni = -1;*/
   m_pDeferredMainRenderer = dynamic_cast< TBDRMainRenderer* >( &g_pApp->m_pRenderManager->VGetMainRenderer() );
   ENG_ASSERT( m_pDeferredMainRenderer );
   m_VerticesIndexCount = 0;
   m_UseNormalMap = false;
   }

SkeletalMeshSceneNode::~SkeletalMeshSceneNode( void )
   {
   ReleaseResource();
   }

// now load the reouce into VRam
int SkeletalMeshSceneNode::VOnRestore( Scene *pScene )
   {
   ReleaseResource();

   // glGenVertexArrays( 1, &m_VertexArrayObj );
   // glBindVertexArray( m_VertexArrayObj );

   // m_VertexShader.VOnRestore();
   // m_FragmentShader.VOnRestore();

   // m_Program = OpenGLRendererLoader::GenerateProgram( { m_VertexShader.GetShaderObject(), m_FragmentShader.GetShaderObject() } );
   //// m_Program = OpenGLRenderer::GenerateProgram( m_VertexShader.VGetShaderObject(), m_FragmentShader.VGetShaderObject() );

   // m_VertexShader.VReleaseShader( m_Program );
   // m_FragmentShader.VReleaseShader( m_Program );

   // shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( m_pMeshResource );
   // shared_ptr<MeshResourceExtraData> pMeshExtra = static_pointer_cast< MeshResourceExtraData >( pMeshResHandle->GetExtraData() );

   // m_VerticesIndexCount = pMeshExtra->m_NumVertexIndex;
   //// SetRadius( pMeshExtra->m_Radius );

   // OpenGLRendererLoader::LoadMesh( &m_Buffers[ Vertex_Buffer ], &m_Buffers[ UV_Buffer ], &m_Buffers[ Index_Buffer ], &m_Buffers[ Normal_Buffer ], pMeshResHandle );
   // OpenGLRendererLoader::LoadBones( &m_Buffers[ Bone_Buffer ], pMeshResHandle );

   // glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Vertex_Buffer ] );
   // glEnableVertexAttribArray( VERTEX_LOCATION );
   // glVertexAttribPointer(
   //    VERTEX_LOCATION,                  // attribute
   //    3,                  // size
   //    GL_FLOAT,           // type
   //    GL_FALSE,           // normalized?
   //    0,                  // stride
   //    ( const GLvoid* ) 0            // array buffer offset
   //    );

   // glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ UV_Buffer ] );
   // glEnableVertexAttribArray( UV_LOCATION );
   // glVertexAttribPointer(
   //    UV_LOCATION,                                // attribute
   //    2,                                // size
   //    GL_FLOAT,                         // type
   //    GL_FALSE,                         // normalized?
   //    0,                                // stride
   //    ( const GLvoid* ) 0                          // array buffer offset
   //    );

   // glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Normal_Buffer ] );
   // glEnableVertexAttribArray( NORMAL_LOCATION );
   // glVertexAttribPointer(
   //    NORMAL_LOCATION,                  // attribute
   //    3,                  // size
   //    GL_FLOAT,           // type
   //    GL_FALSE,           // normalized?
   //    0,                  // stride
   //    ( const GLvoid* ) 0            // array buffer offset
   //    );

   // glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Bone_Buffer ] );

   // glEnableVertexAttribArray( BONE_ID_LOCATION );
   // glVertexAttribIPointer( 
   //    BONE_ID_LOCATION, 
   //    MAXIMUM_BONES_PER_VEREX,
   //    GL_UNSIGNED_INT, 
   //    sizeof( BoneDataOfVertex ), 
   //    ( const GLvoid* ) 0 );

   // glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
   // glVertexAttribPointer( 
   //    BONE_WEIGHT_LOCATION, 
   //    MAXIMUM_BONES_PER_VEREX,
   //    GL_FLOAT,
   //    GL_FALSE, 
   //    sizeof( BoneDataOfVertex ), 
   //    ( const GLvoid* ) ( sizeof( BoneId ) * MAXIMUM_BONES_PER_VEREX ) // Starting from the beginning of VertexToBoneMappingData
   //    );

   // m_MVPUni = glGetUniformLocation( m_Program, "uMVP" );
   // m_MeshTextureUni = glGetUniformLocation( m_Program, "uMeshTexture" );

   // m_MUni = glGetUniformLocation( m_Program, "uM" );
   // m_LightPosWorldSpaceUni = glGetUniformLocation( m_Program, "uLightPosition_WorldSpace" );
   // m_LigthDirWorldSpaceUni = glGetUniformLocation( m_Program, "uLighDirection_WorldSpace" );
   // m_LightColorUni = glGetUniformLocation( m_Program, "uLightColor" );
   // m_LightPowerUni = glGetUniformLocation( m_Program, "uLightPower" );
   // m_LightNumberUni = glGetUniformLocation( m_Program, "uLightNumber" );

   // m_EyePosWorldSpaceUni = glGetUniformLocation( m_Program, "uEyePosition_WorldSpace" );

   // m_MaterialDiffuseUni = glGetUniformLocation( m_Program, "uMaterialDiffuse" );
   // m_MaterialAmbientUni = glGetUniformLocation( m_Program, "uMaterialAmbient" );
   // m_MaterialSpecularUni = glGetUniformLocation( m_Program, "uMaterialSpecular" );

   // m_BoneTransformUni = glGetUniformLocation( m_Program, "uBoneTransform" );


   //////////////////////////////////////////////////
   // Load textures for mesh
   m_MeshTextureObj = VideoTextureResourceLoader::LoadAndReturnTextureObject( m_pMaterial->m_pDiffuseTextureRes );

   if( m_pMaterial->m_pNormalTextureRes && m_pMaterial->m_pNormalTextureRes->m_Name.size() )
      {
      m_NormalMapTextureObj = VideoTextureResourceLoader::LoadAndReturnTextureObject( m_pMaterial->m_pNormalTextureRes );
      m_UseNormalMap = true;
      }
   else
      {
      m_UseNormalMap = false;
      }

   unsigned int meshIdx = m_pMaterial->GetMeshIndex();
   auto pMeshResData = VideoMeshResourceLoader::LoadAndReturnMeshResourceExtraData( m_pMeshResource );
   SetRadius( pMeshResData->m_Radius[ meshIdx ] );

   m_VerticesIndexCount = pMeshResData->m_MeshCount[ meshIdx ][ VideoMeshResourceExtraData::MeshCount_Index ];
   m_Buffers[ Vertex_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Vertex ];
   m_Buffers[ UV_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_UV ];
   m_Buffers[ Index_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Index ];
   m_Buffers[ Normal_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Normal ];
   m_Buffers[ Tangent_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Tangent ];
   m_Buffers[ Bitangent_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_Bitangent ];
   m_Buffers[ Bone_Buffer ] = pMeshResData->m_BufferObjects[ meshIdx ][ VideoMeshResourceExtraData::MeshBufferData_BoneData ];

   shared_ptr<ResHandle> pMeshResHandle = g_pApp->m_pResCache->GetHandle( m_pMeshResource );
   shared_ptr<ResHandle> pScriptResHandle = g_pApp->m_pResCache->GetHandle( m_pAnimScriptResource );
   if( pScriptResHandle )
      {
      auto luaAnimState = LuaStateManager::GetSingleton().GetGlobalVars().Lookup( "scriptRet" );
      ENG_ASSERT( luaAnimState.IsTable() && IsBaseClassOf< AnimationState >( luaAnimState ) );
      auto pAnimState = GetObjUserDataPtr< AnimationState >( luaAnimState );
      m_pAnimationState.reset( GetObjUserDataPtr< AnimationState >( luaAnimState ) );
      m_pAnimationState->SetMeshIndex( meshIdx );
      m_pAnimationState->SetMeshResourcePtr( pMeshResHandle );
      }
   else
      {
      m_pAnimationState.reset( ENG_NEW AnimationState( pMeshResHandle, NULL ) );
      }
   bool animStateInitSuccess = m_pAnimationState->Init();
   ENG_ASSERT( animStateInitSuccess );
   m_pAnimationState->SetOwner( m_pRenderComponent->VGetOwner().lock() );
   AnimationManager::GetSingleton().VAddAnimationState( m_pAnimationState );
   // LuaPlus::LuaFunction< LuaPlus::LuaObject > zzz;
   // Setup VAO
   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

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

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ UV_Buffer ] );
   glEnableVertexAttribArray( UV_LOCATION );
   glVertexAttribPointer(
      UV_LOCATION,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      ( void* ) 0
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Tangent_Buffer ] );
   glEnableVertexAttribArray( TANGENT_LOCATION );
   glVertexAttribPointer(
      TANGENT_LOCATION,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      ( void* ) 0
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Bitangent_Buffer ] );
   glEnableVertexAttribArray( BITANGENT_LOCATION );
   glVertexAttribPointer(
      BITANGENT_LOCATION,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      ( void* ) 0
      );

   glBindBuffer( GL_ARRAY_BUFFER, m_Buffers[ Bone_Buffer ] );

   glEnableVertexAttribArray( BONE_ID_LOCATION );
   glVertexAttribIPointer(
      BONE_ID_LOCATION,
      MAXIMUM_BONES_PER_VEREX,
      GL_UNSIGNED_INT,
      sizeof( BoneDataOfVertex ),
      ( const GLvoid* ) 0 );

   glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
   glVertexAttribPointer(
      BONE_WEIGHT_LOCATION,
      MAXIMUM_BONES_PER_VEREX,
      GL_FLOAT,
      GL_FALSE,
      sizeof( BoneDataOfVertex ),
      ( const GLvoid* ) ( sizeof( BoneId ) * MAXIMUM_BONES_PER_VEREX ) // Starting from the beginning of VertexToBoneMappingData
      );

   glBindVertexArray( 0 );
   // restore all of its children
   SceneNode::VOnRestore( pScene );
   OpenGLRenderManager::CheckError();

   return S_OK;
   }

int SkeletalMeshSceneNode::VRender( Scene *pScene )
   {
   //glUseProgram( m_Program );
   //glBindVertexArray( m_VAO );

   //Mat4x4 globalToWorld = VGetGlobalTransformPtr()->GetToWorld();

   //// Get the projection & view matrix from the camera class
   //Mat4x4 mWorldViewProjection = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * globalToWorld;

   //glUniformMatrix4fv( m_MVPUni, 1, GL_FALSE, &mWorldViewProjection[ 0 ][ 0 ] );

   //glUniformMatrix4fv( m_MUni, 1, GL_FALSE, &( globalToWorld[ 0 ][ 0 ] ) );

   //auto pLightManager = pScene->GetLightManagerPtr();

   //const auto& globalBoneTransform = m_pAnimationState->m_GlobalBoneTransform;
   //glUniformMatrix4fv( m_BoneTransformUni, globalBoneTransform.size(), GL_TRUE, &( globalBoneTransform[ 0 ][ 0 ][ 0 ] ) );

   //// Bind our texture in Texture Unit 0
   //glActiveTexture( GL_TEXTURE0 );
   //glBindTexture( GL_TEXTURE_2D, m_MeshTextureObj );
   //// Set our "myTextureSampler" sampler to user Texture Unit 0
   //glUniform1i( m_MeshTextureUni, 0 );

   //// Draw the triangles !
   //glDrawElements(
   //   GL_TRIANGLES,      // mode
   //   m_VerticesIndexCount,    // count
   //   GL_UNSIGNED_INT,   // type
   //   ( void* ) 0           // element array buffer offset
   //   );
   //glBindVertexArray( 0 );


   ////////
   glBindVertexArray( m_VAO  );
   OpenGLRenderManager::CheckError();
   auto renderPass = TBDRMainRenderer::RenderPass_GeometrySK;
   glUseProgram( m_pDeferredMainRenderer->m_Programs[ renderPass ] );

   
   glBindFramebuffer( GL_FRAMEBUFFER, m_pDeferredMainRenderer->m_FBO[ renderPass ] );
   OpenGLRenderManager::CheckError();
   auto view = pScene->GetCamera()->GetView();
   auto proj = pScene->GetCamera()->GetProjection();
   auto model = VGetGlobalTransformPtr()->GetToWorld();
   //auto mvp = pScene->GetCamera()->GetProjection() * pScene->GetCamera()->GetView() * pNode->VGetGlobalTransformPtr()->GetToWorld();
   // auto mvp = pScene->GetCamera()->GetProjection() * viewTest * pNode->VGetGlobalTransformPtr()->GetToWorld();
   auto mvp = proj * view * model;
   glUniformMatrix4fv( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometryPassUni_MVP ], 1, GL_FALSE, &mvp[ 0 ][ 0 ] );
   glUniformMatrix4fv( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometryPassUni_PrevMVP ], 1, GL_FALSE, &m_PrevMVP[ 0 ][ 0 ] );

   m_PrevMVP = mvp;
   Mat4x4 normalMat = ( view * model );
   normalMat = normalMat.Inverse().Transpose();
   glUniformMatrix4fv( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometryPassUni_NormalMat ], 1, GL_FALSE, &normalMat[ 0 ][ 0 ] );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, m_MeshTextureObj );
   if( m_UseNormalMap )
      {
      glActiveTexture( GL_TEXTURE1 );
      glBindTexture( GL_TEXTURE_2D, m_NormalMapTextureObj );
      glUniform1ui( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometryPassUni_UseNormalMap ], 1u );
      }
   else
      {
      glUniform1ui( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometryPassUni_UseNormalMap ], 0u );
      }

   const auto& globalBoneTransform = m_pAnimationState->m_GlobalBoneTransform;
   glUniformMatrix4fv( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometrySKPassUni_BoneTransform ], globalBoneTransform.size(), GL_TRUE, &( globalBoneTransform[ 0 ][ 0 ][ 0 ] ) );
   // std::vector< Mat4x4 > fake( m_pAnimationState->m_GlobalBoneTransform.size() );
   // glUniformMatrix4fv( m_pDeferredMainRenderer->m_Uniforms[ renderPass ][ TBDRMainRenderer::GeometrySKPassUni_BoneTransform ], globalBoneTransform.size(), GL_FALSE, &( fake[ 0 ][ 0 ][ 0 ] ) );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Buffers[ Index_Buffer ] );

   glDrawElements(
      GL_TRIANGLES,
      m_VerticesIndexCount,
      GL_UNSIGNED_INT,
      ( void* ) 0
      );

   OpenGLRenderManager::CheckError();

   glUseProgram( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   glBindVertexArray( 0 );
   ////////////////////////////////

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
   if( m_VAO )
      {
      glDeleteVertexArrays( 1, &m_VAO );
      m_VAO = 0;
      }

   /*if( m_pAnimationState )
      {
      AnimationManager::GetSingleton().VRemoveAnimationState( m_pAnimationState->m_pOwner->GetId() );
      m_pAnimationState.reset();
      }*/
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
   ENG_ASSERT( pNodeAnim->mNumRotationKeys > 0u );

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
   ENG_ASSERT( pNodeAnim->mNumScalingKeys > 0u );

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
   for( unsigned int nodeIdx = 0; nodeIdx < pAnimation->mNumChannels; ++nodeIdx )
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