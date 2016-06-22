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
         this.xmlTreeView = new System.Windows.Forms.TreeView();
         this.xmlPanel = new System.Windows.Forms.TableLayoutPanel();
         this.rootPanel.SuspendLayout();
         this.SuspendLayout();
         // 
         // rootPanel
         // 
         this.rootPanel.AutoSize = true;
         this.rootPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.rootPanel.BackColor = System.Drawing.Color.FromArgb( ( (int) ( ( (byte) ( 64 ) ) ) ), ( (int) ( ( (byte) ( 64 ) ) ) ), ( (int) ( ( (byte) ( 64 ) ) ) ) );
         this.rootPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Outset;
         this.rootPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.rootPanel.Controls.Add(this.xmlTreeView, 0, 0);
         this.rootPanel.Controls.Add(this.xmlPanel, 0, 1);
         this.rootPanel.Location = new System.Drawing.Point(0, 0);
         this.rootPanel.Name = "rootPanel";
         this.rootPanel.RowCount = 2;
         this.rootPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
         this.rootPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
         this.rootPanel.Size = new System.Drawing.Size(131, 118);
         this.rootPanel.TabIndex = 0;
         // 
         // xmlTreeView
         // 
         this.xmlTreeView.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.xmlTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
         this.xmlTreeView.ForeColor = System.Drawing.Color.Silver;
         this.xmlTreeView.HotTracking = true;
         this.xmlTreeView.LineColor = System.Drawing.Color.Silver;
         this.xmlTreeView.Location = new System.Drawing.Point(5, 5);
         this.xmlTreeView.Name = "xmlTreeView";
         this.xmlTreeView.Size = new System.Drawing.Size(121, 100);
         this.xmlTreeView.TabIndex = 0;
         this.xmlTreeView.AfterExpand += new System.Windows.Forms.TreeViewEventHandler(this.treeViewAfterExpand);
         // 
         // xmlPanel
         // 
         this.xmlPanel.AutoScroll = true;
         this.xmlPanel.AutoSize = true;
         this.xmlPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.xmlPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.xmlPanel.ColumnCount = 2;
         this.xmlPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.xmlPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
         this.xmlPanel.Location = new System.Drawing.Point(5, 113);
         this.xmlPanel.Name = "xmlPanel";
         this.xmlPanel.Size = new System.Drawing.Size(0, 0);
         this.xmlPanel.TabIndex = 1;
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
         this.Size = new System.Drawing.Size(134, 121);
         this.rootPanel.ResumeLayout(false);
         this.rootPanel.PerformLayout();
         this.ResumeLayout(false);
         this.PerformLayout();

         }

      #endregion
      private System.Windows.Forms.TableLayoutPanel rootPanel;
      private System.Windows.Forms.TableLayoutPanel xmlPanel;
      private System.Windows.Forms.TreeView xmlTreeView;
      }
   }
