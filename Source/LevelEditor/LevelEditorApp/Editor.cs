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
      private string m_ProjectDirectory;
      private string m_AssetsDirectory;
      private string m_CurrentLevelFile;

      private List<XmlNode> m_ActorsXmlNodes = new List<XmlNode>();
      private int m_SelectedActorId = INVALID_ACTOR_ID;

      private MessageHandler m_messageFilter;
      //private ActorComponentEditor m_ActorComponentEditor;

      public Editor()
         {
         InitializeComponent();
         var currentDirectory = Directory.GetCurrentDirectory();
         m_AssetsDirectory = currentDirectory + "\\Assets";

         try
            {
            SDL.SDL_Init( SDL.SDL_INIT_VIDEO );
            //IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );
            m_pSDLWindow = SDL.SDL_CreateWindow( string.Empty, 
               SDL.SDL_WINDOWPOS_CENTERED, SDL.SDL_WINDOWPOS_CENTERED, 
               this.splitContainer2.Panel1.Width, 
               this.splitContainer2.Panel1.Height, 
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
            NativeMethods.SetParent( winHandle, this.splitContainer2.Panel1.Handle );
            NativeMethods.ShowWindow( winHandle, 1 ); // SHOWNORMAL
            NativeMethods.EditorMain( m_pSDLWindow, this.splitContainer2.Panel1.Width, this.splitContainer2.Panel1.Height );
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
               currentNode.Nodes.Add( childNode );
               }
            }
         }

      TreeView_Assets.Nodes.Add( node );
         }
      private IntPtr m_pSDLWindow;
      }
   }
