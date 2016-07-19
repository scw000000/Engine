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
      private uint m_ActorId;

      public ActorData( string data, uint actorId ) 
         {
         m_IsDirty = false;
         m_Data = data;
         m_ActorId = actorId;
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

      public void SaveToFile() 
         {
         if( m_IsDirty )
            {
            NativeMethods.SaveActor( m_ActorId );
            m_IsDirty = false;
            }
         }

      public bool IsDirty() 
         {
         return m_IsDirty;
         }
      }
   }
