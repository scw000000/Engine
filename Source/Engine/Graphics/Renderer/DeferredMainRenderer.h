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
   public:
      DeferredMainRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VOnRestore( void ) override;
    //  virtual int VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode ) override;
   
   protected:
      void ReleaseResource( void );
      int OnRestoreSSBO( void );
      int OnRestoreTileFrustum( void );
      int OnRestoreGeometryPass( void );
      int OnRestoreLightPass( void );
      
   private:
      enum RenderPass
         {
         RenderPass_Geometry,
         RenderPass_Light,
         RenderPass_Num
         };
      GLuint m_Programs[ RenderPass_Num ];
      
      VertexShader m_VertexShaders[ RenderPass_Num ];
      FragmentShader m_FragmentShaders[ RenderPass_Num ];

      GLuint m_VAOs[ RenderPass_Num ];

      enum ScreenSpaceTextures
         {
         SST_Depth,
         SST_Normal,
         SST_Texture,
         SST_Ambient,
         SST_Diffuse,
         SST_Specular,
         SST_Num
         };

      GLuint m_SST[ SST_Num ];

      GLuint m_FBO[ RenderPass_Num ];

      enum GeometryPassUniforms
         {
         GeometryPassUni_MVP,
         GeometryPassUni_NormalMat,
         GeometryPassUni_MeshTexture,
         GeometryPassUni_Num
         };

      std::vector< std::vector< GLuint > > m_Uniforms;

      GLuint m_MVPUni;
      GLuint m_MUni;
      unsigned int m_TileNum[ 2 ];
      ComputeShader m_TileFrustumShader;
      GLuint m_TileFrustumSSBO;
   };

typedef struct tileFrustum
   {
   public:
      Vec4 m_Planes[ 4 ];
   }TileFrustum;