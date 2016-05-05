#pragma once
/*!
 * \file OpenGLRenderer.h
 * \date 2016/05/02 20:24
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

// Maximum number of lights in GLSL shader and light manager
#define MAXIMUM_LIGHTS_SUPPORTED (8)

/*!
 * \class OpenGlRenderer
 *
 * \brief 
 *  LATER: try to implement shader resource manager to prevent unnecessary waste of memory
 * \author SCW
 * \date 05 2016
 */
class OpenGLRenderer : public IRenderer
   {
   public:
      OpenGLRenderer( void );
      ~OpenGLRenderer( void );
      
      virtual void VSetBackgroundColor( Color color ) override; 
	   virtual GLuint VOnRestore( void ) override;
	   virtual void VShutdown( void ) override;
      // This functino is called at the begin of HumanView::Render before calling Scene to Rendoer
	   virtual bool VPreRender( void ) override;
      // This functino is called at the end of HumanView::Render after calling Scene to Render
	   virtual bool VPostRender( void ) override;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) override;

      static void LoadTexture( GLuint* textureId, Resource& textureResource );
      static void LoadMesh( GLuint* vertexBuffer, float* radius, GLuint* uvBuffer, GLuint* indexBuffer, GLuint* normalBuffer, Resource& meshResource );
	   
      //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void )=0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void )=0;
	   //virtual void VDrawLine( const Vec3& from,const Vec3& to,const Color& color )=0;
      

      static GLuint CompileShader( const GLchar* const* pSrcData, const GLuint shaderID );

      static GLuint GenerateProgram( GLuint vertexShader, GLuint fragmentShader );

      static void SetRenderAlpha( bool isAlpha );
   private:

   };