/*!
 * \file DirectLightSceneNode.cpp
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
#include "DirectLightSceneNode.h"

const char* const FIRST_PASS_VERTEX_SHADER_FILE_NAME = "Effects\\DepthRTT.vertexshader";
const char* const FIRST_PASS_FRAGMENT_SHADER_FILE_NAME = "Effects\\DepthRTT.fragmentshader";

DirectLightNode::DirectLightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform )
   : LightNode( actorId, pRenderComponent, pLightProps, pTransform ),
   m_FirstPassVertexShader( Resource( FIRST_PASS_VERTEX_SHADER_FILE_NAME ) ),
   m_FirstPassFragmentShader( Resource( FIRST_PASS_FRAGMENT_SHADER_FILE_NAME ) )
   {
 
   }

int DirectLightNode::VOnRestore( Scene *pScene )
   {

   m_FirstPassVertexShader.VOnRestore();
   m_FirstPassFragmentShader.VOnRestore();

   m_FirstPassProgram = OpenGLRenderer::GenerateProgram( m_FirstPassVertexShader.VGetShaderObject(), m_FirstPassFragmentShader.VGetShaderObject() );

   m_FirstPassVertexShader.VReleaseShader( m_FirstPassProgram );
   m_FirstPassFragmentShader.VReleaseShader( m_FirstPassProgram );

   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

   // Generate depth texture for shadow mappikng
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

   // No need for render buffer for depth test
   glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );

   // No color output in the bound framebuffer, only depth.
   glDrawBuffer( GL_NONE );

   // Always check that our framebuffer is ok
   if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
      return false;


   return S_OK; 
   }

void DirectLightNode::VRenderShadowMap( shared_ptr< SceneNode > pTarget )
   {
   
   }

int DirectLightNode::VDelegateUpdate( Scene *pScene, unsigned long elapsedMs )
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
      // Transform to light space
      cameraFrustum.m_PlaneVerts[ i ] = VGetGlobalTransformPtr()->GetFromWorld().Xform( cameraFrustum.m_PlaneVerts[ i ] );

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
   m_Frustum.Init( maxDimension.x, minDimension.x, maxDimension.y, minDimension.y, maxDimension.z, 0.f );

   return S_OK;
   }