using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Xml;

using SDL2;
namespace LevelEditorApp
   {
   public class SDLThreadObject
      {

      [DllImport( "user32.dll", SetLastError = true )]
      public static extern IntPtr SetParent( IntPtr hWndChild, IntPtr hWndNewParent );

      [DllImport( "user32.dll" )]
      public static extern bool SetWindowPos( IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags );

      [DllImport( "user32.dll" )]
      public static extern int ShowWindow( IntPtr hwnd, int nCmdShow );

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

      private Dictionary<uint, ActorData> m_ActorData;

      public SDLThreadObject( IntPtr windowHandle, IntPtr tabHandle, int width, int height ) 
         {
         m_WindowHandle = windowHandle;
         m_TabHandle = tabHandle;
         m_Width = width;
         m_Height = height;
         m_ShouldLoop = true;
         m_ActorData = new Dictionary<uint, ActorData>();
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
            SetWindowPos(
               m_pSDLWindowHandle,
               m_WindowHandle,
               0,
               0,
               0,
               0,
               0x0401 // NOSIZE | SHOWWINDOW 
               );
            SetParent( m_pSDLWindowHandle, m_TabHandle );
            ShowWindow( m_pSDLWindowHandle, 1 ); // SHOWNORMAL
            SDL.SDL_EventState( SDL.SDL_EventType.SDL_DROPFILE, SDL.SDL_ENABLE );
            m_SDLEventFilter = new SDL.SDL_EventFilter( this.SDLEventFilter );
            SDL.SDL_SetEventFilter( m_SDLEventFilter, new IntPtr() );
            NativeMethods.EditorMain( m_pSDLWindow, m_Width, m_Height );
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }

      public void StartAndResumeEngine() 
         {
         NativeMethods.StartAndResumeEngine();
         }

      public delegate void myDelegate();
      myDelegate ReInitDelegate = new myDelegate( NativeMethods.StopEngine );

      
      public void StopEngine() 
         {
         NativeMethods.StopEngine();
         foreach( var actorData in m_ActorData )
            {
            if( actorData.Value.IsDirty() )
               {
               ModifyActor( actorData.Key, actorData.Value.data );
               }
            }
         }

      public void ModifyActor( string actorOrerrides )
         {
         ModifyActor( m_SelectedActorId, actorOrerrides );
         }

      public void ModifyActor( uint acotrId, string actorOrerrides ) // It must have been clicked before modified, so just modify its data
         {
         if( acotrId != INVALID_ACTOR_ID )
            {
            NativeMethods.ModifyActor( acotrId, actorOrerrides );
            m_ActorData[ acotrId ].data = actorOrerrides;
            }
         }

      public void PickActor()
         {
         uint selectedActorId = NativeMethods.PickActor();
         if( selectedActorId != INVALID_ACTOR_ID )
            {
            m_SelectedActorId = selectedActorId;
            string actorXml = GetActorXml( m_SelectedActorId );

            if( !m_ActorData.ContainsKey( m_SelectedActorId ) ) // never clicked before, must also never be modified
               {
               m_ActorData.Add( m_SelectedActorId, new ActorData( actorXml, m_SelectedActorId, false ) );
               }
            
            Program.s_Editor.BeginInvoke( Program.s_Editor.m_SetActorDataStringDelegate, actorXml );
            }
         }

     // myDelegate SaveDelegate = new myDelegate( NativeMethods.SaveAllActors );

      public void SaveAll() 
         {
         foreach( var actorDataPair in m_ActorData )
            {
            actorDataPair.Value.SaveToFile();
            }
         NativeMethods.SaveWorld();
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
            case SDL.SDL_EventType.SDL_DROPFILE:
               string filePath = Marshal.PtrToStringAnsi( eventInstance.drop.file );
               OnFileDrop( filePath );
               return 0;
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

      private string GetActorXml( uint actorId ) 
         {
         if( actorId != INVALID_ACTOR_ID )
            {
            uint xmlSize = NativeMethods.GetActorXmlSize( actorId );
            if( xmlSize == 0 )
               {
               return "";
               }
            IntPtr tempArray = Marshal.AllocCoTaskMem( ( (int) xmlSize + 1 ) * sizeof( char ) );
            NativeMethods.GetActorXML( tempArray, actorId );
            string actorXml = Marshal.PtrToStringAnsi( tempArray );
            Marshal.FreeCoTaskMem( tempArray );
            return actorXml;
            }
         return "";
         }

      private void OnFileDrop( string filePath ) 
         {
         XmlDocument doc = new XmlDocument();
         doc.Load( Program.s_Editor.m_AssetsDirectory + filePath );
         string rootNodeName = doc.FirstChild.Name;
         if( rootNodeName.Equals( "ActorClass" ) )
            {
            uint newActorId = NativeMethods.CreateActor( filePath );
            if( newActorId != INVALID_ACTOR_ID )
               {
               m_ActorData.Add( newActorId, new ActorData( GetActorXml( newActorId ), newActorId, true ) );
               }
            }
         else if( rootNodeName.Equals( "World" ) )
            {
            }

         }
      }

   }
