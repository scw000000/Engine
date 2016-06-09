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
      public static SDLThread s_SDLThread;
      public static Editor s_Editor;
      /// <summary>
      /// The main entry point for the application.
      /// </summary>
      [STAThread]
      static void Main()
         {
         Application.EnableVisualStyles();
         Application.SetCompatibleTextRenderingDefault( false );
         s_Editor = new Editor();
         s_SDLThread = new SDLThread();
         Thread workerThread = new Thread( s_SDLThread.Run );
         workerThread.Start();
         Application.Run( s_Editor );
         }
      }
   }
