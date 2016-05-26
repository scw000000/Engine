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
      public unsafe static extern int test( IntPtr pSDLWindow );

      [DllImport( "user32.dll", SetLastError = true )]
      public static extern IntPtr SetParent( IntPtr hWndChild, IntPtr hWndNewParent );
      }
   }
