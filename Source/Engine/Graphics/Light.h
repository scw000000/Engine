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
#include ".\SceneNode\SceneNodes.h"

#define MAXIMUM_LIGHTS_SUPPORTED             4096u
#define MAXIMUM_SHADOWMAP_TEXTURE_SUPPORTED  8u

#define LIGHT_TYPE_UNDEFINED   0u
#define LIGHT_TYPE_POINT       1u
#define LIGHT_TYPE_DIRECTIONAL 2u

struct LightProperties;
typedef shared_ptr<LightProperties> LightPropertiesPtr;


struct LightProperties
   {
   public:
      LightProperties( void );
      bool Init( TiXmlElement* pData );
      TiXmlElement* GenerateXML( void );

   public:
      //--------------------------------------------------------------
      Vec3  m_Color;
      unsigned int m_Type;
      //--------------------------------------------------------------
      Vec3  m_PositionVS;
      unsigned int m_Enabled;
      //--------------------------------------------------------------
      Vec3  m_DirectionVS;
      float  m_HalfConeAngle;
      //--------------------------------------------------------------
      Vec4  m_Attenuation;
      //--------------------------------------------------------------
      //float  m_Padding[ 3 ];
      //--------------------------------------------------------------

   };

class ILightNode : public SceneNode
   {
   public:
      ILightNode( const ActorId actorId, IRenderComponent* pRenderComponent, const LightPropertiesPtr& pLightProps, TransformPtr pTransform );
      const LightPropertiesPtr& GetLightPropertiesPtr( void ) const { return m_pLightProps; };
      virtual void VSetUpRenderShadowMap( void ) = 0;
      virtual void VRenderShadowMap( shared_ptr< ISceneNode > pNode ) = 0;
      virtual void VPreRenderShadowMap( void ) = 0;
      virtual bool VIsInside( const Vec3& worldPos, float radius = 0.f ) const = 0;
      virtual Mat4x4 VGetVPMatrix( void ) const = 0;
      virtual Mat4x4 VGetShadowMapMatrix( void ) const = 0;
      virtual GLuint VGetShadowMapTexture( void ) const = 0;

   protected:
      LightPropertiesPtr m_pLightProps;
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
      // copy all of lights that effects this node into constant buffer (Deprecated) 
      void CalcLighting( SceneNode *pNode );
      void RenderShadowMap( ISceneNode *pNode );

	   int GetActiveLightCount( void ) const { return m_ActiveLights.size(); }
	   bool AddLightNode( shared_ptr<ILightNode> pNewLight );   
      bool RemoveLightNode( shared_ptr<ILightNode> pRemovedLight );

      Vec3* GetLightPosWorldSpace( void ) { return m_LightPosWorldSpace; }
      Vec3* GetLightDirection( void ) { return m_LightDir; }
      float* GetLightPower( void ) { return m_LightPower; }
      Color* GetLightColor( void ) { return m_LightColor; }
      Mat4x4* GetShadowMapMatrix( void ) { return m_ShadowMapMatrix; }
      GLuint* GetShadowMaptexture( void ) { return m_ShadowMapTexture; }
      void NewSceneNodeDelegate( IEventPtr pEvent );
      void DestroySceneNodeDelegate( IEventPtr pEvent );

   protected:
      void CalcShadow( Scene *pScene, shared_ptr< ISceneNode > pNode );
      void RenderShadowMap( shared_ptr< ILightNode > ) const;

   protected:
      Lights	m_Lights;
      Lights   m_ActiveLights;
      Mat4x4   m_ShadowMapMatrix[ MAXIMUM_LIGHTS_SUPPORTED ];
      GLuint   m_ShadowMapTexture[ MAXIMUM_LIGHTS_SUPPORTED ];
      Color		m_LightColor[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3     m_LightPosWorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
      float     m_LightPower[ MAXIMUM_LIGHTS_SUPPORTED ];
      Vec3		m_LightDir[ MAXIMUM_LIGHTS_SUPPORTED ];
//      DeferredMainRenderer m_DeferredShader;
   };
