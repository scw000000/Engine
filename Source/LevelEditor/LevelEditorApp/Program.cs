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
         MessageHandler messageHandler = new MessageHandler();
         Application.AddMessageFilter( messageHandler );
         Editor editor = new Editor();   
         Application.Idle += new EventHandler( messageHandler.Application_Idle );
         
         Application.Run( editor );

         }
      }
   }
