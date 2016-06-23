using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LevelEditorApp
   {

   class DllRedirectThreadObject
      {
      System.IO.StreamReader m_Reader = null;
      System.IO.Pipes.NamedPipeServerStream m_ServerPipe = null;
      public void SetReader( System.IO.Pipes.NamedPipeServerStream serverPipe )
         {
         m_ServerPipe = serverPipe;
         m_Reader = new System.IO.StreamReader( serverPipe );
         }

      public void Run()
         {
         while( m_ServerPipe.IsConnected )
            {
            try
               {
               string txt = m_Reader.ReadLine();
               if( !string.IsNullOrEmpty( txt ) )
                  {
                  Program.s_Editor.Invoke( Program.s_Editor.m_RedirectStringDelegate, txt );
                  }
               
               }
            catch(  Exception e )
               {
               System.Windows.Forms.MessageBox.Show( e.ToString() );
               break; // normal disconnect
               }
            }
         }

      }
   }
