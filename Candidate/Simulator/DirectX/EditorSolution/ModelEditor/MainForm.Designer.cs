namespace ModelEditor
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
            this.panelEditorWindow = new System.Windows.Forms.Panel();
            this.timerRenderer = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cSelectionModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tTranslationModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rRotationModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sScalingModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.gGlobalAxisToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lLocalAxisToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.modelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.geometryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cubeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cylnderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.coneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelEditorWindow
            // 
            this.panelEditorWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelEditorWindow.Location = new System.Drawing.Point(12, 30);
            this.panelEditorWindow.Name = "panelEditorWindow";
            this.panelEditorWindow.Size = new System.Drawing.Size(905, 684);
            this.panelEditorWindow.TabIndex = 0;
            // 
            // timerRenderer
            // 
            this.timerRenderer.Enabled = true;
            this.timerRenderer.Tick += new System.EventHandler(this.timerRenderer_Tick);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.modelToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(929, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cSelectionModeToolStripMenuItem,
            this.tTranslationModeToolStripMenuItem,
            this.rRotationModeToolStripMenuItem,
            this.sScalingModeToolStripMenuItem,
            this.toolStripMenuItem1,
            this.gGlobalAxisToolStripMenuItem,
            this.lLocalAxisToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // cSelectionModeToolStripMenuItem
            // 
            this.cSelectionModeToolStripMenuItem.Name = "cSelectionModeToolStripMenuItem";
            this.cSelectionModeToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.cSelectionModeToolStripMenuItem.Text = "[&C] Selection Mode";
            this.cSelectionModeToolStripMenuItem.Click += new System.EventHandler(this.cSelectionModeToolStripMenuItem_Click);
            // 
            // tTranslationModeToolStripMenuItem
            // 
            this.tTranslationModeToolStripMenuItem.Name = "tTranslationModeToolStripMenuItem";
            this.tTranslationModeToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.tTranslationModeToolStripMenuItem.Text = "[&T] Translation Mode";
            this.tTranslationModeToolStripMenuItem.Click += new System.EventHandler(this.tTranslationModeToolStripMenuItem_Click);
            // 
            // rRotationModeToolStripMenuItem
            // 
            this.rRotationModeToolStripMenuItem.Name = "rRotationModeToolStripMenuItem";
            this.rRotationModeToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.rRotationModeToolStripMenuItem.Text = "[&R] Rotation Mode";
            this.rRotationModeToolStripMenuItem.Click += new System.EventHandler(this.rRotationModeToolStripMenuItem_Click);
            // 
            // sScalingModeToolStripMenuItem
            // 
            this.sScalingModeToolStripMenuItem.Name = "sScalingModeToolStripMenuItem";
            this.sScalingModeToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.sScalingModeToolStripMenuItem.Text = "[S] Scaling Mode";
            this.sScalingModeToolStripMenuItem.Click += new System.EventHandler(this.sScalingModeToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(182, 6);
            // 
            // gGlobalAxisToolStripMenuItem
            // 
            this.gGlobalAxisToolStripMenuItem.Name = "gGlobalAxisToolStripMenuItem";
            this.gGlobalAxisToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.gGlobalAxisToolStripMenuItem.Text = "[&G] Global Axis";
            this.gGlobalAxisToolStripMenuItem.Click += new System.EventHandler(this.gGlobalAxisToolStripMenuItem_Click);
            // 
            // lLocalAxisToolStripMenuItem
            // 
            this.lLocalAxisToolStripMenuItem.Name = "lLocalAxisToolStripMenuItem";
            this.lLocalAxisToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.lLocalAxisToolStripMenuItem.Text = "[&L] Local Axis";
            this.lLocalAxisToolStripMenuItem.Click += new System.EventHandler(this.lLocalAxisToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.resetToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // resetToolStripMenuItem
            // 
            this.resetToolStripMenuItem.Name = "resetToolStripMenuItem";
            this.resetToolStripMenuItem.Size = new System.Drawing.Size(102, 22);
            this.resetToolStripMenuItem.Text = "&Reset";
            this.resetToolStripMenuItem.Click += new System.EventHandler(this.resetToolStripMenuItem_Click);
            // 
            // modelToolStripMenuItem
            // 
            this.modelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.geometryToolStripMenuItem});
            this.modelToolStripMenuItem.Name = "modelToolStripMenuItem";
            this.modelToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.modelToolStripMenuItem.Text = "&Model";
            // 
            // geometryToolStripMenuItem
            // 
            this.geometryToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cubeToolStripMenuItem,
            this.sphereToolStripMenuItem,
            this.cylnderToolStripMenuItem,
            this.coneToolStripMenuItem});
            this.geometryToolStripMenuItem.Name = "geometryToolStripMenuItem";
            this.geometryToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.geometryToolStripMenuItem.Text = "&Geometry";
            // 
            // cubeToolStripMenuItem
            // 
            this.cubeToolStripMenuItem.Name = "cubeToolStripMenuItem";
            this.cubeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.cubeToolStripMenuItem.Text = "&Cube";
            this.cubeToolStripMenuItem.Click += new System.EventHandler(this.cubeToolStripMenuItem_Click);
            // 
            // sphereToolStripMenuItem
            // 
            this.sphereToolStripMenuItem.Name = "sphereToolStripMenuItem";
            this.sphereToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.sphereToolStripMenuItem.Text = "&Sphere...";
            this.sphereToolStripMenuItem.Click += new System.EventHandler(this.sphereToolStripMenuItem_Click);
            // 
            // cylnderToolStripMenuItem
            // 
            this.cylnderToolStripMenuItem.Name = "cylnderToolStripMenuItem";
            this.cylnderToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.cylnderToolStripMenuItem.Text = "C&ylnder...";
            this.cylnderToolStripMenuItem.Click += new System.EventHandler(this.cylnderToolStripMenuItem_Click);
            // 
            // coneToolStripMenuItem
            // 
            this.coneToolStripMenuItem.Name = "coneToolStripMenuItem";
            this.coneToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.coneToolStripMenuItem.Text = "C&one...";
            this.coneToolStripMenuItem.Click += new System.EventHandler(this.coneToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(929, 727);
            this.Controls.Add(this.panelEditorWindow);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Vczh Model Editor v1.0";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panelEditorWindow;
        private System.Windows.Forms.Timer timerRenderer;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem modelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cSelectionModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tTranslationModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rRotationModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sScalingModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem gGlobalAxisToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lLocalAxisToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem geometryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cubeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sphereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cylnderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem coneToolStripMenuItem;
    }
}

