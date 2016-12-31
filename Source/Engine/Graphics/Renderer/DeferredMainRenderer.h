#pragma once
/*!
 * \file DeferredShader.h
 * \date 2016/11/20 9:01
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
#include "..\Shaders.h"
#include "MainRenderer.h"

#define AVERAGE_OVERLAP_LIGHTS_PER_TILE 200u

class DeferredMainRenderer : public MainRenderer
   {
   friend class MeshSceneNode;
   friend class OpenGLRenderManager;
   public:
      DeferredMainRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VOnRestore( void ) override;
    //  virtual int VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode ) override;
      virtual void VLoadLight( Lights& lights ) override;

   protected:
      void ReleaseResource( void );
      int OnRestoreSSBO( void );
      int OnRestoreTextures( void );
      int OnRestoreTileFrustum( void );
      int OnRestoreGeometryPass( void );
      int OnRestourLightCullPass( void );
      int OnRestoreLightingPass( void );
      void LightCulling( Scene* pScene );
      void GenerateProgram( unsigned int renderPass );

   private:
      enum RenderPass
         {
         RenderPass_Geometry,
         RenderPass_LightCulling,
         RenderPass_Lighting,
         RenderPass_Num
         };
      GLuint m_Programs[ RenderPass_Num ];
      
      std::vector< shared_ptr< OpenGLShader > > m_Shaders[ RenderPass_Num ];

    //  GLuint m_VAOs[ RenderPass_Num ];

      enum SSBOs
         {
         SSBO_TileFrustum,
         SSBO_LightIndexCount,
         SSBO_LightIndexList,
         SSBO_LightIndexGrid,
         SSBO_LightProperties,
         SSBO_Num
         };

      GLuint m_SSBOs[ SSBO_Num ];

      enum ScreenSpaceTextures
         {
         SST_Depth,
         SST_NormalGlossiness,
         SST_AlbedoMetalness,
         SST_TileDebugging,
         SST_Num
         };

      GLuint m_SST[ SST_Num ];

      GLuint m_FBO[ RenderPass_Num ];

      enum GeometryPassUniforms
         {
         GeometryPassUni_MVP,
         GeometryPassUni_NormalMat,
         GeometryPassUni_AlbedoTexture,
         GeometryPassUni_Num
         };

      enum LightCullingPassUniforms
         {
         LightCullPassUni_DepthTex,
         LightCullPassUni_Proj,
         LightCullPassUni_ScreenSize,
         LightCullPassUni_DebugTex,
         LightCullPassUni_Num
         };

      std::vector< GLuint > m_Uniforms[ RenderPass_Num ];

      unsigned int m_TileNum[ 2 ];
      ComputeShader m_TileFrustumShader;
   };

typedef struct tileFrustum
   {
   public:
      Vec4 m_Planes[ 4 ];
   }TileFrustum;