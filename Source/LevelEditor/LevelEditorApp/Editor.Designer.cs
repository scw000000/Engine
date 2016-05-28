namespace LevelEditorApp
{
    partial class Editor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
         this.menuStrip1 = new System.Windows.Forms.MenuStrip();
         this.fIleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.eDITToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.splitContainer1 = new System.Windows.Forms.SplitContainer();
         this.tabControl1 = new System.Windows.Forms.TabControl();
         this.tabPage1 = new System.Windows.Forms.TabPage();
         this.treeView1 = new System.Windows.Forms.TreeView();
         this.tabPage2 = new System.Windows.Forms.TabPage();
         this.treeView2 = new System.Windows.Forms.TreeView();
         this.splitContainer2 = new System.Windows.Forms.SplitContainer();
         this.tabControl2 = new System.Windows.Forms.TabControl();
         this.tabPage3 = new System.Windows.Forms.TabPage();
         this.tabPage4 = new System.Windows.Forms.TabPage();
         this.menuStrip1.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
         this.splitContainer1.Panel1.SuspendLayout();
         this.splitContainer1.Panel2.SuspendLayout();
         this.splitContainer1.SuspendLayout();
         this.tabControl1.SuspendLayout();
         this.tabPage1.SuspendLayout();
         this.tabPage2.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
         this.splitContainer2.Panel2.SuspendLayout();
         this.splitContainer2.SuspendLayout();
         this.tabControl2.SuspendLayout();
         this.SuspendLayout();
         // 
         // menuStrip1
         // 
         this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fIleToolStripMenuItem,
            this.eDITToolStripMenuItem});
         this.menuStrip1.Location = new System.Drawing.Point(0, 0);
         this.menuStrip1.Name = "menuStrip1";
         this.menuStrip1.Size = new System.Drawing.Size(759, 24);
         this.menuStrip1.TabIndex = 0;
         this.menuStrip1.Text = "menuStrip1";
         // 
         // fIleToolStripMenuItem
         // 
         this.fIleToolStripMenuItem.Name = "fIleToolStripMenuItem";
         this.fIleToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
         this.fIleToolStripMenuItem.Text = "FILE";
         // 
         // eDITToolStripMenuItem
         // 
         this.eDITToolStripMenuItem.Name = "eDITToolStripMenuItem";
         this.eDITToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
         this.eDITToolStripMenuItem.Text = "EDIT";
         // 
         // splitContainer1
         // 
         this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer1.Location = new System.Drawing.Point(0, 24);
         this.splitContainer1.Name = "splitContainer1";
         // 
         // splitContainer1.Panel1
         // 
         this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
         // 
         // splitContainer1.Panel2
         // 
         this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
         this.splitContainer1.Size = new System.Drawing.Size(759, 488);
         this.splitContainer1.SplitterDistance = 593;
         this.splitContainer1.SplitterWidth = 15;
         this.splitContainer1.TabIndex = 1;
         // 
         // tabControl1
         // 
         this.tabControl1.Controls.Add(this.tabPage1);
         this.tabControl1.Controls.Add(this.tabPage2);
         this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabControl1.Location = new System.Drawing.Point(0, 0);
         this.tabControl1.Name = "tabControl1";
         this.tabControl1.SelectedIndex = 0;
         this.tabControl1.Size = new System.Drawing.Size(151, 488);
         this.tabControl1.TabIndex = 0;
         // 
         // tabPage1
         // 
         this.tabPage1.Controls.Add(this.treeView1);
         this.tabPage1.Location = new System.Drawing.Point(4, 22);
         this.tabPage1.Name = "tabPage1";
         this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
         this.tabPage1.Size = new System.Drawing.Size(143, 462);
         this.tabPage1.TabIndex = 0;
         this.tabPage1.Text = "tabPage1";
         this.tabPage1.UseVisualStyleBackColor = true;
         // 
         // treeView1
         // 
         this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
         this.treeView1.Location = new System.Drawing.Point(3, 3);
         this.treeView1.Name = "treeView1";
         this.treeView1.Size = new System.Drawing.Size(137, 456);
         this.treeView1.TabIndex = 0;
         // 
         // tabPage2
         // 
         this.tabPage2.Controls.Add(this.treeView2);
         this.tabPage2.Location = new System.Drawing.Point(4, 22);
         this.tabPage2.Name = "tabPage2";
         this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
         this.tabPage2.Size = new System.Drawing.Size(483, 462);
         this.tabPage2.TabIndex = 1;
         this.tabPage2.Text = "tabPage2";
         this.tabPage2.UseVisualStyleBackColor = true;
         // 
         // treeView2
         // 
         this.treeView2.Dock = System.Windows.Forms.DockStyle.Fill;
         this.treeView2.Location = new System.Drawing.Point(3, 3);
         this.treeView2.Name = "treeView2";
         this.treeView2.Size = new System.Drawing.Size(477, 456);
         this.treeView2.TabIndex = 0;
         // 
         // splitContainer2
         // 
         this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer2.Location = new System.Drawing.Point(0, 0);
         this.splitContainer2.Name = "splitContainer2";
         this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
         // 
         // splitContainer2.Panel2
         // 
         this.splitContainer2.Panel2.Controls.Add(this.tabControl2);
         this.splitContainer2.Size = new System.Drawing.Size(593, 488);
         this.splitContainer2.SplitterDistance = 394;
         this.splitContainer2.SplitterWidth = 15;
         this.splitContainer2.TabIndex = 0;
         // 
         // tabControl2
         // 
         this.tabControl2.Alignment = System.Windows.Forms.TabAlignment.Bottom;
         this.tabControl2.Controls.Add(this.tabPage3);
         this.tabControl2.Controls.Add(this.tabPage4);
         this.tabControl2.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabControl2.Location = new System.Drawing.Point(0, 0);
         this.tabControl2.Multiline = true;
         this.tabControl2.Name = "tabControl2";
         this.tabControl2.SelectedIndex = 0;
         this.tabControl2.Size = new System.Drawing.Size(593, 79);
         this.tabControl2.TabIndex = 0;
         // 
         // tabPage3
         // 
         this.tabPage3.Location = new System.Drawing.Point(4, 4);
         this.tabPage3.Name = "tabPage3";
         this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
         this.tabPage3.Size = new System.Drawing.Size(585, 53);
         this.tabPage3.TabIndex = 0;
         this.tabPage3.Text = "tabPage3";
         this.tabPage3.UseVisualStyleBackColor = true;
         // 
         // tabPage4
         // 
         this.tabPage4.Location = new System.Drawing.Point(4, 4);
         this.tabPage4.Name = "tabPage4";
         this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
         this.tabPage4.Size = new System.Drawing.Size(585, 53);
         this.tabPage4.TabIndex = 1;
         this.tabPage4.Text = "tabPage4";
         this.tabPage4.UseVisualStyleBackColor = true;
         // 
         // Editor
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(759, 512);
         this.Controls.Add(this.splitContainer1);
         this.Controls.Add(this.menuStrip1);
         this.MainMenuStrip = this.menuStrip1;
         this.Name = "Editor";
         this.Text = "Form1";
         this.menuStrip1.ResumeLayout(false);
         this.menuStrip1.PerformLayout();
         this.splitContainer1.Panel1.ResumeLayout(false);
         this.splitContainer1.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
         this.splitContainer1.ResumeLayout(false);
         this.tabControl1.ResumeLayout(false);
         this.tabPage1.ResumeLayout(false);
         this.tabPage2.ResumeLayout(false);
         this.splitContainer2.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
         this.splitContainer2.ResumeLayout(false);
         this.tabControl2.ResumeLayout(false);
         this.ResumeLayout(false);
         this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fIleToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.TreeView treeView2;
        private System.Windows.Forms.ToolStripMenuItem eDITToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.TabControl tabControl2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
    }
}

