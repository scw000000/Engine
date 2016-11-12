#pragma once
/*!
 * \file DirectLightSceneNode.h
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
#include "..\Light.h"
#include "..\Shaders.h"

class DirectLightNode : public LightNode
   {
   public:
   DirectLightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform );

      virtual int VOnRestore( Scene *pScene ) override;
      virtual void VSetUpRenderShadowMap( void ) override;
      virtual void VRenderShadowMap( const Mat4x4& vp, shared_ptr< ISceneNode > pNode ) override;
      virtual void VPreRenderShadowMap( void ) override;
      // Update Frustum size here       
      virtual int VDelegateUpdate( Scene *pScene, unsigned long elapsedMs ) override;
      virtual bool VIsInside( const Vec3& worldPos, float radius = 0.f ) const override;
      virtual Mat4x4 VGetVPMatrix( void ) const override;

   protected:
      

   protected:
      Point          m_ShadowMapDimension;

      GLuint            m_FirstPassProgram;

      VertexShader		m_FirstPassVertexShader;
      FragmentShader		m_FirstPassFragmentShader;

      GLuint            m_VAO;

      GLuint         m_ShadowMapTextureObj;
      GLuint         m_FrameBufferObj;

      GLuint         m_MVPUni;

      OrthogonalFrustum  m_Frustum;

   };


