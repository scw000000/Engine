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
      virtual GLuint VGetShaderObject( void ) const = 0;
      virtual void VSetResource( const Resource& resource ) = 0;
   };

class OpenGLShader : public IShader
   {
   public:
      OpenGLShader( const Resource& shaderResource );
      
      virtual void VReleaseShader( GLuint program ) override;
      virtual GLuint VGetShaderObject( void ) const override { return m_ShaderObj; };
      virtual void VSetResource( const Resource& resource ) override { m_ShaderResource = resource; }

   protected:
      void CompileAndSetShader( GLuint shaderType );


   protected:
      GLuint m_ShaderObj;
      Resource m_ShaderResource;
   };

class VertexShader : public OpenGLShader
   {
   public:
      VertexShader( const Resource& shaderResource = Resource( "" ) );
	   ~VertexShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile vertex shader
      void VOnRestore( void ) override;

   };

class FragmentShader : public OpenGLShader
   {
   public:
      FragmentShader( const Resource& shaderResource = Resource( "" ) );
	   ~FragmentShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile fragment shader
      void VOnRestore( void ) override;
   };