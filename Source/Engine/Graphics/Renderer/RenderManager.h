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
#include "DeferredMainRenderer.h"
#include "TextureRenderer.h"

class OpenGLRenderManager : public IRenderManager
   {
   public:
      OpenGLRenderManager( void );

      virtual int VInit( void ) override;
      virtual int VOnRestore( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      virtual void VShutDown( void ) override;
      virtual IMainRenderer& VGetMainRenderer( void ) override { return m_MainRenderer; };
    //  virtual void VCheckError( void ) const override;
      static void CheckError( void );
   protected:
      DeferredMainRenderer m_MainRenderer;
      TextureRenderer m_TextureDrawer;
   };