using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using SDL2;

namespace LevelEditorApp
   {
   class ApplicationEventHandler
      {
      System.IO.StreamReader m_Reader = null;
      public ApplicationEventHandler() 
         {
        // m_Reader = new System.IO.StringReader();
         //m_Reader = new System.IO.StreamReader( Program.s_Editor.m_ServerPipe );
         }
      public void Application_Closing( Object sender, FormClosingEventArgs e )
         {
         Program.s_SDLThreadObject.RequestStop();
         }

      

      public void Application_Idle( object sender, EventArgs evnt )
         {
         if( m_Reader == null )
            {
            return;
            }
         try
            {
            if( Program.s_Editor.m_ServerPipe.IsConnected )
               {
               //System.Threading.Tasks.Task< string >txt = m_Reader.ReadLineAsync();
             //  Console.WriteLine( txt. );
               }
            
            }
         catch( Exception e )
            {
            MessageBox.Show( e.ToString(), "Error", MessageBoxButtons.OK );
            }
         }
      }
   }
