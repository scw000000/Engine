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

      public void Application_Closing( Object sender, FormClosingEventArgs e )
         {
         Program.s_SDLThreadObject.RequestStop();
         }
      }
   }
