using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LevelEditorApp
   {
   class TextBoxEX : TextBox
      {
      public String m_FilePath;
      public event FormClosingEventHandler OnClose;
      //public event FormClosingEventHandler FormClosing;
      public TextBoxEX( string filePath )
         {
         
         m_FilePath = filePath;
         this.OnClose += new FormClosingEventHandler( this.OnTextBoxClose );
         }
      private void OnTextBoxClose( object sender, FormClosingEventArgs e ) 
         {
         
         }
      
      }
   }
