namespace LevelEditorApp
   {
   partial class XMLControl
      {
      /// <summary> 
      /// Required designer variable.
      /// </summary>
      private System.ComponentModel.IContainer components = null;

      /// <summary> 
      /// Clean up any resources being used.
      /// </summary>
      /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
      protected override void Dispose( bool disposing )
         {
         if( disposing && ( components != null ) )
            {
            components.Dispose();
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
         this.webXMLView = new System.Windows.Forms.WebBrowser();
         this.SuspendLayout();
         // 
         // webXMLView
         // 
         this.webXMLView.Dock = System.Windows.Forms.DockStyle.Fill;
         this.webXMLView.Location = new System.Drawing.Point(0, 0);
         this.webXMLView.MinimumSize = new System.Drawing.Size(20, 20);
         this.webXMLView.Name = "webXMLView";
         this.webXMLView.Size = new System.Drawing.Size(150, 150);
         this.webXMLView.TabIndex = 0;
         // 
         // XMLControl
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.Controls.Add(this.webXMLView);
         this.Name = "XMLControl";
         this.ResumeLayout(false);

         }

      #endregion
      private System.Windows.Forms.WebBrowser webXMLView;
      }
   }
