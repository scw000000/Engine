#pragma once
/*!
 * \file RenderHelper.h
 * \date 2017/06/04 9:08
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

class OpenglRenderHelper
   {
   public:
      static int OnRestore( void );
      static void RenderQuad( void );
      static void SetRenderingAlpha( bool isAlpha );
      static void SetBackgroundColor( const Color& color ) ;
      static void DrawLine( const Vec3& fromWS, const Vec3& toWS, const Color& color );
      static void CheckError( void );
   private:
      static void OnRelease( void );

   private:
      enum VBOs
         {
         VBOs_Vertex,
         VBOs_UV,
         VBOs_Index,
         VBOs_Num
         };
   static GLuint m_VBOs[ VBOs_Num ];
   
   static GLuint m_VAO;
   };