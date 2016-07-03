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
      const string editorDllName = "EditorDLLWin32Debug.dll";

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern int EditorMain( IntPtr pSDLWindow, int screenWidth, int screenHeight );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public static extern void SingleLoop();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void PushSDLEvent( IntPtr evt );

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern void Shutdown();

      [DllImport( editorDllName, CallingConvention = CallingConvention.Cdecl )]
      public unsafe static extern int PickActor();

      [DllImport( "user32.dll", SetLastError = true )]
      public static extern IntPtr SetParent( IntPtr hWndChild, IntPtr hWndNewParent );

      [DllImport( "user32.dll" )]
      public static extern bool SetWindowPos( IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags );

      [DllImport( "user32.dll" )]
      public static extern int ShowWindow( IntPtr hwnd, int nCmdShow );

      [DllImport( "coredll.dll" )]
      public static extern bool SetForegroundWindow( IntPtr hWnd );
      }
   }
