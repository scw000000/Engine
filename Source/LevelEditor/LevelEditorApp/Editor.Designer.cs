using System.Windows.Forms;

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
         this.splitContainer2 = new System.Windows.Forms.SplitContainer();
         this.splitContainer_Left = new System.Windows.Forms.SplitContainer();
         this.splitContainer_Mid = new System.Windows.Forms.SplitContainer();
         this.splitContainer_Right = new System.Windows.Forms.SplitContainer();
         this.tabCtlEX_LeftUp = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_LeftDown = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_MidUp = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_MidDown = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_RightUp = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_RightDown = new LevelEditorApp.TabControlEX();
         this.xmlControl_ActorXML = new LevelEditorApp.XMLControl();
         this.menuStrip1.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
         this.splitContainer1.Panel1.SuspendLayout();
         this.splitContainer1.Panel2.SuspendLayout();
         this.splitContainer1.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
         this.splitContainer2.Panel1.SuspendLayout();
         this.splitContainer2.Panel2.SuspendLayout();
         this.splitContainer2.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Left)).BeginInit();
         this.splitContainer_Left.Panel1.SuspendLayout();
         this.splitContainer_Left.Panel2.SuspendLayout();
         this.splitContainer_Left.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Mid)).BeginInit();
         this.splitContainer_Mid.Panel1.SuspendLayout();
         this.splitContainer_Mid.Panel2.SuspendLayout();
         this.splitContainer_Mid.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Right)).BeginInit();
         this.splitContainer_Right.Panel1.SuspendLayout();
         this.splitContainer_Right.Panel2.SuspendLayout();
         this.splitContainer_Right.SuspendLayout();
         this.SuspendLayout();
         // 
         // menuStrip1
         // 
         this.menuStrip1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
         this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fIleToolStripMenuItem,
            this.eDITToolStripMenuItem});
         this.menuStrip1.Location = new System.Drawing.Point(0, 0);
         this.menuStrip1.Name = "menuStrip1";
         this.menuStrip1.Size = new System.Drawing.Size(1912, 24);
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
         this.splitContainer1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
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
         this.splitContainer1.Panel2.Controls.Add(this.splitContainer_Right);
         this.splitContainer1.Size = new System.Drawing.Size(1912, 1029);
         this.splitContainer1.SplitterDistance = 1744;
         this.splitContainer1.SplitterWidth = 10;
         this.splitContainer1.TabIndex = 1;
         // 
         // splitContainer2
         // 
         this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer2.Location = new System.Drawing.Point(0, 0);
         this.splitContainer2.Name = "splitContainer2";
         // 
         // splitContainer2.Panel1
         // 
         this.splitContainer2.Panel1.Controls.Add(this.splitContainer_Left);
         // 
         // splitContainer2.Panel2
         // 
         this.splitContainer2.Panel2.Controls.Add(this.splitContainer_Mid);
         this.splitContainer2.Size = new System.Drawing.Size(1744, 1029);
         this.splitContainer2.SplitterDistance = 130;
         this.splitContainer2.SplitterWidth = 10;
         this.splitContainer2.TabIndex = 0;
         // 
         // splitContainer_Left
         // 
         this.splitContainer_Left.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.splitContainer_Left.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer_Left.Location = new System.Drawing.Point(0, 0);
         this.splitContainer_Left.Name = "splitContainer_Left";
         this.splitContainer_Left.Orientation = System.Windows.Forms.Orientation.Horizontal;
         // 
         // splitContainer_Left.Panel1
         // 
         this.splitContainer_Left.Panel1.Controls.Add(this.tabCtlEX_LeftUp);
         // 
         // splitContainer_Left.Panel2
         // 
         this.splitContainer_Left.Panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.splitContainer_Left.Panel2.Controls.Add(this.tabCtlEX_LeftDown);
         this.splitContainer_Left.Size = new System.Drawing.Size(130, 1029);
         this.splitContainer_Left.SplitterDistance = 421;
         this.splitContainer_Left.SplitterWidth = 10;
         this.splitContainer_Left.TabIndex = 0;
         // 
         // splitContainer_Mid
         // 
         this.splitContainer_Mid.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.splitContainer_Mid.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
         this.splitContainer_Mid.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer_Mid.Location = new System.Drawing.Point(0, 0);
         this.splitContainer_Mid.Name = "splitContainer_Mid";
         this.splitContainer_Mid.Orientation = System.Windows.Forms.Orientation.Horizontal;
         // 
         // splitContainer_Mid.Panel1
         // 
         this.splitContainer_Mid.Panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.splitContainer_Mid.Panel1.Controls.Add(this.tabCtlEX_MidUp);
         // 
         // splitContainer_Mid.Panel2
         // 
         this.splitContainer_Mid.Panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.splitContainer_Mid.Panel2.Controls.Add(this.tabCtlEX_MidDown);
         this.splitContainer_Mid.Size = new System.Drawing.Size(1604, 1029);
         this.splitContainer_Mid.SplitterDistance = 770;
         this.splitContainer_Mid.SplitterWidth = 10;
         this.splitContainer_Mid.TabIndex = 0;
         // 
         // splitContainer_Right
         // 
         this.splitContainer_Right.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.splitContainer_Right.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer_Right.Location = new System.Drawing.Point(0, 0);
         this.splitContainer_Right.Name = "splitContainer_Right";
         this.splitContainer_Right.Orientation = System.Windows.Forms.Orientation.Horizontal;
         // 
         // splitContainer_Right.Panel1
         // 
         this.splitContainer_Right.Panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.splitContainer_Right.Panel1.Controls.Add(this.tabCtlEX_RightUp);
         // 
         // splitContainer_Right.Panel2
         // 
         this.splitContainer_Right.Panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.splitContainer_Right.Panel2.Controls.Add(this.tabCtlEX_RightDown);
         this.splitContainer_Right.Size = new System.Drawing.Size(158, 1029);
         this.splitContainer_Right.SplitterDistance = 394;
         this.splitContainer_Right.SplitterWidth = 10;
         this.splitContainer_Right.TabIndex = 1;
         // 
         // tabCtlEX_LeftUp
         // 
         this.tabCtlEX_LeftUp.ConfirmOnClose = true;
         this.tabCtlEX_LeftUp.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_LeftUp.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_LeftUp.ItemSize = new System.Drawing.Size(50, 18);
         this.tabCtlEX_LeftUp.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_LeftUp.Name = "tabCtlEX_LeftUp";
         this.tabCtlEX_LeftUp.SelectedIndex = 0;
         this.tabCtlEX_LeftUp.Size = new System.Drawing.Size(130, 421);
         this.tabCtlEX_LeftUp.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
         this.tabCtlEX_LeftUp.TabIndex = 0;
         this.tabCtlEX_LeftUp.TabStop = false;
         // 
         // tabCtlEX_LeftDown
         // 
         this.tabCtlEX_LeftDown.ConfirmOnClose = true;
         this.tabCtlEX_LeftDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_LeftDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_LeftDown.ItemSize = new System.Drawing.Size(50, 18);
         this.tabCtlEX_LeftDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_LeftDown.Name = "tabCtlEX_LeftDown";
         this.tabCtlEX_LeftDown.SelectedIndex = 0;
         this.tabCtlEX_LeftDown.Size = new System.Drawing.Size(130, 598);
         this.tabCtlEX_LeftDown.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
         this.tabCtlEX_LeftDown.TabIndex = 0;
         this.tabCtlEX_LeftDown.TabStop = false;
         // 
         // tabCtlEX_MidUp
         // 
         this.tabCtlEX_MidUp.AllowDrop = true;
         this.tabCtlEX_MidUp.ConfirmOnClose = true;
         this.tabCtlEX_MidUp.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_MidUp.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_MidUp.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_MidUp.Name = "tabCtlEX_MidUp";
         this.tabCtlEX_MidUp.SelectedIndex = 0;
         this.tabCtlEX_MidUp.Size = new System.Drawing.Size(1602, 768);
         this.tabCtlEX_MidUp.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
         this.tabCtlEX_MidUp.TabIndex = 0;
         this.tabCtlEX_MidUp.TabStop = false;
         // 
         // tabCtlEX_MidDown
         // 
         this.tabCtlEX_MidDown.Alignment = System.Windows.Forms.TabAlignment.Bottom;
         this.tabCtlEX_MidDown.ConfirmOnClose = true;
         this.tabCtlEX_MidDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_MidDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_MidDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_MidDown.Name = "tabCtlEX_MidDown";
         this.tabCtlEX_MidDown.SelectedIndex = 0;
         this.tabCtlEX_MidDown.Size = new System.Drawing.Size(1602, 247);
         this.tabCtlEX_MidDown.TabIndex = 0;
         this.tabCtlEX_MidDown.TabStop = false;
         // 
         // tabCtlEX_RightUp
         // 
         this.tabCtlEX_RightUp.ConfirmOnClose = true;
         this.tabCtlEX_RightUp.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_RightUp.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_RightUp.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_RightUp.Name = "tabCtlEX_RightUp";
         this.tabCtlEX_RightUp.SelectedIndex = 0;
         this.tabCtlEX_RightUp.Size = new System.Drawing.Size(158, 394);
         this.tabCtlEX_RightUp.TabIndex = 0;
         this.tabCtlEX_RightUp.TabStop = false;
         // 
         // tabCtlEX_RightDown
         // 
         this.tabCtlEX_RightDown.ConfirmOnClose = true;
         this.tabCtlEX_RightDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_RightDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_RightDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_RightDown.Name = "tabCtlEX_RightDown";
         this.tabCtlEX_RightDown.SelectedIndex = 0;
         this.tabCtlEX_RightDown.Size = new System.Drawing.Size(158, 625);
         this.tabCtlEX_RightDown.TabIndex = 0;
         this.tabCtlEX_RightDown.TabStop = false;
         // 
         // xmlControl_ActorXML
         // 
         this.xmlControl_ActorXML.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
         this.xmlControl_ActorXML.Dock = System.Windows.Forms.DockStyle.Fill;
         this.xmlControl_ActorXML.DataFilePath = "";
         this.xmlControl_ActorXML.DataSetTableIndex = 0;
         this.xmlControl_ActorXML.Location = new System.Drawing.Point( 12, 41 );
         this.xmlControl_ActorXML.Name = "XMLControl";
         this.xmlControl_ActorXML.TabIndex = 0;
         this.xmlControl_ActorXML.ViewMode = LevelEditorApp.XMLControl.VIEW_MODE.TABLE;
         // 
         // Editor
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(1912, 1053);
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
         this.splitContainer2.Panel1.ResumeLayout(false);
         this.splitContainer2.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
         this.splitContainer2.ResumeLayout(false);
         this.splitContainer_Left.Panel1.ResumeLayout(false);
         this.splitContainer_Left.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Left)).EndInit();
         this.splitContainer_Left.ResumeLayout(false);
         this.splitContainer_Mid.Panel1.ResumeLayout(false);
         this.splitContainer_Mid.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Mid)).EndInit();
         this.splitContainer_Mid.ResumeLayout(false);
         this.splitContainer_Right.Panel1.ResumeLayout(false);
         this.splitContainer_Right.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Right)).EndInit();
         this.splitContainer_Right.ResumeLayout(false);
         this.ResumeLayout(false);
         this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fIleToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripMenuItem eDITToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer_Left;
        private System.Windows.Forms.SplitContainer splitContainer_Mid;
        private System.Windows.Forms.SplitContainer splitContainer_Right;
        private LevelEditorApp.TabControlEX tabCtlEX_LeftUp;
        private LevelEditorApp.TabControlEX tabCtlEX_LeftDown;
        private LevelEditorApp.TabControlEX tabCtlEX_MidUp;
        private LevelEditorApp.TabControlEX tabCtlEX_MidDown;
        private LevelEditorApp.TabControlEX tabCtlEX_RightUp;
        private LevelEditorApp.TabControlEX tabCtlEX_RightDown;
        private LevelEditorApp.TabPageEX tabPageEX_Assets;
        private LevelEditorApp.TabPageEX tabPageEX_World;
        private LevelEditorApp.TabPageEX tabPageEX_ActorXML;
        private LevelEditorApp.XMLControl xmlControl_ActorXML;
    }
}

