using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SDL2;

namespace LevelEditorApp
   {
   public partial class Editor : Form
      {
      public Editor()
         {
         InitializeComponent();
         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_VIDEO );
            //IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );
            m_pWindow = SDL.SDL_CreateWindow( string.Empty, 
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED, 
               this.splitContainer2.Panel1.Width, 
               this.splitContainer2.Panel1.Height, 
               SDL.SDL_WindowFlags.SDL_WINDOW_BORDERLESS | SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL );   
            IntPtr pContext = SDL2.SDL.SDL_GL_CreateContext( m_pWindow );
            SDL.SDL_SysWMinfo info = new SDL.SDL_SysWMinfo();
            SDL.SDL_GetWindowWMInfo( m_pWindow, ref info );
            IntPtr winHandle = info.info.win.window;
            NativeMethods.SetWindowPos(
         winHandle,
         Handle,
         0,
         0,
         0,
         0,
         0x0401 // NOSIZE | SHOWWINDOW
         );
            //string errStr = SDL2.SDL.SDL_GetError();
            //CreateParams para = new CreateParams();
            //para.Width = this.splitContainer1.Panel1.Width;
            //para.Height = this.splitContainer1.Panel1.Height;
            //para.X = 0;
            //para.Y = 0;
            //para.Parent = this.splitContainer1.Panel1.Handle;
            //para.Style = 0x40000000 | 0x10000000 | 0x04000000 | 0x02000000;
         //   IntPtr hwnd = this.CreateHandle( para );
               ///WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

            //Panel m_SdlPanel = new Panel();
            //m_SdlPanel.Size = new Size( 512, 512 );
            //m_SdlPanel.Location = new Point( 0, 0 );
            //Controls.Add( m_SdlPanel );
            NativeMethods.SetParent( winHandle, this.splitContainer2.Panel1.Handle );
           NativeMethods.ShowWindow( winHandle, 1 ); // SHOWNORMAL
         if( pContext.Equals( 0 ) )
            {
         
            }
         NativeMethods.EditorMain( m_pWindow, this.splitContainer2.Panel1.Width, this.splitContainer2.Panel1.Height );
         //   NativeMethods.test( pWindow );
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }
      private IntPtr m_pWindow;
      }
   }
