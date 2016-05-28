/*!
* \file GlobalFunctions.cpp
* \date 2016/05/26 8:49
*
* \author SCW
* Contact: scw000000@gmail.com
*
* \brief
*
*
*
* \note
*/

#include "EditorStd.h"
#include "GlobalFunctions.h"
#include "..\Main\Engine.h"

EngineApp engineApp;

int EditorMain( int *pWindow, int screenWidth, int screenHeight )
   {
   // Set up checks for memory leaks.
   // Game Coding Complete reference - Chapter 21, page 834
   //
   int tmpDbgFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

   // set this flag to keep memory blocks around
   // tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

   // perform memory check for each alloc/dealloc
   //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

   /*
   _CRTDBG_LEAK_CHECK_DF is used at program initialization to force a
   //   leak check just before program exit. This is important because
   //   some classes may dynamically allocate memory in globally constructed
   //   objects.
   */
   tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

   _CrtSetDbgFlag( tmpDbgFlag );

   Logger::Init( "logging.xml" );

   g_pApp->m_EngineOptions.Init( "EngineOptions.xml" );

   g_pApp->InitInstance( ( SDL_Window* )pWindow, g_pApp->m_EngineOptions.m_ScreenSize.x, g_pApp->m_EngineOptions.m_ScreenSize.y );

  // g_pApp->MainLoop();

  // Logger::Destroy();

   return true;
   }

void SingleLoop( void )
   {
   //g_pApp->m_pRenderer->VPreRender();
   //g_pApp->m_pRenderer->VPreRender();

   g_pApp->SingleLoop();
   }
//
//#include <windows.h>
//#include <GL/gl.h>	
//
////using namespace System::Windows::Forms;
//
//namespace OpenGLForm
//   {
//   public ref class COpenGL : public System::Windows::Forms::NativeWindow
//      {
//         public:
//         COpenGL( System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight )
//            {
//            CreateParams^ cp = gcnew CreateParams;
//
//            // Set the position on the form
//            cp->X = 100;
//            cp->Y = 100;
//            cp->Height = iWidth;
//            cp->Width = iHeight;
//
//            // Specify the form as the parent.
//            cp->Parent = parentForm->Handle;
//
//            // Create as a child of the specified parent and make OpenGL compliant (no clipping)
//            cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
//
//            // Create the actual window
//            this->CreateHandle( cp );
//
//            m_hDC = GetDC( ( HWND )this->Handle.ToPointer() );
//
//            if( m_hDC )
//               MySetPixelFormat( m_hDC );
//            }
//
//         virtual System::Void Render( System::Void )
//            {
//            // Clear the color and depth buffers.
//            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//            }
//
//         System::Void SwapOpenGLBuffers( System::Void )
//            {
//            SwapBuffers( m_hDC );
//            }
//
//         private:
//         HDC m_hDC;
//         HGLRC m_hglrc;
//
//         protected:
//         ~COpenGL( System::Void )
//            {
//            this->DestroyHandle();
//            }
//
//         GLint MySetPixelFormat( HDC hdc )
//            {
//            PIXELFORMATDESCRIPTOR pfd = {
//               sizeof( PIXELFORMATDESCRIPTOR ),    // size of this pfd 
//               1,                                // version number 
//               PFD_DRAW_TO_WINDOW |              // support window 
//               PFD_SUPPORT_OPENGL |              // support OpenGL 
//               PFD_DOUBLEBUFFER,                 // double buffered 
//               PFD_TYPE_RGBA,                    // RGBA type 
//               24,                               // 24-bit color depth 
//               0, 0, 0, 0, 0, 0,                 // color bits ignored 
//               0,                                // no alpha buffer 
//               0,                                // shift bit ignored 
//               0,                                // no accumulation buffer 
//               0, 0, 0, 0,                       // accum bits ignored 
//               32,                               // 32-bit z-buffer     
//               0,                                // no stencil buffer 
//               0,                                // no auxiliary buffer 
//               PFD_MAIN_PLANE,                   // main layer 
//               0,                                // reserved 
//               0, 0, 0                           // layer masks ignored 
//               };
//
//            GLint  iPixelFormat;
//
//            // get the device context's best, available pixel format match 
//            if( ( iPixelFormat = ChoosePixelFormat( hdc, &pfd ) ) == 0 )
//               {
//               MessageBox::Show( "ChoosePixelFormat Failed" );
//               return 0;
//               }
//
//            // make that match the device context's current pixel format 
//            if( SetPixelFormat( hdc, iPixelFormat, &pfd ) == FALSE )
//               {
//               MessageBox::Show( "SetPixelFormat Failed" );
//               return 0;
//               }
//
//            if( ( m_hglrc = wglCreateContext( m_hDC ) ) == NULL )
//               {
//               MessageBox::Show( "wglCreateContext Failed" );
//               return 0;
//               }
//
//            if( ( wglMakeCurrent( m_hDC, m_hglrc ) ) == NULL )
//               {
//               MessageBox::Show( "wglMakeCurrent Failed" );
//               return 0;
//               }
//
//
//            return 1;
//            }
//      };
//   }