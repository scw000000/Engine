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
         this.components = new System.ComponentModel.Container();
         this.menuStrip1 = new System.Windows.Forms.MenuStrip();
         this.fIleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.eDITToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.splitContainer1 = new System.Windows.Forms.SplitContainer();
         this.splitContainer2 = new System.Windows.Forms.SplitContainer();
         this.splitContainer_Left = new System.Windows.Forms.SplitContainer();
         this.tabCtlEX_LeftUp = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_LeftDown = new LevelEditorApp.TabControlEX();
         this.tabPageEX_Assets = new LevelEditorApp.TabPageEX(this.components);
         this.treeView_Assets = new System.Windows.Forms.TreeView();
         this.splitContainer_Mid = new System.Windows.Forms.SplitContainer();
         this.tabCtlEX_MidUp = new LevelEditorApp.TabControlEX();
         this.tabPageEX_World = new LevelEditorApp.TabPageEX(this.components);
         this.tabCtlEX_MidDown = new LevelEditorApp.TabControlEX();
         this.tabPageEX_Console = new LevelEditorApp.TabPageEX(this.components);
         this.splitContainer3 = new System.Windows.Forms.SplitContainer();
         this.textBox_Console = new System.Windows.Forms.TextBox();
         this.textBox1 = new System.Windows.Forms.TextBox();
         this.splitContainer_Right = new System.Windows.Forms.SplitContainer();
         this.tabCtlEX_RightUp = new LevelEditorApp.TabControlEX();
         this.tabCtlEX_RightDown = new LevelEditorApp.TabControlEX();
         this.tabPageEX_ActorXML = new LevelEditorApp.TabPageEX(this.components);
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
         this.tabCtlEX_LeftDown.SuspendLayout();
         this.tabPageEX_Assets.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Mid)).BeginInit();
         this.splitContainer_Mid.Panel1.SuspendLayout();
         this.splitContainer_Mid.Panel2.SuspendLayout();
         this.splitContainer_Mid.SuspendLayout();
         this.tabCtlEX_MidUp.SuspendLayout();
         this.tabCtlEX_MidDown.SuspendLayout();
         this.tabPageEX_Console.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
         this.splitContainer3.Panel1.SuspendLayout();
         this.splitContainer3.Panel2.SuspendLayout();
         this.splitContainer3.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Right)).BeginInit();
         this.splitContainer_Right.Panel1.SuspendLayout();
         this.splitContainer_Right.Panel2.SuspendLayout();
         this.splitContainer_Right.SuspendLayout();
         this.tabCtlEX_RightDown.SuspendLayout();
         this.tabPageEX_ActorXML.SuspendLayout();
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
         this.tabCtlEX_LeftDown.Controls.Add(this.tabPageEX_Assets);
         this.tabCtlEX_LeftDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_LeftDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_LeftDown.ItemSize = new System.Drawing.Size(91, 18);
         this.tabCtlEX_LeftDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_LeftDown.Name = "tabCtlEX_LeftDown";
         this.tabCtlEX_LeftDown.SelectedIndex = 0;
         this.tabCtlEX_LeftDown.Size = new System.Drawing.Size(130, 598);
         this.tabCtlEX_LeftDown.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
         this.tabCtlEX_LeftDown.TabIndex = 0;
         this.tabCtlEX_LeftDown.TabStop = false;
         // 
         // tabPageEX_Assets
         // 
         this.tabPageEX_Assets.AllowDrop = true;
         this.tabPageEX_Assets.CloseBtnEnabled = false;
         this.tabPageEX_Assets.Controls.Add(this.treeView_Assets);
         this.tabPageEX_Assets.Location = new System.Drawing.Point(4, 22);
         this.tabPageEX_Assets.Menu = null;
         this.tabPageEX_Assets.Name = "tabPageEX_Assets";
         this.tabPageEX_Assets.Size = new System.Drawing.Size(122, 572);
         this.tabPageEX_Assets.TabIndex = 0;
         this.tabPageEX_Assets.Text = "Assets View";
         // 
         // treeView_Assets
         // 
         this.treeView_Assets.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.treeView_Assets.Dock = System.Windows.Forms.DockStyle.Fill;
         this.treeView_Assets.LineColor = System.Drawing.Color.WhiteSmoke;
         this.treeView_Assets.Location = new System.Drawing.Point(0, 0);
         this.treeView_Assets.Name = "treeView_Assets";
         this.treeView_Assets.RightToLeft = System.Windows.Forms.RightToLeft.No;
         this.treeView_Assets.Size = new System.Drawing.Size(122, 572);
         this.treeView_Assets.TabIndex = 0;
         this.treeView_Assets.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.treeView_Assets_ItemDrag);
         this.treeView_Assets.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView_Assets_NodeMouseClick);
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
         // tabCtlEX_MidUp
         // 
         this.tabCtlEX_MidUp.AllowDrop = true;
         this.tabCtlEX_MidUp.ConfirmOnClose = true;
         this.tabCtlEX_MidUp.Controls.Add(this.tabPageEX_World);
         this.tabCtlEX_MidUp.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_MidUp.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_MidUp.ItemSize = new System.Drawing.Size(91, 18);
         this.tabCtlEX_MidUp.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_MidUp.Name = "tabCtlEX_MidUp";
         this.tabCtlEX_MidUp.SelectedIndex = 0;
         this.tabCtlEX_MidUp.Size = new System.Drawing.Size(1602, 768);
         this.tabCtlEX_MidUp.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
         this.tabCtlEX_MidUp.TabIndex = 0;
         this.tabCtlEX_MidUp.TabStop = false;
         // 
         // tabPageEX_World
         // 
         this.tabPageEX_World.AllowDrop = true;
         this.tabPageEX_World.CloseBtnEnabled = false;
         this.tabPageEX_World.Location = new System.Drawing.Point(4, 22);
         this.tabPageEX_World.Menu = null;
         this.tabPageEX_World.Name = "tabPageEX_World";
         this.tabPageEX_World.Size = new System.Drawing.Size(1594, 742);
         this.tabPageEX_World.TabIndex = 0;
         this.tabPageEX_World.Text = "World View";
         // 
         // tabCtlEX_MidDown
         // 
         this.tabCtlEX_MidDown.Alignment = System.Windows.Forms.TabAlignment.Bottom;
         this.tabCtlEX_MidDown.ConfirmOnClose = true;
         this.tabCtlEX_MidDown.Controls.Add(this.tabPageEX_Console);
         this.tabCtlEX_MidDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_MidDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_MidDown.ItemSize = new System.Drawing.Size(71, 18);
         this.tabCtlEX_MidDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_MidDown.Name = "tabCtlEX_MidDown";
         this.tabCtlEX_MidDown.SelectedIndex = 0;
         this.tabCtlEX_MidDown.Size = new System.Drawing.Size(1602, 247);
         this.tabCtlEX_MidDown.TabIndex = 0;
         this.tabCtlEX_MidDown.TabStop = false;
         // 
         // tabPageEX_Console
         // 
         this.tabPageEX_Console.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.tabPageEX_Console.CloseBtnEnabled = false;
         this.tabPageEX_Console.Controls.Add(this.splitContainer3);
         this.tabPageEX_Console.Location = new System.Drawing.Point(4, 4);
         this.tabPageEX_Console.Menu = null;
         this.tabPageEX_Console.Name = "tabPageEX_Console";
         this.tabPageEX_Console.Size = new System.Drawing.Size(1594, 221);
         this.tabPageEX_Console.TabIndex = 0;
         this.tabPageEX_Console.Text = "Console";
         // 
         // splitContainer3
         // 
         this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
         this.splitContainer3.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
         this.splitContainer3.IsSplitterFixed = true;
         this.splitContainer3.Location = new System.Drawing.Point(0, 0);
         this.splitContainer3.Name = "splitContainer3";
         this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
         // 
         // splitContainer3.Panel1
         // 
         this.splitContainer3.Panel1.Controls.Add(this.textBox_Console);
         // 
         // splitContainer3.Panel2
         // 
         this.splitContainer3.Panel2.Controls.Add(this.textBox1);
         this.splitContainer3.Size = new System.Drawing.Size(1594, 221);
         this.splitContainer3.SplitterDistance = 192;
         this.splitContainer3.TabIndex = 2;
         // 
         // textBox_Console
         // 
         this.textBox_Console.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.textBox_Console.Dock = System.Windows.Forms.DockStyle.Fill;
         this.textBox_Console.ForeColor = System.Drawing.Color.Silver;
         this.textBox_Console.Location = new System.Drawing.Point(0, 0);
         this.textBox_Console.Multiline = true;
         this.textBox_Console.Name = "textBox_Console";
         this.textBox_Console.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
         this.textBox_Console.Size = new System.Drawing.Size(1594, 192);
         this.textBox_Console.TabIndex = 0;
         // 
         // textBox1
         // 
         this.textBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
         this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
         this.textBox1.ForeColor = System.Drawing.Color.Silver;
         this.textBox1.Location = new System.Drawing.Point(0, 0);
         this.textBox1.Name = "textBox1";
         this.textBox1.Size = new System.Drawing.Size(1594, 22);
         this.textBox1.TabIndex = 1;
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
         this.tabCtlEX_RightDown.Controls.Add(this.tabPageEX_ActorXML);
         this.tabCtlEX_RightDown.Dock = System.Windows.Forms.DockStyle.Fill;
         this.tabCtlEX_RightDown.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         this.tabCtlEX_RightDown.ItemSize = new System.Drawing.Size(88, 18);
         this.tabCtlEX_RightDown.Location = new System.Drawing.Point(0, 0);
         this.tabCtlEX_RightDown.Name = "tabCtlEX_RightDown";
         this.tabCtlEX_RightDown.SelectedIndex = 0;
         this.tabCtlEX_RightDown.Size = new System.Drawing.Size(158, 625);
         this.tabCtlEX_RightDown.TabIndex = 0;
         this.tabCtlEX_RightDown.TabStop = false;
         // 
         // tabPageEX_ActorXML
         // 
         this.tabPageEX_ActorXML.AllowDrop = true;
         this.tabPageEX_ActorXML.CloseBtnEnabled = false;
         this.tabPageEX_ActorXML.Controls.Add(this.xmlControl_ActorXML);
         this.tabPageEX_ActorXML.Location = new System.Drawing.Point(4, 22);
         this.tabPageEX_ActorXML.Menu = null;
         this.tabPageEX_ActorXML.Name = "tabPageEX_ActorXML";
         this.tabPageEX_ActorXML.Size = new System.Drawing.Size(150, 599);
         this.tabPageEX_ActorXML.TabIndex = 0;
         this.tabPageEX_ActorXML.Text = "Actor XML";
         // 
         // xmlControl_ActorXML
         // 
         this.xmlControl_ActorXML.AutoScroll = true;
         this.xmlControl_ActorXML.AutoSize = true;
         this.xmlControl_ActorXML.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.xmlControl_ActorXML.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
         this.xmlControl_ActorXML.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
         this.xmlControl_ActorXML.DataFilePath = "";
         this.xmlControl_ActorXML.DataString = null;
         this.xmlControl_ActorXML.Dock = System.Windows.Forms.DockStyle.Fill;
         this.xmlControl_ActorXML.Location = new System.Drawing.Point(0, 0);
         this.xmlControl_ActorXML.Name = "xmlControl_ActorXML";
         this.xmlControl_ActorXML.Size = new System.Drawing.Size(150, 599);
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
         this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Editor_FormClosing);
         this.Load += new System.EventHandler(this.Editor_Load);
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
         this.tabCtlEX_LeftDown.ResumeLayout(false);
         this.tabPageEX_Assets.ResumeLayout(false);
         this.splitContainer_Mid.Panel1.ResumeLayout(false);
         this.splitContainer_Mid.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Mid)).EndInit();
         this.splitContainer_Mid.ResumeLayout(false);
         this.tabCtlEX_MidUp.ResumeLayout(false);
         this.tabCtlEX_MidDown.ResumeLayout(false);
         this.tabPageEX_Console.ResumeLayout(false);
         this.splitContainer3.Panel1.ResumeLayout(false);
         this.splitContainer3.Panel1.PerformLayout();
         this.splitContainer3.Panel2.ResumeLayout(false);
         this.splitContainer3.Panel2.PerformLayout();
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
         this.splitContainer3.ResumeLayout(false);
         this.splitContainer_Right.Panel1.ResumeLayout(false);
         this.splitContainer_Right.Panel2.ResumeLayout(false);
         ((System.ComponentModel.ISupportInitialize)(this.splitContainer_Right)).EndInit();
         this.splitContainer_Right.ResumeLayout(false);
         this.tabCtlEX_RightDown.ResumeLayout(false);
         this.tabPageEX_ActorXML.ResumeLayout(false);
         this.tabPageEX_ActorXML.PerformLayout();
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
        private System.Windows.Forms.TreeView treeView_Assets;
        private System.Windows.Forms.TextBox textBox_Console;
        private LevelEditorApp.TabControlEX tabCtlEX_LeftUp;
        private LevelEditorApp.TabControlEX tabCtlEX_LeftDown;
        private LevelEditorApp.TabControlEX tabCtlEX_MidUp;
        private LevelEditorApp.TabControlEX tabCtlEX_MidDown;
        private LevelEditorApp.TabControlEX tabCtlEX_RightUp;
        private LevelEditorApp.TabControlEX tabCtlEX_RightDown;
        private LevelEditorApp.TabPageEX tabPageEX_Assets;
        private LevelEditorApp.TabPageEX tabPageEX_World;
        private LevelEditorApp.TabPageEX tabPageEX_ActorXML;
        private LevelEditorApp.TabPageEX tabPageEX_Console;
        private LevelEditorApp.XMLControl xmlControl_ActorXML;
        private TextBox textBox1;
        private SplitContainer splitContainer3;
    }
}

