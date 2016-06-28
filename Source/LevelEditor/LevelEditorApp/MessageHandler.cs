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
      public ApplicationEventHandler() 
         {
         }
      public void Application_Closing( Object sender, FormClosingEventArgs e )
         {
         }

      public void Application_Idle( object sender, EventArgs evnt )
         {
         try
            {
            
            }
         catch( Exception e )
            {
            MessageBox.Show( e.ToString(), "Error", MessageBoxButtons.OK );
            }
         }
      }
   }
