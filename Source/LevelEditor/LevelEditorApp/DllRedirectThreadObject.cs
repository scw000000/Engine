using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Pipes;

namespace LevelEditorApp
   {

   class DllRedirectThreadObject
      {
      StreamReader m_Reader = null;
      NamedPipeServerStream m_ServerPipe = null;
      private volatile bool m_ShouldLoop;
      public void SetReader( NamedPipeServerStream serverPipe )
         {
         m_ServerPipe = serverPipe;
         m_Reader = new System.IO.StreamReader( m_ServerPipe );
         m_ShouldLoop = true;
         }

      public void RequestStop() 
         {
         m_ShouldLoop = false;
         }

      public void Run()
         {
         while( m_ShouldLoop && m_ServerPipe.IsConnected )
            {
            try
               {
               string txt = m_Reader.ReadLine();
               if( !string.IsNullOrEmpty( txt ) )
                  {
                  var task = Task.Factory.StartNew( () =>
                     {

                     Program.s_Editor.BeginInvoke( Program.s_Editor.m_RedirectStringDelegate, txt );
                     } );

                  }

               }
            catch( Exception e )
               {
               System.Windows.Forms.MessageBox.Show( e.ToString() );
               break; // normal disconnect
               }
            
            }
         return;
         }

      }
   }
