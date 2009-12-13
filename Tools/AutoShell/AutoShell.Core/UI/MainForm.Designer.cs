namespace AutoShell.UI
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.toolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.splitLeft = new System.Windows.Forms.SplitContainer();
            this.tabLeft = new System.Windows.Forms.TabControl();
            this.imageListLeft = new System.Windows.Forms.ImageList(this.components);
            this.splitRight = new System.Windows.Forms.SplitContainer();
            this.splitTop = new System.Windows.Forms.SplitContainer();
            this.tabTop = new System.Windows.Forms.TabControl();
            this.imageListTop = new System.Windows.Forms.ImageList(this.components);
            this.splitBottom = new System.Windows.Forms.SplitContainer();
            this.tabDocument = new System.Windows.Forms.TabControl();
            this.imageListDocument = new System.Windows.Forms.ImageList(this.components);
            this.tabBottom = new System.Windows.Forms.TabControl();
            this.imageListBottom = new System.Windows.Forms.ImageList(this.components);
            this.tabRight = new System.Windows.Forms.TabControl();
            this.imageListRight = new System.Windows.Forms.ImageList(this.components);
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.toolStripContainer.BottomToolStripPanel.SuspendLayout();
            this.toolStripContainer.ContentPanel.SuspendLayout();
            this.toolStripContainer.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer.SuspendLayout();
            this.splitLeft.Panel1.SuspendLayout();
            this.splitLeft.Panel2.SuspendLayout();
            this.splitLeft.SuspendLayout();
            this.splitRight.Panel1.SuspendLayout();
            this.splitRight.Panel2.SuspendLayout();
            this.splitRight.SuspendLayout();
            this.splitTop.Panel1.SuspendLayout();
            this.splitTop.Panel2.SuspendLayout();
            this.splitTop.SuspendLayout();
            this.splitBottom.Panel1.SuspendLayout();
            this.splitBottom.Panel2.SuspendLayout();
            this.splitBottom.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripContainer
            // 
            // 
            // toolStripContainer.BottomToolStripPanel
            // 
            this.toolStripContainer.BottomToolStripPanel.Controls.Add(this.statusStripMain);
            // 
            // toolStripContainer.ContentPanel
            // 
            this.toolStripContainer.ContentPanel.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.toolStripContainer.ContentPanel.Controls.Add(this.splitLeft);
            this.toolStripContainer.ContentPanel.Size = new System.Drawing.Size(526, 302);
            this.toolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer.Name = "toolStripContainer";
            this.toolStripContainer.Size = new System.Drawing.Size(526, 328);
            this.toolStripContainer.TabIndex = 0;
            this.toolStripContainer.Text = "toolStripContainer1";
            // 
            // toolStripContainer.TopToolStripPanel
            // 
            this.toolStripContainer.TopToolStripPanel.Controls.Add(this.menuStripMain);
            // 
            // statusStripMain
            // 
            this.statusStripMain.Dock = System.Windows.Forms.DockStyle.None;
            this.statusStripMain.Location = new System.Drawing.Point(0, 0);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(526, 22);
            this.statusStripMain.TabIndex = 0;
            // 
            // splitLeft
            // 
            this.splitLeft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitLeft.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitLeft.Location = new System.Drawing.Point(0, 0);
            this.splitLeft.Name = "splitLeft";
            // 
            // splitLeft.Panel1
            // 
            this.splitLeft.Panel1.Controls.Add(this.tabLeft);
            this.splitLeft.Panel1MinSize = 1;
            // 
            // splitLeft.Panel2
            // 
            this.splitLeft.Panel2.Controls.Add(this.splitRight);
            this.splitLeft.Size = new System.Drawing.Size(526, 302);
            this.splitLeft.SplitterDistance = 64;
            this.splitLeft.TabIndex = 0;
            // 
            // tabLeft
            // 
            this.tabLeft.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabLeft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabLeft.ImageList = this.imageListLeft;
            this.tabLeft.Location = new System.Drawing.Point(0, 0);
            this.tabLeft.Multiline = true;
            this.tabLeft.Name = "tabLeft";
            this.tabLeft.SelectedIndex = 0;
            this.tabLeft.Size = new System.Drawing.Size(64, 302);
            this.tabLeft.TabIndex = 0;
            this.tabLeft.Click += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabLeft.Enter += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabLeft.SelectedIndexChanged += new System.EventHandler(this.tabToolWindow_SelectedIndexChanged);
            // 
            // imageListLeft
            // 
            this.imageListLeft.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageListLeft.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListLeft.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // splitRight
            // 
            this.splitRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitRight.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitRight.Location = new System.Drawing.Point(0, 0);
            this.splitRight.Name = "splitRight";
            // 
            // splitRight.Panel1
            // 
            this.splitRight.Panel1.Controls.Add(this.splitTop);
            // 
            // splitRight.Panel2
            // 
            this.splitRight.Panel2.Controls.Add(this.tabRight);
            this.splitRight.Panel2MinSize = 1;
            this.splitRight.Size = new System.Drawing.Size(458, 302);
            this.splitRight.SplitterDistance = 386;
            this.splitRight.TabIndex = 0;
            // 
            // splitTop
            // 
            this.splitTop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitTop.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitTop.Location = new System.Drawing.Point(0, 0);
            this.splitTop.Name = "splitTop";
            this.splitTop.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitTop.Panel1
            // 
            this.splitTop.Panel1.Controls.Add(this.tabTop);
            this.splitTop.Panel1MinSize = 1;
            // 
            // splitTop.Panel2
            // 
            this.splitTop.Panel2.Controls.Add(this.splitBottom);
            this.splitTop.Size = new System.Drawing.Size(386, 302);
            this.splitTop.SplitterDistance = 60;
            this.splitTop.TabIndex = 0;
            // 
            // tabTop
            // 
            this.tabTop.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabTop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabTop.ImageList = this.imageListTop;
            this.tabTop.Location = new System.Drawing.Point(0, 0);
            this.tabTop.Name = "tabTop";
            this.tabTop.SelectedIndex = 0;
            this.tabTop.Size = new System.Drawing.Size(386, 60);
            this.tabTop.TabIndex = 0;
            this.tabTop.Click += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabTop.Enter += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabTop.SelectedIndexChanged += new System.EventHandler(this.tabToolWindow_SelectedIndexChanged);
            // 
            // imageListTop
            // 
            this.imageListTop.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageListTop.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListTop.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // splitBottom
            // 
            this.splitBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitBottom.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitBottom.Location = new System.Drawing.Point(0, 0);
            this.splitBottom.Name = "splitBottom";
            this.splitBottom.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitBottom.Panel1
            // 
            this.splitBottom.Panel1.Controls.Add(this.tabDocument);
            // 
            // splitBottom.Panel2
            // 
            this.splitBottom.Panel2.Controls.Add(this.tabBottom);
            this.splitBottom.Panel2MinSize = 1;
            this.splitBottom.Size = new System.Drawing.Size(386, 238);
            this.splitBottom.SplitterDistance = 185;
            this.splitBottom.TabIndex = 0;
            // 
            // tabDocument
            // 
            this.tabDocument.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabDocument.ImageList = this.imageListDocument;
            this.tabDocument.Location = new System.Drawing.Point(0, 0);
            this.tabDocument.Name = "tabDocument";
            this.tabDocument.SelectedIndex = 0;
            this.tabDocument.Size = new System.Drawing.Size(386, 185);
            this.tabDocument.TabIndex = 0;
            this.tabDocument.Click += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabDocument.Enter += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabDocument.SelectedIndexChanged += new System.EventHandler(this.tabToolWindow_SelectedIndexChanged);
            // 
            // imageListDocument
            // 
            this.imageListDocument.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageListDocument.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListDocument.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // tabBottom
            // 
            this.tabBottom.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabBottom.ImageList = this.imageListBottom;
            this.tabBottom.Location = new System.Drawing.Point(0, 0);
            this.tabBottom.Name = "tabBottom";
            this.tabBottom.SelectedIndex = 0;
            this.tabBottom.Size = new System.Drawing.Size(386, 49);
            this.tabBottom.TabIndex = 0;
            this.tabBottom.Click += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabBottom.Enter += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabBottom.SelectedIndexChanged += new System.EventHandler(this.tabToolWindow_SelectedIndexChanged);
            // 
            // imageListBottom
            // 
            this.imageListBottom.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageListBottom.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListBottom.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // tabRight
            // 
            this.tabRight.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabRight.ImageList = this.imageListRight;
            this.tabRight.Location = new System.Drawing.Point(0, 0);
            this.tabRight.Multiline = true;
            this.tabRight.Name = "tabRight";
            this.tabRight.SelectedIndex = 0;
            this.tabRight.Size = new System.Drawing.Size(68, 302);
            this.tabRight.TabIndex = 0;
            this.tabRight.Click += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabRight.Enter += new System.EventHandler(this.tabToolWindow_Enter);
            this.tabRight.SelectedIndexChanged += new System.EventHandler(this.tabToolWindow_SelectedIndexChanged);
            // 
            // imageListRight
            // 
            this.imageListRight.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageListRight.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListRight.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // menuStripMain
            // 
            this.menuStripMain.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStripMain.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(526, 4);
            this.menuStripMain.TabIndex = 0;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(526, 328);
            this.Controls.Add(this.toolStripContainer);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Vczh AutoShell 2010";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.toolStripContainer.BottomToolStripPanel.ResumeLayout(false);
            this.toolStripContainer.BottomToolStripPanel.PerformLayout();
            this.toolStripContainer.ContentPanel.ResumeLayout(false);
            this.toolStripContainer.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer.TopToolStripPanel.PerformLayout();
            this.toolStripContainer.ResumeLayout(false);
            this.toolStripContainer.PerformLayout();
            this.splitLeft.Panel1.ResumeLayout(false);
            this.splitLeft.Panel2.ResumeLayout(false);
            this.splitLeft.ResumeLayout(false);
            this.splitRight.Panel1.ResumeLayout(false);
            this.splitRight.Panel2.ResumeLayout(false);
            this.splitRight.ResumeLayout(false);
            this.splitTop.Panel1.ResumeLayout(false);
            this.splitTop.Panel2.ResumeLayout(false);
            this.splitTop.ResumeLayout(false);
            this.splitBottom.Panel1.ResumeLayout(false);
            this.splitBottom.Panel2.ResumeLayout(false);
            this.splitBottom.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer;
        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.SplitContainer splitLeft;
        private System.Windows.Forms.SplitContainer splitRight;
        private System.Windows.Forms.SplitContainer splitTop;
        private System.Windows.Forms.SplitContainer splitBottom;
        private System.Windows.Forms.TabControl tabLeft;
        private System.Windows.Forms.TabControl tabTop;
        private System.Windows.Forms.TabControl tabBottom;
        private System.Windows.Forms.TabControl tabRight;
        private System.Windows.Forms.TabControl tabDocument;
        private System.Windows.Forms.ImageList imageListLeft;
        private System.Windows.Forms.ImageList imageListTop;
        private System.Windows.Forms.ImageList imageListRight;
        private System.Windows.Forms.ImageList imageListBottom;
        private System.Windows.Forms.ImageList imageListDocument;
        private System.Windows.Forms.StatusStrip statusStripMain;

    }
}