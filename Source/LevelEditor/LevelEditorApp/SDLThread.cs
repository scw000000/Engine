using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LevelEditorApp
   {
   class SDLThread
      {
      public void Run() 
         {
         while( m_IsAlive )
            {
            NativeMethods.SingleLoop();
            }
         }

      bool IsAlive() 
         {
         return m_IsAlive;
         }

      private volatile bool m_IsAlive;
      }

   
   }
