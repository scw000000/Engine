#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Shaders.h
////////////////////////////////////////////////////////////////////////////////

class SceneNode;
class Scene;

class VertexShader
   {
   public:
	   VertexShader( void );
	   ~VertexShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile vertex shader
	   GLint OnRestore( Scene *pScene );
      // This function is called at begin of SceneNode::VRender
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );
	   void SetEnableLights(bool enableLights) { m_enableLights = enableLights; }
      GLuint GetVertexShader( void ) { return m_VertexShader; }

   protected:
      GLuint m_VertexShader;
      GLuint m_MVPMatrix;
      GLuint m_WorldMatrix;
	   bool	m_enableLights;
   };

class FragmentShader
   {
   public:
      
	   FragmentShader( void );
	   ~FragmentShader( void );
      // This function is called in SceneNode::Onrestore
      // reload and compile fragment shader
	   GLint OnRestore( Scene *pScene );
      // This function is called at begin of SceneNode::VRender
      // bind fragment shader to program
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );
      GLuint GetFragmentShader( void ) { return m_FragmentShader; }

   protected:
      GLuint m_FragmentShader;
   };