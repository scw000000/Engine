﻿using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace LevelEditorApp
   {
   public class TabPageEX: System.Windows.Forms.TabPage
	   {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
      public TabPageEX(System.ComponentModel.IContainer container)
		   {
			///
			/// Required for Windows.Forms Class Composition Designer support
			///
         if( container != null )
            {
            container.Add( this );
            }
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		   }

      public TabPageEX()
		   {
			///
			/// Required for Windows.Forms Class Composition Designer support
			///
			InitializeComponent();
         
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		   }

      public void tabPageEX_DragEnter( object sender, DragEventArgs e )
         {
         e.Effect = DragDropEffects.All;
         }

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		   {
			if( disposing )
			   {
				if(components != null)
				   {
					components.Dispose();
				   }
			   }
			base.Dispose( disposing );
		   }

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		   {
			components = new System.ComponentModel.Container();
		   }

      private bool isCloseBtnEnabled = false;
      public bool CloseBtnEnabled
         {
         get
            {
            return this.isCloseBtnEnabled;
            }
         set
            {
            this.isCloseBtnEnabled = value;
            }
         }
      private ContextMenu ctxtMenu = null;
      public ContextMenu Menu
         {
         get
            {
            return this.ctxtMenu;
            }
         set
            {
            this.ctxtMenu = value;
            }
        }
		#endregion
	   }
   }
