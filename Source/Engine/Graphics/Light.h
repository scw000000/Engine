#pragma once
/*!
 * \file Light.h
 * \date 2016/05/02 14:13
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
#include "SceneNodes.h"
#include "OpenGLRenderer.h"

struct LightProperties;
typedef shared_ptr<LightProperties> LightPropertiesPtr;

// Note: Light color is stored in the Material structure, which is already present in all SceneNodes.

//
// struct LightProperties				- Chapter 16, page 551
//
struct LightProperties
   {
   Color m_Diffuse;
	float	m_Attenuation[3];  /* Attenuation coefficients */
	float	m_Range;
	float	m_Falloff;
	float	m_Theta;
	float	m_Phi;
   };


//
// class LightNode						- Chapter 16, page 551
//
//    Note: In the book this class implements the LightNode in D3D11, but here it is a base
//    class. The derived classes make it possible to run the engine in D3D9 or D3D11.
//
class LightNode : public SceneNode
   {
   protected:
	   LightPropertiesPtr m_pLightProps;

   public:
	   LightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform );

      const LightPropertiesPtr& GetLightPropertiesPtr( void ) const { return m_pLightProps; };
   };

class GLLightNode : public LightNode
   {
   public:
	   GLLightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform )
		   : LightNode( actorId, renderComponent, pLightProps, pTransform ) { }

      virtual int VOnRestore( Scene *pScene ) override { return S_OK; } ;
      virtual int VOnUpdate( Scene *, const unsigned long deltaMs ) override;
   };


/*!
 * \class LightManager
 *
 * \brief 
 *  LATER: implement more dedicated ambient color and available light calculation
 * \author SCW
 * \date 05 2016li
 */
class LightManager
   {
	friend class Scene;

   protected:
	   Lights			m_Lights;
	   Vec4				m_vLightDir[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color			   m_vLightDiffuse[ MAXIMUM_LIGHTS_SUPPORTED ];
	   Vec4  			m_vLightAmbient;

   public:
	   /**
	    * @brief This function is called by Scene::OnRender
	    *
	    * @param  pScene Scene * pScene
	    * @return void
	    */
	    void CalcLighting( Scene *pScene );
      // copy all of lights that effects this node into 
	   void CalcLighting( SceneNode *pNode );
	   int GetLightCount(const SceneNode *node) { return m_Lights.size(); }
	   const Vec4 *GetLightAmbient(const SceneNode *node) { return &m_vLightAmbient; }
	   const Vec4 *GetLightDirection(const SceneNode *node) { return m_vLightDir; }
	   const Color *GetLightDiffuse(const SceneNode *node) { return m_vLightDiffuse; }
   };
