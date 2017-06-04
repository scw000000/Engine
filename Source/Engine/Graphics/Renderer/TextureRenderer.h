#pragma once
/*!
 * \file TexureRenderer.h
 * \date 2016/10/29 20:09
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
#include "..\Shaders.h"

class TextureRenderer : public IRenderer
   {
   public:
      TextureRenderer( void );
      virtual int VOnRestore( Scene* pScene ) override;
      virtual void VShutdown( void ) override;
      virtual int VPreRender( void ) override;
      virtual int VPostRender( void ) override;
      void DrawTexture( GLuint textureObj, const Point& offset, const Point& dimension );

   protected:
      void ReleaseResource( void );

   protected:
      
      GLuint            m_Program;
      VertexShader		m_VertexShader;
      FragmentShader		m_FragmentShader;

      GLuint            m_TextureUni;
   };
