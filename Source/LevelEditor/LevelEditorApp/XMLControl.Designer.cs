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
         this.rootPanel = new System.Windows.Forms.TableLayoutPanel();
         this.xmlPanel = new System.Windows.Forms.TableLayoutPanel();
         this.button1 = new System.Windows.Forms.Button();
         this.rootPanel.SuspendLayout();
         this.SuspendLayout();
         // 
         // rootPanel
         // 
         this.rootPanel.AutoScroll = true;
         this.rootPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.rootPanel.Controls.Add(this.xmlPanel, 0, 1);
         this.rootPanel.Controls.Add(this.button1, 0, 0);
         this.rootPanel.Dock = System.Windows.Forms.DockStyle.Fill;
         this.rootPanel.Location = new System.Drawing.Point(0, 0);
         this.rootPanel.Name = "rootPanel";
         this.rootPanel.RowCount = 2;
         this.rootPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
         this.rootPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
         this.rootPanel.Size = new System.Drawing.Size(0, 0);
         this.rootPanel.TabIndex = 0;
         // 
         // xmlPanel
         // 
         this.xmlPanel.AutoSize = true;
         this.xmlPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.xmlPanel.ColumnCount = 2;
         this.xmlPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.xmlPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.xmlPanel.Dock = System.Windows.Forms.DockStyle.Fill;
         this.xmlPanel.Location = new System.Drawing.Point(3, 43);
         this.xmlPanel.Name = "xmlPanel";
         this.xmlPanel.Size = new System.Drawing.Size(75, 1);
         this.xmlPanel.TabIndex = 1;
         // 
         // button1
         // 
         this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
         this.button1.BackColor = System.Drawing.Color.Silver;
         this.button1.Location = new System.Drawing.Point(3, 3);
         this.button1.Name = "button1";
         this.button1.Size = new System.Drawing.Size(75, 34);
         this.button1.TabIndex = 1;
         this.button1.Text = "button1";
         this.button1.UseVisualStyleBackColor = false;
         this.button1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.button1_MouseClick);
         // 
         // XMLControl
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.AutoScroll = true;
         this.AutoSize = true;
         this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.Controls.Add(this.rootPanel);
         this.Name = "XMLControl";
         this.Size = new System.Drawing.Size(0, 0);
         this.rootPanel.ResumeLayout(false);
         this.rootPanel.PerformLayout();
         this.ResumeLayout(false);

         }

      #endregion
      private System.Windows.Forms.TableLayoutPanel rootPanel;
      private System.Windows.Forms.Button button1;
      private System.Windows.Forms.TableLayoutPanel xmlPanel;
      }
   }
