using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SDL2;

namespace LevelEditorApp
   {
   public partial class Editor : Form
      {
      public Editor()
         {
         InitializeComponent();
         SDL2.SDL.SDL_Init( SDL2.SDL.SDL_INIT_EVERYTHING );
         IntPtr pWindow = SDL2.SDL.SDL_CreateWindowFrom( this.splitContainer1.Panel1.Handle );

         }
      }
   }
