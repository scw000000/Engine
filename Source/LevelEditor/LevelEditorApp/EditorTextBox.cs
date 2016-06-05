using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace LevelEditorApp
   {
   public class EditorTextBox : TextBox
      {
      public String m_FilePath;
      private String m_OriginalFile;
      public EditorTextBox( string name, string filePath )
         {
         m_FilePath = filePath;
         this.Dock = DockStyle.Fill;
         this.Name = name;
         this.BackColor = Color.FromArgb( 255, 49, 49, 49 );
         this.ForeColor = Color.WhiteSmoke;
         this.Multiline = true;
         this.ScrollBars = ScrollBars.Vertical;

         try
            {
            StreamReader objstream = new StreamReader( m_FilePath );
            this.Text = objstream.ReadToEnd();
            m_OriginalFile = this.Text;
            objstream.Close();
            }
         catch( Exception e )
            {
            MessageBox.Show( "Err: " + e.ToString() );
            }

         }
      public bool ConfirmClose()
         {
         DialogResult messageBoxAns = DialogResult.Yes;
         if( !this.Text.Equals( m_OriginalFile ) )
            {
            messageBoxAns = MessageBox.Show( "You modified this file, would you like to save?", 
               "Confirm close",
               MessageBoxButtons.YesNoCancel );
            if( messageBoxAns == ( DialogResult.Yes ) )
               {
               try
                  {
                  StreamWriter objstream = new StreamWriter( m_FilePath );
                  objstream.Write( this.Text );
                  objstream.Close();
                  }
               catch( Exception e )
                  {
                  MessageBox.Show( "Err: " + e.ToString() );
                  return true;
                  }
               }     
            }
         return ( messageBoxAns == DialogResult.Yes || messageBoxAns == DialogResult.No );
         }

      }
   }
