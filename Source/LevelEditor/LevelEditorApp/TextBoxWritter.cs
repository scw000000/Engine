using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;

namespace LevelEditorApp
   {
   class TextBoxWritter : TextWriter
      {
      private int MAX_LINE_NUM = 100;
      TextBox _output = null;

      public TextBoxWritter( TextBox output )
         {
         _output = output;
         }

      public override void Write( char value )
         {
         base.Write( value );      
         String str = value.ToString();
         if( !_output.IsDisposed )
            {
            _output.AppendText( str );
            if( _output.Lines.Length >= MAX_LINE_NUM + 20)
               {
               int numOfLines = _output.Lines.Length - MAX_LINE_NUM; 
               var lines = this._output.Lines;
               var newLines = lines.Skip( numOfLines );
               this._output.Lines = newLines.ToArray();
               }
            }
         }

      public override Encoding Encoding
         {
         get { return System.Text.Encoding.UTF8; }
         }
      }
   }
