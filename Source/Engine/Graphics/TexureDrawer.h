#pragma once
/*!
 * \file TexureDrawer.h
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
#include "Shaders.h"

class OpenGLTextureDrawer
   {
   public:
      OpenGLTextureDrawer( void );
      int OnRestore( void );
      void DrawTexture( GLuint textureObj, const Point& offset, const Point& dimension );

   protected:
      void ReleaseResource( void );

      enum VB_TYPES
         {
         Vertex_Buffer,
         UV_Buffer,
         Index_Buffer,
         Num_Buffers
         };

   protected:

      GLuint            m_Program;
      VertexShader		m_VertexShader;
      FragmentShader		m_FragmentShader;

      GLuint            m_VAO;
      GLuint            m_Buffers[ Num_Buffers ];
      GLuint            m_TextureUni;
   };
