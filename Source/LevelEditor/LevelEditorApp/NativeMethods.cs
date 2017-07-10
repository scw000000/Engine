using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace LevelEditorApp
   {
   static class NativeMethods
      {
#if DEBUG  
    const string editorDllName = "EditorDLLWin32Debug.dll";
#else
    const string editorDllName = "EditorDLLWin32Release.dll";
#endif

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern int EditorMain( IntPtr pSDLWindow, int screenWidth, int screenHeight );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void SingleLoop();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void PushSDLEvent( IntPtr evt );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void Shutdown();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void LoadLevel( [MarshalAs( UnmanagedType.BStr )] string levelPath );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern uint PickActor();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern uint GetActorXmlSize( uint actorId );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void GetActorXML( IntPtr actorXMLPtrAddress, uint actorId );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void ModifyActor( uint actorId, [ MarshalAs( UnmanagedType.BStr )] string lactorModXML );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void StartAndResumeEngine();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void StopEngine();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void SaveAllActors();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void SaveActor( uint actorId );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern uint CreateActor( [MarshalAs( UnmanagedType.BStr )] string lactorModXML );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void SaveWorld( );

      [DllImport( "coredll.dll" )]
      public static extern bool SetForegroundWindow( IntPtr hWnd );
      }
   }
