using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LevelEditorApp
   {
   static class Program
      {
      /// <summary>
      /// The main entry point for the application.
      /// </summary>
      [STAThread]
      static void Main()
         {
         
         Application.EnableVisualStyles();
         Application.SetCompatibleTextRenderingDefault( false );
         Editor editor = new Editor();
         MessageHandler messageHandler = new MessageHandler( ref editor.tabPageEX_World, ref editor );
         Application.AddMessageFilter( messageHandler );
         Application.Idle += new EventHandler( messageHandler.Application_Idle );
         Application.Run( editor );

         }
      }
   }
