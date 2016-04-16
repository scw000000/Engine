////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLRenderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "EngineStd.h"
#include "OpenGLRenderer.h"

void VSetBackgroundColor( Color color )
   {
   glClearColor( color.m_Component.r, color.m_Component.g, color.m_Component.b, color.m_Component.a );
   }

GLuint VOnRestore( void )
   {
   return 0;
   }

void VShutdown( void )
   {
   
   }

bool VPreRender( void )
   {
   
   }

bool VPostRender( void )
   {
   
   }

void VSetWorldTransform( const Mat4x4 *m )
   {
   
   }

void VSetViewTransform( const Mat4x4 *m )
   {
   
   }