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

	   GLint OnRestore( Scene *pScene );
	   GLint SetupRender( Scene *pScene, SceneNode *pNode );
	   void SetEnableLights(bool enableLights) { m_enableLights = enableLights; }

   protected:
      GLuint m_VetrexShader;
      GLuint m_MVPMatrix;
      GLuint m_WorldMatrix;
	   bool	m_enableLights;
   };