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
      TextBox _output = null;

      public TextBoxWritter( TextBox output )
         {
         _output = output;
         }

      public override void Write( char value )
         {
         base.Write( value );
         String str = value.ToString();
         _output.AppendText( str );
         }

      public override Encoding Encoding
         {
         get { return System.Text.Encoding.UTF8; }
         }
      }
   }
