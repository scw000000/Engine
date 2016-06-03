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
      //private ActorComponentEditor m_ActorComponentEditor;
      private IntPtr m_pSDLWindow;
      private List<XmlNode> m_ActorsXmlNodes = new List<XmlNode>();
      private MessageHandler m_messageFilter;
      private System.Windows.Forms.TreeView treeView_Assets;

      private const int INVALID_ACTOR_ID = 0;
      private const int m_MaxTabSize = 100;
      private const int m_MinTabSize = 50;

      private int m_SelectedActorId = INVALID_ACTOR_ID;

      private string m_AssetsDirectory;
      private string m_CurrentDirectory;
      private string m_ProjectDirectory;

      public Editor()
         {
         InitializeComponent();
         m_CurrentDirectory = Directory.GetCurrentDirectory();
         m_AssetsDirectory = m_CurrentDirectory + "\\Assets";

         // Setting all of splitter width in all comtainers
         int splitterWidth = 10;
         this.splitContainer1.SplitterWidth = splitterWidth;
         this.splitContainer2.SplitterWidth = splitterWidth;
         this.splitContainer_Left.SplitterWidth = splitterWidth;
         this.splitContainer_Mid.SplitterWidth = splitterWidth;
         this.splitContainer_Right.SplitterWidth = splitterWidth;

         this.tabPageEX_World = AddTabePage( "tabPageEX_World", this.tabCtlEX_MidUp.Controls, false );

         this.tabPageEX_Assets = AddTabePage( "tabPageEX_Assets", this.tabCtlEX_RightUp.Controls, false );
         this.treeView_Assets = new System.Windows.Forms.TreeView();
         this.treeView_Assets.Dock = DockStyle.Fill;
         this.treeView_Assets.BackColor = Color.FromArgb( 255, 70, 70, 70 );
         this.treeView_Assets.LineColor = Color.WhiteSmoke;
         this.treeView_Assets.NodeMouseDoubleClick += new TreeNodeMouseClickEventHandler( this.treeView_Assets_NodeMouseClick );
         this.tabPageEX_Assets.Controls.Add( this.treeView_Assets );

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
         node.ForeColor = Color.WhiteSmoke;
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
                  childDirectoryNode.ForeColor = Color.WhiteSmoke;
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
                  childNode.ForeColor = Color.WhiteSmoke;
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

      private System.Windows.Forms.TextBox AddTextBox( string name, string fileName, System.Windows.Forms.Control.ControlCollection controls, bool closeBtnEnabled )
         {
         System.Windows.Forms.TextBox textBox = new System.Windows.Forms.TextBox();
         textBox.Dock = DockStyle.Fill;
         textBox.Name = name;
         textBox.BackColor = Color.FromArgb( 255, 49, 49, 49 );
         textBox.ForeColor = Color.WhiteSmoke;
         textBox.Multiline = true;
         textBox.ScrollBars = ScrollBars.Vertical;
         try
            {
            StreamReader objstream = new StreamReader( fileName );

            textBox.Text = objstream.ReadToEnd ();
            }
         catch( Exception e )
            {
            MessageBox.Show( "Err: " + e.ToString() );
            }
         controls.Add( textBox );
         return textBox;
         }

      private void tabCtlEX_MidUp_OnClose( object sender, CloseEventArgs e )
         {
         this.tabCtlEX_MidUp.Controls.Remove( this.tabCtlEX_MidUp.TabPages[ e.TabIndex ] );
         }

      void treeView_Assets_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
         {
         LevelEditorApp.TabPageEX textPage = AddTabePage( e.Node.Text, this.tabCtlEX_MidUp.Controls, true );
         String filePath = "";
         TreeNode tempNode = e.Node;
         while( tempNode != null )
            {
            filePath = "\\" + tempNode.Text + filePath;
            tempNode = tempNode.Parent;
            }

         System.Windows.Forms.TextBox textBox = AddTextBox( e.Node.Text, m_CurrentDirectory + "\\" + filePath, textPage.Controls, false );
         }
      }
   }
