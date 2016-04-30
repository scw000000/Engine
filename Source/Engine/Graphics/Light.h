#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Light.h
////////////////////////////////////////////////////////////////////////////////
#include "SceneNodes.h"
#include "OpenGLRenderer.h"

// Note: Light color is stored in the Material structure, which is already present in all SceneNodes.

//
// struct LightProperties				- Chapter 16, page 551
//
struct LightProperties
   {
   Color m_Color;
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
	   LightProperties m_LightProps;

   public:
	   LightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const LightProperties &props, const Transform& t = Transform::g_Identity );
   };

class GLLightNode : public LightNode
   {
   public:
	   GLLightNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent,  const LightProperties &lightProps, const Transform& t)
		   : LightNode( actorId, renderComponent, lightProps,  t ) { }

      virtual int VOnRestore( Scene *pScene ) override { return S_OK; } ;
      virtual int VOnUpdate( Scene *, const unsigned long deltaMs ) override;
   };


//
// class LightManager					- Chapter 16, 553
//
class LightManager
   {
	friend class Scene;

   protected:
	   Lights			m_Lights;
	   Vec4				m_vLightDir[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color			   m_vLightDiffuse[ MAXIMUM_LIGHTS_SUPPORTED ];
	   Vec4  			m_vLightAmbient;

public:
	void CalcLighting( Scene *pScene );
   // copy all of lights that effects this node into 
	void CalcLighting( SceneNode *pNode );
	int GetLightCount(const SceneNode *node) { return m_Lights.size(); }
	const Vec4 *GetLightAmbient(const SceneNode *node) { return &m_vLightAmbient; }
	const Vec4 *GetLightDirection(const SceneNode *node) { return m_vLightDir; }
	const Color *GetLightDiffuse(const SceneNode *node) { return m_vLightDiffuse; }
};
