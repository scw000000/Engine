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

struct LightProperties
   {
   Color m_Diffuse;
   float m_Power;
   bool Init( TiXmlElement* pData );
   TiXmlElement* GenerateXML( void );
   TiXmlElement* GenerateOverridesXML( TiXmlElement* pResource );
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
   LightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform );

      const LightPropertiesPtr& GetLightPropertiesPtr( void ) const { return m_pLightProps; };
   };

class GLLightNode : public LightNode
   {
   public:
   GLLightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform )
      : LightNode( actorId, pRenderComponent, pLightProps, pTransform )
      {}

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

   public:
      LightManager( void );
	   /**
	    * @brief This function is called by Scene::OnRender
	    *
       *  Decide which lights can be active light
       *
	    * @param  pScene Scene * pScene
	    * @return void
	    */
	   void CalcLighting( Scene *pScene );
      // copy all of lights that effects this node into 
      void CalcLighting( SceneNode *pNode );
	   int GetActiveLightCount( void ) const { return m_ActiveLights.size(); }
	   bool AddLightNode( shared_ptr<LightNode> pNewLight );   
      
      Vec3* GetLightPosWorldSpace( void ) { return m_LightPosWorldSpace; }
      Vec3* GetLightDirection( void ) { return m_LightDir; }
      float* GetLightPower( void ) { return m_LightPower; }
      Color* GetLightColor( void ) { return m_LightColor; }
      Color* GetLightAmbient( void ) { return &m_LightAmbient; }
      void NewSceneNodeDelegate( IEventPtr pEvent );

   protected:
      Lights	m_Lights;
      Lights   m_ActiveLights;
      Vec3     m_LightPosWorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3		m_LightDir[ MAXIMUM_LIGHTS_SUPPORTED ];
      float     m_LightPower[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color		m_LightColor[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color		m_LightAmbient;
   };
