using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SDL2;

namespace LevelEditorApp
   {
   class SDLThreadObject
      {
      private volatile bool m_ShouldLoop;

      public SDLThreadObject() 
         {
         m_ShouldLoop = true;
         }

      public void Run() 
         {
         while( m_ShouldLoop )
            {
            Program.s_Editor.Invoke( Program.s_Editor.m_UpdateSDLDelegate );
            }
         Console.WriteLine( "SDL thread: terminating gracefully." );
         }

      public void RequestStop()
         {
         m_ShouldLoop = false;
         }

      public bool IsAlive() 
         {
         return m_ShouldLoop;
         }
      }

   
   }
