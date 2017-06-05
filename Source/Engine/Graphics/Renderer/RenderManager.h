/*!
 * \file RenderManager.h
 * \date 2016/12/26 13:12
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
#include "TBDRMainRenderer.h"
#include "TextureRenderer.h"
#include "SSAORenderer.h"

class OpenGLRenderManager : public IRenderManager
   {
   public:
      OpenGLRenderManager( void );

      virtual int VInit( void ) override;
      virtual int VOnRestore( Scene* pScene ) override;
      virtual int VPreRender( void ) override;
      virtual int VLightingPass( Scene* pScene ) override;
      virtual int VSSAOPass( void ) override;
      virtual int VMotionBlurPass( void ) override;
      virtual int VBloomPass( void ) override;
      virtual int VToneMappingPass( void ) override;
      virtual int VPostRender( void ) override;
      virtual void VShutDown( void ) override;
      virtual IMainRenderer& VGetMainRenderer( void ) override { return m_MainRenderer; };
    //  virtual void VCheckError( void ) const override;
      static void CheckError( void );
   protected:
      int OnRestoreTextures( void );

   protected:

      enum ScreenSpaceTextures
         {
         SST_Depth,
         SST_MRT0,
         SST_MRT1,
         SST_SSAO,
         SST_SSAOBlur,
#ifdef _DEBUG
         SST_TileDebugging,
#endif // _DEBUG   
         SST_Num
         };

      GLuint m_SST[ SST_Num ];

      TBDRMainRenderer m_MainRenderer;
      SSAORenderer m_SSAORenderer;
      TextureRenderer m_TextureDrawer;
   };