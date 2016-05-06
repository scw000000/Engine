#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////

class SceneNode;
class Scene;

class VertexShader
   {
   public:
	   VertexShader( const char* pResourceFile );
	   ~VertexShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile vertex shader
	   void OnRestore( Scene *pScene );
      // 
	   /**
	    * @brief  
	    *
       *  This function is called at begin of SceneNode::VRender 
       *  The original implementation of SetupRender
	    * @param  pScene Scene * pScene
	    * @param  pNode SceneNode * pNode
	    * @return GLint
	    */
	    GLint SetupRender( Scene *pScene, SceneNode *pNode );
	   void SetEnableLights(bool enableLights) { m_enableLights = enableLights; }
      GLuint GetVertexShader( void ) { return m_VertexShader; }
      void ReleaseShader( GLuint program );

   protected:
      GLuint m_VertexShader;
      GLuint m_MVPMatrix;
      GLuint m_WorldMatrix;
	   bool	m_enableLights;
      Resource m_ShaderResource;

   };

class FragmentShader
   {
   public:
      
	   FragmentShader( const char* pResourceFile );
	   ~FragmentShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile fragment shader
	   void OnRestore( Scene *pScene );
      // This function is called at begin of SceneNode::VRender
      // bind fragment shader to program
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );
      GLuint GetFragmentShader( void ) { return m_FragmentShader; }
      void ReleaseShader( GLuint program );

   protected:
      GLuint m_FragmentShader;
      Resource m_ShaderResource;
   };