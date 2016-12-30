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

class DeferredMainRenderer : public MainRenderer
   {
   friend class MeshSceneNode;
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
      int OnRestoreTileFrustum( void );
      int OnRestoreTextures( void );
      int OnRestoreGeometryPass( void );
      int OnRestoreLightPass( void );
      
   private:
      enum RenderPass
         {
         RenderPass_Geometry,
         RenderPass_LightCull,
         RenderPass_Light,
         RenderPass_Num
         };
      GLuint m_Programs[ RenderPass_Num ];
      
      VertexShader m_VertexShaders[ RenderPass_Num ];
      FragmentShader m_FragmentShaders[ RenderPass_Num ];

      GLuint m_VAOs[ RenderPass_Num ];

      enum SSBOs
         {
         SSBO_TileFrustum,
         SSBO_VisibleLight,
         SSBO_Num
         };

      GLuint m_SSBOs[ SSBO_Num ];

      enum ScreenSpaceTextures
         {
         SST_Depth,
         SST_NormalGlossiness,
         SST_AlbedoMetalness,
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

      std::vector< std::vector< GLuint > > m_Uniforms;

      unsigned int m_TileNum[ 2 ];
      ComputeShader m_TileFrustumShader;
      ComputeShader m_LightListShader;
   };

typedef struct tileFrustum
   {
   public:
      Vec4 m_Planes[ 4 ];
   }TileFrustum;