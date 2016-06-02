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
      private LevelEditorApp.TabPageEX tabPageEX_World;
      private LevelEditorApp.TabPageEX tabPageEX_Assets;

      private System.Windows.Forms.TreeView treeView_Assets;
      public Editor()
         {
         InitializeComponent();
         // Setting all of splitter width in all comtainers
         int splitterWidth = 10;
         this.splitContainer1.SplitterWidth = splitterWidth;
         this.splitContainer2.SplitterWidth = splitterWidth;
         this.splitContainer_Left.SplitterWidth = splitterWidth;
         this.splitContainer_Mid.SplitterWidth = splitterWidth;
         this.splitContainer_Right.SplitterWidth = splitterWidth;

         this.tabPageEX_World = this.AddTabePage( "tabPageEX_World", this.tabCtlEX_MidUp.Controls, true );
         this.tabPageEX_Assets = this.AddTabePage( "tabPageEX_Assets", this.tabCtlEX_RightUp.Controls, false );
         this.treeView_Assets = new System.Windows.Forms.TreeView();
         this.treeView_Assets.Dock = DockStyle.Fill;
         this.treeView_Assets.BackColor = Color.FromArgb( 255, 70, 70, 70 );
         this.treeView_Assets.LineColor = Color.WhiteSmoke;
         this.treeView_Assets.NodeMouseDoubleClick += new TreeNodeMouseClickEventHandler( this.treeView_Assets_NodeMouseClick );
         this.tabPageEX_Assets.Controls.Add( this.treeView_Assets );

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
            NativeMethods.SetParent( winHandle, this.tabPageEX_World.Handle );
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
         treeView_Assets.Nodes.Clear();

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

         treeView_Assets.Nodes.Add( node );
         }

      private LevelEditorApp.TabPageEX AddTabePage( string name, System.Windows.Forms.Control.ControlCollection controls, bool closeBtnEnabled ) 
         {
         LevelEditorApp.TabPageEX pageEx = new LevelEditorApp.TabPageEX();
         pageEx.Menu = null;
         pageEx.CloseBtnEnabled = closeBtnEnabled;
         pageEx.Name = name;
         pageEx.Text = name;
         controls.Add( pageEx );
         return pageEx;
         }

      private System.Windows.Forms.TextBox AddTextBox( string name, System.Windows.Forms.Control.ControlCollection controls, bool closeBtnEnabled )
         {
         System.Windows.Forms.TextBox textBox = new System.Windows.Forms.TextBox();
         textBox.Dock = DockStyle.Fill;
         textBox.Name = name;
         textBox.Text = "fucccccccccccccccccccccccccccccccccc";
         controls.Add( textBox );
         return textBox;
         }

      private void tabControlEX1_OnClose( object sender, CloseEventArgs e )
         {
         this.tabCtlEX_LeftUp.Controls.Remove( this.tabCtlEX_LeftUp.TabPages[ e.TabIndex ] );
         }

      private IntPtr m_pSDLWindow;

      void treeView_Assets_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
         {
         LevelEditorApp.TabPageEX textPage = AddTabePage( e.Node.Text, this.tabCtlEX_MidUp.Controls, true );
         System.Windows.Forms.TextBox textBox = AddTextBox( e.Node.Text, textPage.Controls, false );
         }
      }
   }
