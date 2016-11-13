/*!
 * \file DirectionalLightSceneNode.cpp
 * \date 2016/10/29 17:21
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "DirectionalLightSceneNode.h"

const char* const FIRST_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DepthRTT.vertexshader";
const char* const FIRST_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DepthRTT.fragmentshader";

#define VERTEX_LOCATION    0

DirectionalLightNode::DirectionalLightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform )
   : LightNode( actorId, pRenderComponent, pLightProps, pTransform ),
   m_FirstPassVertexShader( Resource( FIRST_PASS_VERTEX_SHADER_FILE_NAME ) ),
   m_FirstPassFragmentShader( Resource( FIRST_PASS_FRAGMENT_SHADER_FILE_NAME ) )
   {
   m_ShadowMapDimension = g_pApp->GetScreenSize();
   m_MVPUni = -1;
   }

int DirectionalLightNode::VOnRestore( Scene *pScene )
   {

   m_FirstPassVertexShader.VOnRestore();
   m_FirstPassFragmentShader.VOnRestore();

   m_FirstPassProgram = OpenGLRenderer::GenerateProgram( m_FirstPassVertexShader.VGetShaderObject(), m_FirstPassFragmentShader.VGetShaderObject() );

   m_FirstPassVertexShader.VReleaseShader( m_FirstPassProgram );
   m_FirstPassFragmentShader.VReleaseShader( m_FirstPassProgram );

   glUseProgram( m_FirstPassProgram );

   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

   // Generate depth texture for shadow map
   glGenTextures( 1, &m_ShadowMapTextureObj );
   ENG_ASSERT( m_ShadowMapTextureObj );

   glBindTexture( GL_TEXTURE_2D, m_ShadowMapTextureObj );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_ShadowMapDimension.x, m_ShadowMapDimension.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   // For depth test
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   // attach the texture and the depth texture to the framebuffer
   glGenFramebuffers( 1, &m_FrameBufferObj );
   ENG_ASSERT( m_FrameBufferObj );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObj );

   // No need for render buffer for depth test, 0: mipmap  level of texture
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );

   // No color output in the bound framebuffer, only depth.
   glDrawBuffer( GL_NONE );

   // Always check that our framebuffer is ok
   auto result = glCheckFramebufferStatus( GL_FRAMEBUFFER );
   if( result != GL_FRAMEBUFFER_COMPLETE )
      {
      ENG_ASSERT( 0 && "Frame buffer error" );
      }

   m_MVPUni = glGetUniformLocation( m_FirstPassProgram, "uMVP" );

   glBindVertexArray( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   return S_OK; 
   }

void DirectionalLightNode::VSetUpRenderShadowMap( void )
   {
   glUseProgram( m_FirstPassProgram );
   glBindVertexArray( m_VAO );

   glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObj );
   glViewport( 0, 0, m_ShadowMapDimension.x, m_ShadowMapDimension.y );

   glEnable( GL_CULL_FACE );
   glCullFace( GL_BACK ); 
   }

void DirectionalLightNode::VRenderShadowMap( const Mat4x4& vp, shared_ptr< ISceneNode > pNode )
   {
   auto& vertexInfo = pNode->VGetShadowVertexInfo();

   // not valid vertex info, return
   if( vertexInfo.m_Vertexbuffer == 0 || vertexInfo.m_Elementbuffer == 0 || vertexInfo.m_VertexCount == 0 )
      {
      return;
      }
   //glVertexArrayElementBuffer
   
   auto mvp = VGetVPMatrix();
   //auto& worldPos = VGetGlobalTransformPtr()->GetToWorldPosition();
   //auto view = Mat4x4::LookAt( worldPos, worldPos + VGetGlobalTransformPtr()->GetForward(), VGetGlobalTransformPtr()->GetUp() );
   //mvp = vp * pNode->VGetGlobalTransformPtr()->GetToWorld();
   mvp = mvp * pNode->VGetGlobalTransformPtr()->GetToWorld();

   glUniformMatrix4fv( m_MVPUni, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

   glBindBuffer( GL_ARRAY_BUFFER, vertexInfo.m_Vertexbuffer );
   glEnableVertexAttribArray( VERTEX_LOCATION );
   glVertexAttribPointer(
      VERTEX_LOCATION,  // The attribute we want to configure
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vertexInfo.m_Elementbuffer );

   glDrawElements(
      GL_TRIANGLES,      // mode
      vertexInfo.m_VertexCount,    // count
      GL_UNSIGNED_INT, // type
      ( void* ) 0           // element array buffer offset
      );

   glBindVertexArray( 0 );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   auto screenDimension = g_pApp->GetScreenSize();
   glViewport( 0, 0, screenDimension.x, screenDimension.y );
   }

void DirectionalLightNode::VPreRenderShadowMap( void )
   {
   glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObj );
 //  glClearColor( 0.f, 0.f, 0.f, 1.f );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glBindFramebuffer( GL_FRAMEBUFFER, 0 );
   }

int DirectionalLightNode::VDelegateUpdate( Scene *pScene, unsigned long elapsedMs )
   {
   auto pCamera = pScene->GetCamera();
   auto cameraFrustum = pCamera->GetFrustum();
   Vec3 maxDimension;
   Vec3 minDimension;
   Vec3 cameraVerts[ 8 ];
   for( int i = 0; i < 8; ++i )
      {
      // Transform to world space
      cameraFrustum.m_PlaneVerts[ i ] = pCamera->VGetGlobalTransformPtr()->GetToWorld().Xform( cameraFrustum.m_PlaneVerts[ i ] );
    //  ENG_LOG( "Test", ToStr( cameraFrustum.m_PlaneVerts[ i ] ) + "global space" );
      
      // Transform to light space
      cameraFrustum.m_PlaneVerts[ i ] = VGetGlobalTransformPtr()->GetFromWorld().Xform( cameraFrustum.m_PlaneVerts[ i ] );
    //  ENG_LOG( "Test", ToStr( cameraFrustum.m_PlaneVerts[ i ] ) + "light space"  );
      if( i == 0 )
         {
         maxDimension = cameraFrustum.m_PlaneVerts[ i ];
         minDimension = cameraFrustum.m_PlaneVerts[ i ];
         }
      else
         {
         maxDimension.x = std::max( maxDimension.x, cameraFrustum.m_PlaneVerts[ i ].x );
         maxDimension.y = std::max( maxDimension.y, cameraFrustum.m_PlaneVerts[ i ].y );
         maxDimension.z = std::max( maxDimension.z, cameraFrustum.m_PlaneVerts[ i ].z );

         minDimension.x = std::min( minDimension.x, cameraFrustum.m_PlaneVerts[ i ].x );
         minDimension.y = std::min( minDimension.y, cameraFrustum.m_PlaneVerts[ i ].y );
        // minDimension.z = std::min( minDimension.z, cameraFrustum.m_PlaneVerts[ i ].z ); // no need for min z
         }
      }
 //  ENG_LOG( "Test", ToStr( maxDimension ) );
  // ENG_LOG( "Test", ToStr( minDimension ) );
 //  ENG_LOG( "Test", ToStr( GetForward() ) + " Forward" );
 //  ENG_LOG( "Test", ToStr( VGetGlobalTransformPtr()->GetToWorldPosition() ) + " position" );
   m_Frustum.Init( maxDimension.x, minDimension.x, maxDimension.y, minDimension.y, maxDimension.z, 0.f );

   return S_OK;
   }

bool DirectionalLightNode::VIsInside( const Vec3& worldPos, float radius ) const
   {
   Vec3 localPos = VGetGlobalTransformPtr()->GetFromWorld().Xform( worldPos );
   return m_Frustum.VInside( localPos, radius );
   }

Mat4x4 DirectionalLightNode::VGetVPMatrix( void ) const
   {
   auto& worldPos = VGetGlobalTransformPtr()->GetToWorldPosition();

   auto view = Mat4x4::LookAt( worldPos, worldPos + VGetGlobalTransformPtr()->GetForward(), VGetGlobalTransformPtr()->GetUp() );
   //auto projection = Mat4x4::Ortho( m_Frustum.m_Right, m_Frustum.m_Left, m_Frustum.m_Top, m_Frustum.m_Bottom, m_Frustum.m_FarDis, m_Frustum.m_NearDis );
   auto projection = Mat4x4::Ortho( -m_Frustum.m_Left, -m_Frustum.m_Right, m_Frustum.m_Top, m_Frustum.m_Bottom, m_Frustum.m_FarDis, m_Frustum.m_NearDis );
   return projection * view;
   }

Mat4x4 DirectionalLightNode::VGetShadowMapMatrix( void ) const
   {
   auto ret = VGetVPMatrix();
   // This matrix is for mapping to shadow map texture, from [ +-1 ] to [ 0, 1 ]
   return Mat4x4(
      0.5, 0.0, 0.0, 0.0,
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
      ) * ret;
   }