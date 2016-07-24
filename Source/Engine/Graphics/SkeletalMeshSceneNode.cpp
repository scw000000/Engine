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

#define VERTEX_LOCATION    0
#define UV_LOCATION        1
#define NORMAL_LOCATION    2
#define BONE_ID_LOCATION    3
#define BONE_WEIGHT_LOCATION    4

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\SKMeshFragmentShader.vertexshader";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\SKMeshFragmentShader.fragmentshader";

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
   LoadBones( pMeshExtra );

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
      NUM_BONES_PER_VEREX, 
      GL_UNSIGNED_INT, 
      sizeof( OpenGLRenderer::VertexToBoneMapping ), 
      ( const GLvoid* ) 0 );

   glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
   glVertexAttribPointer( 
      BONE_WEIGHT_LOCATION, 
      NUM_BONES_PER_VEREX, 
      GL_FLOAT, GL_FALSE, 
      sizeof( OpenGLRenderer::VertexToBoneMapping ), 
      ( const GLvoid* ) ( sizeof( unsigned int ) * NUM_BONES_PER_VEREX ) );

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
   if( pAnimation )
      {
      // The unit of the tick in this animation data
      float aiTicksPerSecond = ( float ) ( pAnimation->mTicksPerSecond != 0 ? pAnimation->mTicksPerSecond : 25.0f );
      // how many ticks have passed 
      float aiTimeInTicks = (float) elapsedMs * aiTicksPerSecond / 1000.f;
      // If the time is larger than the animation, mod it
      float aiAnimTicks = fmod( aiTimeInTicks, ( float ) pAnimation->mDuration );

      UpdateAnimationBones( aiAnimTicks, pAnimation, pAiScene->mRootNode, pScene->GetTopTransform() );
      }
   // for updates its children
   SceneNode::VOnUpdate( pScene, elapsedMs );
   return S_OK;
   }

void SkeletalMeshSceneNode::ReleaseResource( void )
   {
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

void SkeletalMeshSceneNode::LoadBones( shared_ptr<MeshResourceExtraData> pMeshExtra )
   {
   auto pAiScene = pMeshExtra->m_pScene;
   
   m_BoneMappingData.reserve( pMeshExtra->m_NumBones );
   for( unsigned int meshIdx = 0; meshIdx < pAiScene->mNumMeshes; ++meshIdx )
      {
      auto pMesh = pAiScene->mMeshes[ meshIdx ];
      for( unsigned int boneIdx = 0; boneIdx < pMesh->mNumBones; ++boneIdx )
         {
         auto pBone = pMesh->mBones[ boneIdx ];
         if( m_BoneMappingData.find( pBone->mName.C_Str() ) == m_BoneMappingData.end() )
            {
            m_BoneMappingData[ pBone->mName.C_Str() ] = BoneData();
            }
         }
      }
   }

void SkeletalMeshSceneNode::UpdateAnimationBones( float aiAnimTicks, aiAnimation* pAnimation, aiNode* pAiNode, const Transform& parentTransfrom )
   {
   std::string nodeName( pAiNode->mName.C_Str() );

   Matrix4f NodeTransformation( pAiNode->mTransformation );
   // find the corresponding aiNodeAnim of this node
   // each aiAnimation has multiple channels, which should map to a aiNode
   const aiNodeAnim* pNodeAnim = FindNodeAnim( pAnimation, NodeName );

   if( pNodeAnim )
      {
      // Interpolate scaling and generate scaling transformation matrix
      aiVector3D scale = CalcInterpolatedScaling( aiAnimTicks, pNodeAnim );
      Matrix4f ScalingM;
      ScalingM.InitScaleTransform( scale.x, scale.y, scale.z );

      // Interpolate rotation and generate rotation transformation matrix
      aiQuaternion quaternion = CalcInterpolatedRotation( aiAnimTicks, pNodeAnim );
      Matrix4f RotationM = Matrix4f( quaternion.GetMatrix() );

      // Interpolate translation and generate translation transformation matrix
      aiVector3D translation = CalcInterpolatedPosition( aiAnimTicks, pNodeAnim );
      Matrix4f TranslationM;
      TranslationM.InitTranslationTransform( translation.x, translation.y, translation.z );

      // Combine the above transformations
      NodeTransformation = TranslationM * RotationM * ScalingM;
      }

   Matrix4f GlobalTransformation = parentTransfrom * NodeTransformation;

   if( m_BoneMapping.find( NodeName ) != m_BoneMapping.end() )
      {
      uint BoneIndex = m_BoneMapping[ NodeName ];
      m_BoneInfo[ BoneIndex ].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[ BoneIndex ].BoneOffset;
      }

   for( unsigned int i = 0; i < pAiNode->mNumChildren; i++ )
      {
      UpdateAnimationBones( aiAnimTicks, pAnimation, pAiNode->mChildren[ i ], GlobalTransformation );
      }
   }


unsigned int SkeletalMeshSceneNode::FindPosition( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   for( unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++ )
      {
      if( AnimationTime < ( float ) pNodeAnim->mPositionKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }


unsigned int SkeletalMeshSceneNode::FindRotation( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumRotationKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++ )
      {
      if( AnimationTime < ( float ) pNodeAnim->mRotationKeys[ i + 1 ].mTime )
         {
         return i;
         }
      }
   ENG_ASSERT( 0 ); // this should not happen
   return 0;
   }


unsigned int SkeletalMeshSceneNode::FindScaling( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   ENG_ASSERT( pNodeAnim->mNumScalingKeys > 0 );

   for( unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++ )
      {
      if( AnimationTime < ( float ) pNodeAnim->mScalingKeys[ i + 1 ].mTime )
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

aiVector3D SkeletalMeshSceneNode::CalcInterpolatedPosition( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   if( pNodeAnim->mNumPositionKeys == 1 )
      {
      return pNodeAnim->mPositionKeys[ 0 ].mValue;
      }

   unsigned int PositionIndex = FindPosition( AnimationTime, pNodeAnim );
   unsigned int NextPositionIndex = ( PositionIndex + 1 );
   ENG_ASSERT( NextPositionIndex < pNodeAnim->mNumPositionKeys );
   float deltaTime = ( float ) ( pNodeAnim->mPositionKeys[ NextPositionIndex ].mTime - pNodeAnim->mPositionKeys[ PositionIndex ].mTime );
   float factor = ( AnimationTime - ( float ) pNodeAnim->mPositionKeys[ PositionIndex ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mPositionKeys[ PositionIndex ].mValue;
   const aiVector3D& end = pNodeAnim->mPositionKeys[ NextPositionIndex ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }


aiQuaternion SkeletalMeshSceneNode::CalcInterpolatedRotation( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   // we need at least two values to interpolate...
   if( pNodeAnim->mNumRotationKeys == 1 )
      {
      return pNodeAnim->mRotationKeys[ 0 ].mValue;
      }

   unsigned int RotationIndex = FindRotation( AnimationTime, pNodeAnim );
   unsigned int NextRotationIndex = ( RotationIndex + 1 );
   ENG_ASSERT( NextRotationIndex < pNodeAnim->mNumRotationKeys );
   float deltaTime = ( float ) ( pNodeAnim->mRotationKeys[ NextRotationIndex ].mTime - pNodeAnim->mRotationKeys[ RotationIndex ].mTime );
   float factor = ( AnimationTime - ( float ) pNodeAnim->mRotationKeys[ RotationIndex ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[ RotationIndex ].mValue;
   const aiQuaternion& endRotationQ = pNodeAnim->mRotationKeys[ NextRotationIndex ].mValue;
   aiQuaternion ret;
   aiQuaternion::Interpolate( ret, startRotationQ, endRotationQ, factor );
   ret.Normalize();
   return ret;
   }


aiVector3D SkeletalMeshSceneNode::CalcInterpolatedScaling( float AnimationTime, const aiNodeAnim* pNodeAnim ) const
   {
   aiVector3D ret;
   if( pNodeAnim->mNumScalingKeys == 1 )
      {
      ret = pNodeAnim->mScalingKeys[ 0 ].mValue;
      return;
      }
   // find the last key frame that its time is smaller than specified animation time
   unsigned int ScalingIndex = FindScaling( AnimationTime, pNodeAnim );
   unsigned int NextScalingIndex = ( ScalingIndex + 1 );
   ENG_ASSERT( NextScalingIndex < pNodeAnim->mNumScalingKeys );
   // time between these two key frames
   float deltaTime = ( float ) ( pNodeAnim->mScalingKeys[ NextScalingIndex ].mTime - pNodeAnim->mScalingKeys[ ScalingIndex ].mTime );
   float factor = ( AnimationTime - ( float ) pNodeAnim->mScalingKeys[ ScalingIndex ].mTime ) / deltaTime;
   ENG_ASSERT( factor >= 0.0f && factor <= 1.0f );
   const aiVector3D& start = pNodeAnim->mScalingKeys[ ScalingIndex ].mValue;
   const aiVector3D& end = pNodeAnim->mScalingKeys[ NextScalingIndex ].mValue;
   return ( 1 - factor ) * start + factor * end;
   }