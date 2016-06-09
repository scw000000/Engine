using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace LevelEditorApp
   {
   static class Program
      {
      public static SDLThreadObject s_SDLThreadObject;
      public static Editor s_Editor;
      public static Thread s_SDLThread;
      /// <summary>
      /// The main entry point for the application.
      /// </summary>
      [STAThread]
      static void Main()
         {
         Application.EnableVisualStyles();
         Application.SetCompatibleTextRenderingDefault( false );
         s_Editor = new Editor();
         s_SDLThreadObject = new SDLThreadObject();
         s_SDLThread = new Thread( s_SDLThreadObject.Run );
         s_SDLThread.Start();
         
         ApplicationEventHandler eventHandler = new ApplicationEventHandler();
         s_Editor.FormClosing += new FormClosingEventHandler( eventHandler.Application_Closing );
         Application.Run( s_Editor );
         Program.s_SDLThread.Join();
         Program.s_Editor.ShutDownSDLWindow();
         }
      }
   }
