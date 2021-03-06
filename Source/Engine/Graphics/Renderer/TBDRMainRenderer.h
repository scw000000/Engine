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

#define AVERAGE_OVERLAP_LIGHTS_PER_TILE 200u

class TBDRMainRenderer : public IMainRenderer
   {
   friend class MeshSceneNode;
   friend class SkeletalMeshSceneNode;
   friend class OpenGLRenderManager;
   public:
      TBDRMainRenderer( void );
      virtual void VShutdown( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      // This functino is called at the end of HumanView::Render after calling Scene to Render
      virtual int VPostRender( void ) override;
    //  virtual int VOnRender( Scene *pScene, shared_ptr< ISceneNode > pNode ) override;
      virtual void VLoadLight( Lights& lights ) override;
      int OnRestoreTextures( GLuint depTex 
                             , GLuint mrt0Tex 
                             , GLuint mrt1Tex
                             , GLuint mrt2Tex
#ifdef _DEBUG
                             , GLuint tileDebugTex
#endif // _DEBUG
                             , GLuint lightingTex
                             , GLuint lightThresHoldTex
                             );

   protected:
      void ReleaseResource( void );
      int OnRestoreSSBO( void );
      int OnRestoreTileFrustum( Scene* pScene );
      int OnRestoreGeometryPass( void );
      int OnRestourLightCullPass( Scene* pScene );
      int OnRestoreLightingPass( Scene* pScene );
      void LightCulling( Scene* pScene );
      void CalculateLighting( void );
      void GenerateProgram( unsigned int renderPass );

   private:
      enum UsedTextures
         {
         UsdTex_Depth,
         UsdTex_Mrt0,
         UsdTex_Mrt1,
         UsdTex_Mrt2,
#ifdef _DEBUG
         UsdTex_TileDebugging,
#endif // _DEBUG
         UsdTex_SSAOBlur,
         UsdTex_Lighting,
         UsdTex_LightThreshold,
         UsdTex_Num
         };
      GLuint m_UsedTextures[ UsdTex_Num ];

      enum RenderPass
         {
         RenderPass_Geometry,
         RenderPass_GeometrySK,
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

      GLuint m_FBO[ RenderPass_Num ];

      enum GeometryPassUniforms
         {
         GeometryPassUni_MVP,
         GeometryPassUni_PrevMVP,
         GeometryPassUni_NormalMat,
         GeometryPassUni_AlbedoTexture,
         GeometryPassUni_NormalMapTexture,
         GeometryPassUni_UseNormalMap,
         GeometryPassUni_Num
         };

      enum GeometrySKPassUniforms
         {
         GeometrySKPassUni_MVP,
         GeometrySKPassUni_PrevMVP,
         GeometrySKPassUni_NormalMat,
         GeometrySKPassUni_AlbedoTexture,
         GeometrySKPassUni_NormalMapTexture,
         GeometrySKPassUni_UseNormalMap,
         GeometrySKPassUni_BoneTransform,
         GeometrySKPassUni_Num
         };

      enum LightCullingPassUniforms
         {
         LightCullPassUni_DepthTex,
         LightCullPassUni_Proj,
         LightCullPassUni_ScreenSize,
         LightCullPassUni_ValidLightNum,
         LightCullPassUni_DebugTex,
         LightCullPassUni_Num
         };

      enum LightingPassUniforms
         {
         LightingPassUni_DepthTex,
         LightingPassUni_MRT0,
         LightingPassUni_MRT1,
         LightingPassUni_SSAOBlurTex,
         LightingPassUni_UseSSAO,
         LightingPassUni_TileNum,
         LightingPassUni_HalfSizeNearPlane,
         LightingPassUni_Proj,
         LightingPassUni_Num
         };

      std::vector< GLuint > m_Uniforms[ RenderPass_Num ];

      unsigned int m_TileNum[ 2 ];
      GLuint m_OutPutTex[ 3 ];
      ComputeShader m_TileFrustumShader;

   };

typedef struct tileFrustum
   {
   public:
      Vec4 m_Planes[ 4 ];
   }TileFrustum;