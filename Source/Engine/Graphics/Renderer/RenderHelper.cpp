/*!
 * \file RenderHelper.cpp
 * \date 2017/06/04 9:09
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

#include "EngineStd.h"
#include "RenderHelper.h"

#define QUAD_VERTEX_LOCATION    0
#define QUAD_UV_LOCATION        1

#define QUAD_INDEX_LENGTH 6

GLuint OpenglRenderHelper::m_VBOs[ VBOs_Num ];

GLuint OpenglRenderHelper::m_VAO = 0u;

int OpenglRenderHelper::OnRestore( void )
   {
   OnRelease();

   const GLfloat QUAD_VERTEX_POSITION[] = {
      -1.0f, -1.0f, // bottom left
      1.0f, -1.0f, // bottom right
      1.0f, 1.0f, // top right
      -1.0f, 1.0f // top left
      };

   const GLfloat QUAD_UV_POSITION[] = {
      0.0f, 0.0f, // bottom left
      1.0f, 0.0f, // bottom right
      1.0f, 1.0f, // top right
      0.0f, 1.0f // top left
      };

   const unsigned short QUAD_VERTEX_INDEX[] = { 0, 1, 2, 0, 2, 3 };

   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );
   // 1st attribute buffer : vertices
   glGenBuffers( 1, &m_VBOs[ VBOs_Vertex ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ VBOs_Vertex ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_POSITION ), QUAD_VERTEX_POSITION, GL_STATIC_DRAW );
   glEnableVertexAttribArray( QUAD_VERTEX_LOCATION );
   glVertexAttribPointer(
      QUAD_VERTEX_LOCATION,                  // attribute
      2,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   // 2nd attribute buffer : UVs
   glGenBuffers( 1, &m_VBOs[ VBOs_UV ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ VBOs_UV ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_UV_POSITION ), QUAD_UV_POSITION, GL_STATIC_DRAW );
   glEnableVertexAttribArray( QUAD_UV_LOCATION );
   glVertexAttribPointer(
      QUAD_UV_LOCATION,                                // attribute
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      ( void* ) 0                          // array buffer offset
      );

   glGenBuffers( 1, &m_VBOs[ VBOs_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBOs[ VBOs_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, ENG_ARRAY_SIZE( QUAD_VERTEX_INDEX ), QUAD_VERTEX_INDEX, GL_STATIC_DRAW );

   glBindVertexArray( 0 );

   return S_OK;
   }

void OpenglRenderHelper::RenderQuad( void )
   {
   glBindVertexArray( m_VAO );
   glDrawElements(
      GL_TRIANGLES,      
      QUAD_INDEX_LENGTH,    
      GL_UNSIGNED_SHORT,   
      ( void* ) 0           
      );

   glBindVertexArray( 0 );
   }

void OpenglRenderHelper::SetRenderingAlpha( bool isAlpha )
   {
   if( isAlpha )
      {
      // Enable blending
      glEnable( GL_BLEND );
      glDisable( GL_CULL_FACE );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      }
   else
      {
      glDisable( GL_BLEND );
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
      glBlendFunc( GL_ONE, GL_ZERO );
      }
   }

void OpenglRenderHelper::SetBackgroundColor( const Color& color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }

void OpenglRenderHelper::DrawLine( const Vec3& fromWS, const Vec3& toWS, const Color& color )
   {
   shared_ptr<Scene> pScene = g_pApp->m_pEngineLogic->m_pWrold;

   auto pCamera = pScene->GetCamera();
   if( !pCamera )
      {
      return;
      }
   auto toCameraSpace = pCamera->VGetProperties().GetFromWorld();
   if( !pCamera->GetFrustum().VInside( toCameraSpace.Xform( fromWS ), toCameraSpace.Xform( toWS ) ) )
      {
      return;
      }

   Mat4x4 vp = ( pCamera->GetProjection() * pCamera->GetView() );
   Vec4 from_Proj = vp.Xform( fromWS, 1.0f );
   Vec4 to_Proj = vp.Xform( toWS, 1.0f );

   glLineWidth( 2.5 );
   // Draw in NDC space [-1, +1]
   glBegin( GL_LINE );
   glColor3f( color.m_Component.r, color.m_Component.g, color.m_Component.b );
   glVertex3f( from_Proj.x / from_Proj.w, from_Proj.y / from_Proj.w, from_Proj.z / from_Proj.w );
   glVertex3f( to_Proj.x / to_Proj.w, to_Proj.y / to_Proj.w, to_Proj.z / to_Proj.w );
   glEnd();
   }

void OpenglRenderHelper::CheckError( void )
   {
   GLenum errCode;
   while( ( errCode = glGetError() ) != GL_NO_ERROR )
      {
      std::string errorStr;
      switch( errCode )
         {
         case GL_INVALID_ENUM:                  errorStr = "INVALID_ENUM"; break;
         case GL_INVALID_VALUE:                 errorStr = "INVALID_VALUE"; break;
         case GL_INVALID_OPERATION:             errorStr = "INVALID_OPERATION"; break;
         case GL_STACK_OVERFLOW:                errorStr = "STACK_OVERFLOW"; break;
         case GL_STACK_UNDERFLOW:               errorStr = "STACK_UNDERFLOW"; break;
         case GL_OUT_OF_MEMORY:                 errorStr = "OUT_OF_MEMORY"; break;
         case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
         }
      ENG_ERROR( errorStr.c_str() );
      }
   }

void OpenglRenderHelper::OnRelease( void )
   {
   glDeleteVertexArrays( 1, &m_VAO );
   m_VAO = 0u;

   glDeleteBuffers( ENG_ARRAY_LENGTH( m_VBOs ), &m_VBOs[ 0 ] );
   ENG_ZERO_MEM( m_VBOs );
   }