using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using SDL2;

namespace LevelEditorApp
   {
   public class SDLThreadObject
      {
      IntPtr m_WindowHandle;
      IntPtr m_TabHandle;
      int m_Width;
      int m_Height;

      private volatile bool m_ShouldLoop;
      public IntPtr m_pSDLWindow;
      public IntPtr m_pSDLWindowHandle;
      private SDL.SDL_EventFilter m_SDLEventFilter;

      private const uint INVALID_ACTOR_ID = 0;
      private uint m_SelectedActorId = INVALID_ACTOR_ID;

      public SDLThreadObject( IntPtr windowHandle, IntPtr tabHandle, int width, int height ) 
         {
         m_WindowHandle = windowHandle;
         m_TabHandle = tabHandle;
         m_Width = width;
         m_Height = height;
         m_ShouldLoop = true;
         }

      public void InitSDLWindow( )
         {
         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_EVERYTHING );
            m_pSDLWindow = SDL.SDL_CreateWindow( string.Empty,
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED,
               m_Width,
               m_Height,
               SDL.SDL_WindowFlags.SDL_WINDOW_BORDERLESS | SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL );
            //IntPtr pContext = SDL2.SDL.SDL_GL_CreateContext( m_pSDLWindow );
            SDL.SDL_SysWMinfo info = new SDL.SDL_SysWMinfo();
            SDL.SDL_GetWindowWMInfo( m_pSDLWindow, ref info );
            m_pSDLWindowHandle = info.info.win.window;
            NativeMethods.SetWindowPos(
               m_pSDLWindowHandle,
               m_WindowHandle,
               0,
               0,
               0,
               0,
               0x0401 // NOSIZE | SHOWWINDOW 
               );
            NativeMethods.SetParent( m_pSDLWindowHandle, m_TabHandle );
            NativeMethods.ShowWindow( m_pSDLWindowHandle, 1 ); // SHOWNORMAL
            m_SDLEventFilter = new SDL.SDL_EventFilter( this.SDLEventFilter );
            SDL.SDL_AddEventWatch( m_SDLEventFilter, new IntPtr() );
            NativeMethods.EditorMain( m_pSDLWindow, m_Width, m_Height );
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }

      public void ModifyActor( string actorOrerrides )
         {
         if( m_SelectedActorId != INVALID_ACTOR_ID )
            {
            NativeMethods.ModifyActor( m_SelectedActorId, actorOrerrides );
            }
         }

      public void PickActor()
         {
         uint selectedActorId = NativeMethods.PickActor();
         if( selectedActorId != INVALID_ACTOR_ID && selectedActorId != m_SelectedActorId )
            {
            m_SelectedActorId = selectedActorId;
            uint xmlSize = NativeMethods.GetActorXmlSize( m_SelectedActorId );
            if( xmlSize == 0 )
               return;

            IntPtr tempArray = Marshal.AllocCoTaskMem( ( (int) xmlSize + 1 ) * sizeof( char ) );
            NativeMethods.GetActorXML( tempArray, m_SelectedActorId );
            string actorXml = Marshal.PtrToStringAnsi( tempArray );
            Marshal.FreeCoTaskMem( tempArray );
            Program.s_Editor.BeginInvoke( Program.s_Editor.m_SetActorDataStringDelegate, actorXml );
            }
         }

      public int SDLEventFilter( IntPtr userData, IntPtr sdlevent )
         {
         SDL.SDL_Event eventInstance = (SDL.SDL_Event) Marshal.PtrToStructure( sdlevent, typeof( SDL.SDL_Event ) );
         switch( eventInstance.type )
            {
            case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
               if( eventInstance.button.button == SDL.SDL_BUTTON_LEFT )
                  {
                  PickActor();
                  }
               break;
            };
         return 1;
         }

      public void Run() 
         {
         InitSDLWindow();

         while( m_ShouldLoop )
            {
            NativeMethods.SingleLoop();
            }

         NativeMethods.Shutdown();
         Console.WriteLine( "SDL thread: terminating gracefully." );
         }

      public void RequestStop()
         {
         m_ShouldLoop = false;
         }

      public bool IsAlive() 
         {
         return m_ShouldLoop;
         }
      }

   }
