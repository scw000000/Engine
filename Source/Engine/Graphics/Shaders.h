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
      // reload and compile vertex shader
	   GLint OnRestore( Scene *pScene );
      // bind vertex shader to program
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );
	   void SetEnableLights(bool enableLights) { m_enableLights = enableLights; }

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

      // reload and compile fragment shader
	   GLint OnRestore( Scene *pScene );
      // bind fragment shader to program
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );

   protected:
      GLuint m_FragmentShader;
   };