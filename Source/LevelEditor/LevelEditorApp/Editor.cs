using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System;

using SDL2;

namespace LevelEditorApp
   {
   public partial class Editor : Form
      {
      //private ActorComponentEditor m_ActorComponentEditor;
      public IntPtr m_pSDLWindow;
      public IntPtr m_pSDLWindowHandle;
      public delegate void myDalegate();
      public myDalegate m_UpdateSDLDelegate;

      private List<XmlNode> m_ActorsXmlNodes = new List<XmlNode>();
      private System.Windows.Forms.TreeView treeView_Assets;

      private const int INVALID_ACTOR_ID = 0;
      private const int m_MaxTabSize = 100;
      private const int m_MinTabSize = 50;

      private int m_SelectedActorId = INVALID_ACTOR_ID;

      private string m_AssetsDirectory;
      private string m_CurrentDirectory;
      private string m_ProjectDirectory;

      private List<String> m_SupportTextPattern;

      public Editor()
         {
         InitializeComponent();
         this.WindowState = FormWindowState.Maximized;
         m_CurrentDirectory = Directory.GetCurrentDirectory();
         m_AssetsDirectory = m_CurrentDirectory + "\\Assets";

         m_SupportTextPattern = new List<string>(){ "*.xml", 
                                                "*.fragmentshader", 
                                                "*.vertexshader",
                                                "*.lua"};
         for ( int i = 0; i < m_SupportTextPattern.Count; i++)
            {
            m_SupportTextPattern[i] = WildCardToRegular( m_SupportTextPattern[i] );
            }

         // Setting all of splitter width in all comtainers
         int splitterWidth = 10;
         this.splitContainer1.SplitterWidth = splitterWidth;
         this.splitContainer2.SplitterWidth = splitterWidth;
         this.splitContainer_Left.SplitterWidth = splitterWidth;
         this.splitContainer_Mid.SplitterWidth = splitterWidth;
         this.splitContainer_Right.SplitterWidth = splitterWidth;

         int SDLWindowWidth = 1366;
         int SDLWindowHeight = 768;
         int lrPageWidth = ( this.splitContainer1.Width - SDLWindowWidth - 2 * splitterWidth - 8 ) / 2;
         this.splitContainer1.SplitterDistance = this.splitContainer1.Width - lrPageWidth - splitterWidth - 4;
         this.splitContainer2.SplitterDistance = this.splitContainer2.Width - SDLWindowWidth - splitterWidth;
         this.splitContainer_Mid.SplitterDistance = SDLWindowHeight;

         this.tabPageEX_World = AddTabePage( "tabPageEX_World", this.tabCtlEX_MidUp.Controls, false );
         tabPageEX_World.AllowDrop = true;
         tabPageEX_World.DragEnter += new DragEventHandler( tabPageEX_World.tabPageEX_DragEnter );

         this.tabPageEX_Assets = AddTabePage( "tabPageEX_Assets", this.tabCtlEX_LeftDown.Controls, false );
         this.treeView_Assets = new System.Windows.Forms.TreeView();
         this.treeView_Assets.Dock = DockStyle.Fill;
         this.treeView_Assets.BackColor = Color.FromArgb( 255, 70, 70, 70 );
         this.treeView_Assets.LineColor = Color.WhiteSmoke;
         this.treeView_Assets.NodeMouseDoubleClick += new TreeNodeMouseClickEventHandler( this.treeView_Assets_NodeMouseClick );
         this.treeView_Assets.ItemDrag += new ItemDragEventHandler( this.treeView_Assets_ItemDrag );
         this.tabPageEX_Assets.Controls.Add( this.treeView_Assets );
         InitializeAssetTree();

         this.tabPageEX_ActorXML = AddTabePage( "tabPageEX_ActorXML", this.tabCtlEX_RightDown.Controls, false );
         this.tabPageEX_ActorXML.Controls.Add( this.xmlControl_ActorXML );

         m_UpdateSDLDelegate = new myDalegate( UpdateSDLWindow );
         InitSDLWindow();

         }

      public void treeView_Assets_NodeMouseClick( object sender, TreeNodeMouseClickEventArgs e )
         {
         if( isTextBoxSupport( e.Node.Text ) )
            {
            LevelEditorApp.TabPageEX textPage = AddTabePage( e.Node.Text, this.tabCtlEX_MidUp.Controls, true );
            String filePath = "";
            TreeNode tempNode = e.Node;
            while( tempNode != null )
               {
               filePath = "\\" + tempNode.Text + filePath;
               tempNode = tempNode.Parent;
               }
            textPage.TextBox = new EditorTextBox( e.Node.Text, m_CurrentDirectory + "\\" + filePath );
            textPage.TextBox.Font = new Font( textPage.TextBox.Font.FontFamily, 18 );
            textPage.Controls.Add( textPage.TextBox );
            }
         else
            {
            MessageBox.Show( "This file format is not supported yet.", "Error", MessageBoxButtons.OK );
            }
         }

      public void treeView_Assets_ItemDrag( object sender, ItemDragEventArgs e ) 
         {
         String filePath = "";
         TreeNode tempNode = (TreeNode) e.Item;
         while( tempNode != treeView_Assets.Nodes[0] )
            {
            filePath = "\\" + tempNode.Text + filePath;
            tempNode = tempNode.Parent;
            }
         String[] filesToDrag = { filePath.Substring( 1 ) };
         DoDragDrop( new DataObject( DataFormats.FileDrop, filesToDrag ), DragDropEffects.Copy );
         }

      public void InitSDLWindow()
         {
         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_EVERYTHING );
            //IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );
            m_pSDLWindow = SDL.SDL_CreateWindow( string.Empty,
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED,
               this.splitContainer_Mid.Panel1.Width,
               this.splitContainer_Mid.Panel1.Height,
               SDL.SDL_WindowFlags.SDL_WINDOW_BORDERLESS | SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL );
            //IntPtr pContext = SDL2.SDL.SDL_GL_CreateContext( m_pSDLWindow );
            SDL.SDL_SysWMinfo info = new SDL.SDL_SysWMinfo();
            SDL.SDL_GetWindowWMInfo( m_pSDLWindow, ref info );
            m_pSDLWindowHandle = info.info.win.window;
            NativeMethods.SetWindowPos(
               m_pSDLWindowHandle,
               Handle,
               0,
               0,
               0,
               0,
               0x0401 // NOSIZE | SHOWWINDOW 
               );
            NativeMethods.SetParent( m_pSDLWindowHandle, this.tabPageEX_World.Handle );
            NativeMethods.ShowWindow( m_pSDLWindowHandle, 1 ); // SHOWNORMAL
            NativeMethods.EditorMain( m_pSDLWindow, this.splitContainer_Mid.Panel1.Width, this.splitContainer_Mid.Panel1.Height );
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
            }
         }

      private void UpdateSDLWindow()
         {
         NativeMethods.SingleLoop();
         }

      public void ShutDownSDLWindow()
         {
         NativeMethods.Shutdown();
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

      private bool isTextBoxSupport( string fileName ) 
         {
         for( int i = 0; i < m_SupportTextPattern.Count; ++i )
            {
            if( Regex.IsMatch( fileName, m_SupportTextPattern[ i ] ) )
               {
               return true;
               }
            }
         return false;
         }

      private static String WildCardToRegular( String value )
         {
         return "^" + Regex.Escape( value ).Replace( "\\?", "." ).Replace( "\\*", ".*" ) + "$";
         }
      }
   }
