using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LevelEditorApp
   {
   public class ActorData
      {
      private bool m_IsDirty;
      

      public ActorData( string data ) 
         {
         m_IsDirty = false;
         m_Data = data;
         }

      private string m_Data;
      public string data
         {
         get
            {
            return m_Data;
            }
         set
            {
            if( !m_Data.Equals( value ) )
               {
               m_Data = value;
               m_IsDirty = true;
               }
            }
         }

      public bool IsDirty() 
         {
         return m_IsDirty;
         }
      }
   }
