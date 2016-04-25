#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLRenderer.h
////////////////////////////////////////////////////////////////////////////////

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
	   
      //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void )=0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void )=0;
	   //virtual void VDrawLine( const Vec3& from,const Vec3& to,const Color& color )=0;
      
      GLuint GetProgramID( void ) const { return m_Program; };

      GLuint CompileShader( const GLchar* const* pSrcData, const GLuint shaderID );
   private:
      GLuint m_Program;
   };