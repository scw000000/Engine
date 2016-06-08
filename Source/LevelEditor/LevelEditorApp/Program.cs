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
      public static FormThread m_FormThread;
      /// <summary>
      /// The main entry point for the application.
      /// </summary>
      [STAThread]
      static void Main()
         {
         Application.EnableVisualStyles();
         Application.SetCompatibleTextRenderingDefault( false );
         //Editor editor = new Editor();
         m_FormThread = new FormThread();
         Thread workerThread = new Thread( m_FormThread.Init );
         workerThread.Start();
         workerThread.Join();

         MessageHandler messageHandler = new MessageHandler( ref  m_FormThread.m_Editor.tabPageEX_World, ref m_FormThread.m_Editor );
         //Application.AddMessageFilter( messageHandler );

         workerThread = new Thread( m_FormThread.Run );

         workerThread.Start();

         //Application.Idle += new EventHandler( messageHandler.Application_Idle );
         Application.Run( m_FormThread.m_Editor );





         
         }
      }
   public class FormThread
      {
      public Editor m_Editor;
      public FormThread() 
         {    
         //MessageHandler messageHandler = new MessageHandler( ref m_Editor.tabPageEX_World, ref m_Editor );
         //Application.AddMessageFilter( messageHandler );
         //Application.Idle += new EventHandler( messageHandler.Application_Idle );
         }

      public void Init() 
         {
         m_Editor = new Editor();
         }

      public void Run() 
         {
         Application.Run( m_Editor );
         while( true )
            {
            NativeMethods.SingleLoop();
            }
         }
      }
   }
