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
	   virtual bool VPreRender( void ) override;
	   virtual bool VPostRender( void ) override;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) override;
	   virtual void VSetWorldTransform( const Mat4x4 *m ) override;
	   virtual void VSetViewTransform( const Mat4x4 *m ) override;
	   virtual void VSetProjectionTransform( const Mat4x4 *m ) override;
	   //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void )=0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void )=0;
	   //virtual void VDrawLine( const Vec3& from,const Vec3& to,const Color& color )=0;
      
      GLuint GetProgramID( void ) const { return m_Program; };


   private:
      GLuint m_Program;
   };