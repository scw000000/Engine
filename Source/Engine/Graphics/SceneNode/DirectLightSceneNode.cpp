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

   //m_FirstPassVertexShader.VOnRestore();
   //m_FirstPassFragmentShader.VOnRestore();

   //m_FirstPassProgram = OpenGLRenderer::GenerateProgram( m_FirstPassVertexShader.VGetShaderObject(), m_FirstPassFragmentShader.VGetShaderObject() );

   //m_FirstPassVertexShader.VReleaseShader( m_FirstPassProgram );
   //m_FirstPassFragmentShader.VReleaseShader( m_FirstPassProgram );

   //glGenVertexArrays( 1, &m_VAO );
   //glBindVertexArray( m_VAO );

   //// Generate depth texture for shadow mappikng
   //glGenTextures( 1, &m_ShadowMapTextureObj );
   //ENG_ASSERT( m_ShadowMapTextureObj );

   //glBindTexture( GL_TEXTURE_2D, m_ShadowMapTextureObj );
   //glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_ShadowMapDimension.x, m_ShadowMapDimension.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   //// For depth test
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
   //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );

   //// attach the texture and the depth buffer to the framebuffer
   //glGenFramebuffers( 1, &m_FrameBufferObj );
   //ENG_ASSERT( m_FrameBufferObj );

   //glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObj );

   //// No need for render buffer for depth test
   //glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMapTextureObj, 0 );

   //// No color output in the bound framebuffer, only depth.
   //glDrawBuffer( GL_NONE );

   //// Always check that our framebuffer is ok
   //if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
   //   return false;


   return S_OK; 
   }

void DirectLightNode::RenderShadowMap( shared_ptr< SceneNode > pTarget )
   {
   
   }