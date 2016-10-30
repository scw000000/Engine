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
      virtual void RenderShadowMap( shared_ptr< SceneNode > pTarget ) override;
     // virtual int VOnUpdate( Scene *, unsigned long deltaMs ) override;

   protected:
      

   protected:
      Point          m_ShadowMapDimension;

      GLuint            m_FirstPassProgram;

      VertexShader		m_FirstPassVertexShader;
      FragmentShader		m_FirstPassFragmentShader;

      GLuint            m_VAO;

      GLuint         m_ShadowMapTextureObj;
      GLuint         m_FrameBufferObj;

   };


