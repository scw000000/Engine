#pragma once
/*!
 * \file Shaders.h
 * \date 2016/10/13 18:12
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

class IShader
   {
   public:
      virtual ~IShader( void ){ }
      virtual void VOnRestore( void ) = 0;
      virtual void VReleaseShader( GLuint program ) = 0;
    //  virtual GLuint VGetShaderObject( void ) const = 0;
      virtual void VSetResource( shared_ptr< Resource > pResource ) = 0;
   };

class OpenGLShader : public IShader
   {
   public:
      OpenGLShader( shared_ptr< Resource > pShaderResource );
      
      virtual void VReleaseShader( GLuint program ) override;
      virtual GLuint GetShaderObject( void ) const { return m_ShaderObj; };
      virtual void VSetResource( shared_ptr< Resource > pResource ) override { m_pShaderResource = pResource; }

   protected:
      void CompileAndSetShader( GLuint shaderType );


   protected:
      GLuint m_ShaderObj;
      shared_ptr< Resource > m_pShaderResource;
   };

class VertexShader : public OpenGLShader
   {
   public:
      VertexShader( shared_ptr< Resource > pResource );
	   ~VertexShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile vertex shader
      void VOnRestore( void ) override;

   };

class FragmentShader : public OpenGLShader
   {
   public:
      FragmentShader( shared_ptr< Resource > pResource );
	   ~FragmentShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile fragment shader
      void VOnRestore( void ) override;
   };

class ComputeShader : public OpenGLShader
   {
      public:
      ComputeShader( shared_ptr< Resource > pResource ) : OpenGLShader( pResource ) {};
      ~ComputeShader( void ) {};
      // This function is called in SceneNode::Onrestore
      // reload and compile fragment shader
      void VOnRestore( void ) override;
   };