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
using System.Runtime.InteropServices;
using System;

using SDL2;

namespace LevelEditorApp
   {
   public partial class Editor : Form
      {
      //private ActorComponentEditor m_ActorComponentEditor;
      public IntPtr m_pSDLWindow;
      public IntPtr m_pSDLWindowHandle;
      public delegate void myDelegate();
      public myDelegate m_UpdateSDLDelegate;
      public delegate void strDelegate( String str );
      public strDelegate m_RedirectStringDelegate;

      private List<XmlNode> m_ActorsXmlNodes = new List<XmlNode>();

      private const uint INVALID_ACTOR_ID = 0;

      private const int m_MaxTabSize = 100;
      private const int m_MinTabSize = 50;

      private uint m_SelectedActorId = INVALID_ACTOR_ID;

      private string m_AssetsDirectory;
      private string m_CurrentDirectory;
      private string m_ProjectDirectory;

      private List<String> m_SupportEditorPattern;

      private SDL.SDL_EventFilter m_SDLEventFilter;

      public Editor()
         {
         InitializeComponent();
         this.WindowState = FormWindowState.Maximized;
         m_CurrentDirectory = Directory.GetCurrentDirectory();
         m_AssetsDirectory = m_CurrentDirectory + "\\Assets";

         m_SupportEditorPattern = new List<string>(){ "*.xml", 
                                                "*.fragmentshader", 
                                                "*.vertexshader",
                                                "*.lua"};
         for ( int i = 0; i < m_SupportEditorPattern.Count; i++)
            {
            m_SupportEditorPattern[i] = WildCardToRegular( m_SupportEditorPattern[i] );
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

         InitializeAssetTree();

         m_UpdateSDLDelegate = new myDelegate( UpdateSDLWindow );
         m_RedirectStringDelegate = new strDelegate( RedirectString );

         //InitSDLWindow();

         }

      public void treeView_Assets_NodeMouseClick( object sender, TreeNodeMouseClickEventArgs e )
         {
         string filePath = "";
         TreeNode tempNode = e.Node;
         while( tempNode != null )
            {
            filePath = "\\" + tempNode.Text + filePath;
            tempNode = tempNode.Parent;
            }
         string fileFullPath = m_CurrentDirectory + "\\" + filePath;
         if( isTextBoxSupport( fileFullPath ) )
            {
            LevelEditorApp.TabPageEX textPage = this.tabCtlEX_MidUp.AddTabPageEx( e.Node.Text, e.Node.Text, true );

            LevelEditorApp.EditorTextBox textBox = new EditorTextBox( "editorTextBox", fileFullPath );
            textBox.Font = new Font( textBox.Font.FontFamily, 18 );
            textPage.Controls.Add( textBox );
            }
         else
            {
            if( File.Exists( fileFullPath ) )
               {
               System.Diagnostics.Process.Start( fileFullPath );
               }
            else 
               {
               MessageBox.Show( "Unknown file", "Error", MessageBoxButtons.OK );
               }
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
            m_SDLEventFilter = new SDL.SDL_EventFilter( this.SDLEventFilter );
            SDL.SDL_AddEventWatch( m_SDLEventFilter, new IntPtr() );
            NativeMethods.EditorMain( m_pSDLWindow, this.splitContainer_Mid.Panel1.Width, this.splitContainer_Mid.Panel1.Height );
            }
         catch( Exception e )
            {
            MessageBox.Show( "Error: " + e.ToString() );
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
            this.xmlControl_ActorXML.DataString = actorXml;    
            }
         }

      public void ModifyActor( string actorOrerrides ) 
         {
         if( m_SelectedActorId != INVALID_ACTOR_ID )
            {
            NativeMethods.ModifyActor( m_SelectedActorId, actorOrerrides );
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

      private LevelEditorApp.TabPageEX AddTabePage( string name, string tabText,System.Windows.Forms.Control.ControlCollection controls, bool closeBtnEnabled ) 
         {
         LevelEditorApp.TabPageEX pageEx = new LevelEditorApp.TabPageEX();
         pageEx.Menu = null;
         pageEx.CloseBtnEnabled = closeBtnEnabled;
         pageEx.Name = name;
         pageEx.Text = tabText;
         controls.Add( pageEx );
         return pageEx;
         }

      private bool isTextBoxSupport( string fileName ) 
         {
         for( int i = 0; i < m_SupportEditorPattern.Count; ++i )
            {
            if( Regex.IsMatch( fileName, m_SupportEditorPattern[ i ] ) )
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

      public void RedirectString( String str ) 
         {
         Console.WriteLine( str );
         }

      [System.Runtime.InteropServices.DllImport( "Kernel32.dll", SetLastError = true )]
      public static extern int SetStdHandle( int device, IntPtr handle );

      private System.IO.Pipes.NamedPipeServerStream m_ServerPipe;
      private System.IO.Pipes.NamedPipeClientStream m_ClientPipe;
      private void Editor_Load( object sender, EventArgs e )
         {
         TextBoxWritter textBoxWritter = new TextBoxWritter( this.textBox_Console );
         Console.SetOut( textBoxWritter );

         Console.WriteLine( "Initializing output redirecting" );
         int id = System.Diagnostics.Process.GetCurrentProcess().Id; // make this instance unique
         m_ServerPipe = new System.IO.Pipes.NamedPipeServerStream( "consoleRedirect" + id, System.IO.Pipes.PipeDirection.In, 1 );
         m_ClientPipe = new System.IO.Pipes.NamedPipeClientStream( ".", "consoleRedirect" + id, System.IO.Pipes.PipeDirection.Out, System.IO.Pipes.PipeOptions.WriteThrough );
         m_ClientPipe.Connect();
         System.Runtime.InteropServices.HandleRef hr11 = new System.Runtime.InteropServices.HandleRef( m_ClientPipe, m_ClientPipe.SafePipeHandle.DangerousGetHandle() );
         SetStdHandle( -11, hr11.Handle ); // redirect stdout to my pipe
         SetStdHandle( -12, hr11.Handle ); // redirect error to my pipe
         Console.WriteLine( "Waiting for console connection" );
         m_ServerPipe.WaitForConnection(); //blocking
         Console.WriteLine( "Console connection complete." );

         Program.s_DllRedirectThreadObject.SetReader( m_ServerPipe );
         Program.s_DllRedirectThread.Start();

         InitSDLWindow();

         Program.s_SDLThreadObject = new SDLThreadObject();
         Program.s_SDLThread = new Thread( Program.s_SDLThreadObject.Run );
         Program.s_SDLThread.Start();
         
         }

      private void Editor_FormClosing( object sender, FormClosingEventArgs e )
         {
         m_ClientPipe.Dispose();
         m_ServerPipe.Disconnect();
         
         Program.s_SDLThreadObject.RequestStop();
         Program.s_DllRedirectThreadObject.RequestStop();
         }

      }
   }
