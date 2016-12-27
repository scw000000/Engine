/*!
 * \file OpenGLRenderer.cpp
 * \date 2016/11/11 11:38
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

#include "EngineStd.h"
#include "MainRenderer.h"

//OpenGLTextureDrawer MainRenderer::s_TextureDrawer;

MainRenderer::MainRenderer( void )
   {

   }

MainRenderer::~MainRenderer( void )
   {

   }

void MainRenderer::VSetBackgroundColor( const Color& color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }
//
//int MainRenderer::VOnRestore( void )
//   {
//   s_TextureDrawer.OnRestore();
//   return S_OK;
//   }
//
//void MainRenderer::VShutdown( void )
//   {}

int MainRenderer::VPreRender( void )
   {
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   return S_OK;
   }

int MainRenderer::VPostRender( void )
   {
   auto screensize = g_pApp->GetScreenSize();
   float xSize = 300.f;
   float ySize = xSize * ( float ) screensize.y / ( float ) screensize.x;

  // MainRenderer::s_TextureDrawer.DrawTexture( 1, Point( 0, 0 ), Point( xSize, ( Sint32 ) ( ySize ) ) ); // for testing
   SDL_GL_SwapWindow( g_pApp->GetWindow() );
   return S_OK;
   }
/*
void OpenGLRenderer::VSetWorldTransform( const Mat4x4 *m )
{

}

void OpenGLRenderer::VSetViewTransform( const Mat4x4 *m )
{

}

void OpenGLRenderer::VSetProjectionTransform( const Mat4x4 *m )
{

}*/

void MainRenderer::VSetRenderingAlpha( bool isAlpha )
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

void MainRenderer::VDrawLine( const Vec3& fromWS, const Vec3& toWS, const Color& color ) const
   {
   shared_ptr<Scene> pScene = g_pApp->m_pEngineLogic->m_pWrold;

   auto pCamera = pScene->GetCamera();
   if( !pCamera )
      {
      return;
      }
   auto toCameraSpace = pCamera->VGetProperties().GetFromWorld();
   if( !pCamera->GetFrustum().VInside( toCameraSpace.Xform( fromWS ) ) && !pCamera->GetFrustum().VInside( toCameraSpace.Xform( toWS ) ) )
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