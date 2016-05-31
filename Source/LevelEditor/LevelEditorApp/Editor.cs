using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


using System.IO;
//using System.Collections;
//using System.Runtime.InteropServices;
using System.Xml;

using SDL2;

namespace LevelEditorApp
   {
   public partial class Editor : Form
      {
      private const int INVALID_ACTOR_ID = 0;

      private const int m_MinTabSize = 50;
      private const int m_MaxTabSize = 100;

      private string m_ProjectDirectory;
      private string m_AssetsDirectory;
      private string m_CurrentLevelFile;

      private List<XmlNode> m_ActorsXmlNodes = new List<XmlNode>();
      private int m_SelectedActorId = INVALID_ACTOR_ID;

      private MessageHandler m_messageFilter;
      //private ActorComponentEditor m_ActorComponentEditor;
      private LevelEditorApp.TabPageEX tabPageEX1;
      public Editor()
         {
      

         InitializeComponent();
         int splitterWidth = 10;
         this.splitContainer1.SplitterWidth = splitterWidth;
         this.splitContainer2.SplitterWidth = splitterWidth;
         this.splitContainer_Left.SplitterWidth = splitterWidth;
         this.splitContainer_Mid.SplitterWidth = splitterWidth;
         this.splitContainer_Right.SplitterWidth = splitterWidth;
         
         
         this.tabControl1.DrawItem += new DrawItemEventHandler( TabPage_DrawItem );
         Graphics g = Graphics.FromHwnd( this.tabPage1.Handle );
         SizeF size = g.MeasureString( this.tabControl1.TabPages[ 0 ].Text + "            X", this.tabControl1.TabPages[ 0 ].Font, 100 );
         this.tabControl1.ItemSize = new Size( Math.Min( m_MaxTabSize, Math.Max( m_MinTabSize, ( tabControl1.Width - 30 ) / tabControl1.TabCount ) ), 0 );

         
         this.tabPageEX1 = new LevelEditorApp.TabPageEX( this.components );
         this.tabPageEX1.Location = new System.Drawing.Point( 4, 22 );
         this.tabPageEX1.Menu = null;
         this.tabPageEX1.Name = "tabPageEX1";
         this.tabPageEX1.Size = new System.Drawing.Size( 189, 144 );
         this.tabPageEX1.TabIndex = 1;
         this.tabPageEX1.Text = "tabPageEX1";
         this.tabControlEX1.Controls.Add( this.tabPageEX1 );
      // this.tabControlEX1.ItemSize = new Size( Math.Min( m_MaxTabSize, Math.Max( m_MinTabSize, ( tabControlEX1.Width - 30 ) / tabControlEX1.TabCount ) ), 0 );  
    
      // this.tabControl1.TabPages[ 0 ].Width = ( int ) Math.Ceiling( size.Width );
        　//　 tabPage1.Width = 5000;
      //TextRenderer.MeasureText( this.tabControl1.TabPages[ 0 ].Text + " X", this.tabControl1.TabPages[ 0 ].Font, 50 );
         //Sizef size = Graphics.MeasureString( this.tabControl1.TabPages[ 0 ].Text + " X", this.tabControl1.TabPages[0].Font, 50 );
         //this.tabControl1.TabPages[0].Width = 
       //  tabControl1.ItemSize = new Size( tabControl1.Width / tabControl1.TabCount, 0 );

       //  tabControl1.ItemSize = new Size( Math.Min( m_MaxTabSize, Math.Max( m_MinTabSize, ( tabControl1.Width - 30 ) / tabControl1.TabCount ) ), 0 );
         var currentDirectory = Directory.GetCurrentDirectory();
         m_AssetsDirectory = currentDirectory + "\\Assets";

         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_VIDEO );
            //IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );
            m_pSDLWindow = SDL.SDL_CreateWindow( string.Empty, 
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED,
               this.splitContainer_Mid.Panel1.Width,
               this.splitContainer_Mid.Panel1.Height, 
               SDL.SDL_WindowFlags.SDL_WINDOW_BORDERLESS | SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL );   
            //IntPtr pContext = SDL2.SDL.SDL_GL_CreateContext( m_pSDLWindow );
            SDL.SDL_SysWMinfo info = new SDL.SDL_SysWMinfo();
            SDL.SDL_GetWindowWMInfo( m_pSDLWindow, ref info );
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
            NativeMethods.SetParent( winHandle, this.splitContainer_Mid.Panel1.Handle );
            NativeMethods.ShowWindow( winHandle, 1 ); // SHOWNORMAL
            NativeMethods.EditorMain( m_pSDLWindow, this.splitContainer_Mid.Panel1.Width, this.splitContainer_Mid.Panel1.Height );
         //   NativeMethods.test( pWindow );
            InitializeAssetTree();
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }
      private void InitializeAssetTree()
         {
         TreeView_Assets.Nodes.Clear();

         var stack = new Stack<TreeNode>();
         var rootDirectory = new DirectoryInfo( m_AssetsDirectory );
         var node = new TreeNode( rootDirectory.Name ) { Tag = rootDirectory };
         node.ForeColor = System.Drawing.Color.Silver;
         stack.Push( node );

         while( stack.Count > 0 )
            {
         var currentNode = stack.Pop();
         var directoryInfo = (DirectoryInfo) currentNode.Tag;
         foreach( var directory in directoryInfo.GetDirectories() )
            {
            FileAttributes attributes = File.GetAttributes( directory.FullName );
            if( ( attributes & FileAttributes.Hidden ) == 0 )
               {
               var childDirectoryNode = new TreeNode( directory.Name );
               childDirectoryNode.Tag = directory;
               childDirectoryNode.ForeColor = System.Drawing.Color.Silver;
               currentNode.Nodes.Add( childDirectoryNode );
               stack.Push( childDirectoryNode );
               }
            }
         foreach( var file in directoryInfo.GetFiles() )
            {
            FileAttributes attributes = File.GetAttributes( file.FullName );
            if( ( attributes & FileAttributes.Hidden ) == 0 )
               {
               var childNode = new TreeNode( file.Name );
               childNode.Tag = file.FullName;
               childNode.ForeColor = System.Drawing.Color.Silver;
               currentNode.Nodes.Add( childNode );
               }
            }
         }

      TreeView_Assets.Nodes.Add( node );
         }
      private void TabPage_DrawItem( object sender, System.Windows.Forms.DrawItemEventArgs e ) 
         {
    //  e.Graphics.DrawImage( this.imageList1.Images[ 0 ], e.Bounds.Right - 19, e.Bounds.Top + 3 );
      
      Rectangle closeButton = new Rectangle( e.Bounds.Right - 15, e.Bounds.Top + 4, 9, 7 );
     // e.Graphics.DrawString( "      X", e.Font, Brushes.Black, e.Bounds.Right - 15, e.Bounds.Top + 4 );
            
      e.Graphics.DrawString( this.tabControl1.TabPages[ e.Index ].Text + "   X", e.Font, Brushes.Black, e.Bounds.Left + 3, e.Bounds.Top + 4 );
         e.DrawFocusRectangle();
         }

      private void tabControlEX1_OnClose( object sender, CloseEventArgs e )
         {
         this.tabControlEX1.Controls.Remove( this.tabControlEX1.TabPages[ e.TabIndex ] );
         }

      private IntPtr m_pSDLWindow;
      }
   }
