using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SDL2;

namespace LevelEditorApp
   {
   class SDLThread
      {
      public IntPtr m_pSDLWindow;
      private volatile bool m_IsAlive;

      public SDLThread() 
         {
         m_IsAlive = true;
         m_pSDLWindow = new IntPtr( 0 );
         }

      public void Init( IntPtr windowHandle, IntPtr parentHandle, int width, int height ) 
         {
         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_EVERYTHING );
            //IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );
            m_pSDLWindow = SDL.SDL_CreateWindow( string.Empty,
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED,
               width,
               height,
               SDL.SDL_WindowFlags.SDL_WINDOW_BORDERLESS | SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL );
            //IntPtr pContext = SDL2.SDL.SDL_GL_CreateContext( m_pSDLWindow );
            SDL.SDL_SysWMinfo info = new SDL.SDL_SysWMinfo();
            SDL.SDL_GetWindowWMInfo( m_pSDLWindow, ref info );
            IntPtr winHandle = info.info.win.window;
            NativeMethods.SetWindowPos(
               winHandle,
               windowHandle,
               0,
               0,
               0,
               0,
               0x0401 // NOSIZE | SHOWWINDOW 
               );
            NativeMethods.SetParent( winHandle, parentHandle );
            NativeMethods.ShowWindow( winHandle, 1 ); // SHOWNORMAL
            NativeMethods.EditorMain( m_pSDLWindow, width, height );

            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }

      unsafe public void Run() 
         {
         
         while( m_IsAlive )
            {
            SDL.SDL_Event evt = new SDL.SDL_Event();
            int i = 0;
            //while( SDL.SDL_PollEvent( out evt ) > 0 )
            //   {
            //   switch( evt.type )
            //      {
            //      case SDL.SDL_EventType.SDL_KEYDOWN:
            //         i++;
            //         break;
            //      }
            //   IntPtr ptr = new IntPtr( &evt );
            //   NativeMethods.PushSDLEvent( ptr );
            //   }
            //if( SDL.SDL_PollEvent( out evt ) > 0 )
            //   {
            //   switch( evt.type )
            //      {
            //      case SDL.SDL_EventType.SDL_KEYDOWN:
            //         i++;
            //         break;
            //      }
            //   IntPtr ptr = new IntPtr( &evt );
            //   }
            //SDL.SDL_PushEvent( ref evt );
            NativeMethods.SingleLoop();
            }
         }

      bool IsAlive() 
         {
         return m_IsAlive;
         }
      }

   
   }
