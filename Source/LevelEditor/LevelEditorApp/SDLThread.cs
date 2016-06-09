using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using SDL2;

namespace LevelEditorApp
   {
   class SDLThread
      {
      private volatile bool m_IsAlive;

      public SDLThread() 
         {
         m_IsAlive = true;
         }

      unsafe public void Run() 
         {
         while( m_IsAlive )
            {
            Program.s_Editor.Invoke( Program.s_Editor.m_UpdateSDLDelegate );
            }
         }

      bool IsAlive() 
         {
         return m_IsAlive;
         }
      }

   
   }
