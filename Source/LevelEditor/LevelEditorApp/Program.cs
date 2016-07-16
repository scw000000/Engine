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
      public static Editor s_Editor;
      [STAThread]
      static void Main()
         {
         Application.EnableVisualStyles();
         Application.SetCompatibleTextRenderingDefault( false );

         s_Editor = new Editor();

         Application.Run( s_Editor );  
         }
      }
   }
