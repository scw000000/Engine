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

class OpenGLRenderer;

class OpenGLRenderManager : public IRenderManager
   {
   public:
      OpenGLRenderManager( void );

      virtual int VInit( void ) override;
      virtual int VOnRestore( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      virtual void VShutDown( void ) override;
      virtual IRenderer* VGetRenderer( void ) const override { return m_pRenderer; };
    //  virtual void VCheckError( void ) const override;
      static void CheckError( void );
   protected:
      OpenGLRenderer *m_pRenderer;
   };