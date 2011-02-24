namespace VlTurtle
{
    partial class TurtleIdeForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TurtleIdeForm));
            this.toolStripRibbon = new System.Windows.Forms.ToolStrip();
            this.toolRibbonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolRibbonUndo = new System.Windows.Forms.ToolStripButton();
            this.toolRibbonRedo = new System.Windows.Forms.ToolStripButton();
            this.toolStripRibbon.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripRibbon
            // 
            this.toolStripRibbon.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStripRibbon.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolRibbonSave,
            this.toolStripSeparator1,
            this.toolRibbonUndo,
            this.toolRibbonRedo});
            this.toolStripRibbon.Location = new System.Drawing.Point(78, 0);
            this.toolStripRibbon.Name = "toolStripRibbon";
            this.toolStripRibbon.Size = new System.Drawing.Size(118, 25);
            this.toolStripRibbon.TabIndex = 2;
            this.toolStripRibbon.Text = "Ribbon Tool Bar";
            // 
            // toolRibbonSave
            // 
            this.toolRibbonSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolRibbonSave.Image = ((System.Drawing.Image)(resources.GetObject("toolRibbonSave.Image")));
            this.toolRibbonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolRibbonSave.Name = "toolRibbonSave";
            this.toolRibbonSave.Size = new System.Drawing.Size(23, 22);
            this.toolRibbonSave.Text = "Save";
            this.toolRibbonSave.Click += new System.EventHandler(this.toolRibbonSave_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolRibbonUndo
            // 
            this.toolRibbonUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolRibbonUndo.Image = ((System.Drawing.Image)(resources.GetObject("toolRibbonUndo.Image")));
            this.toolRibbonUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolRibbonUndo.Name = "toolRibbonUndo";
            this.toolRibbonUndo.Size = new System.Drawing.Size(23, 22);
            this.toolRibbonUndo.Text = "Undo";
            this.toolRibbonUndo.Click += new System.EventHandler(this.toolRibbonUndo_Click);
            // 
            // toolRibbonRedo
            // 
            this.toolRibbonRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolRibbonRedo.Image = ((System.Drawing.Image)(resources.GetObject("toolRibbonRedo.Image")));
            this.toolRibbonRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolRibbonRedo.Name = "toolRibbonRedo";
            this.toolRibbonRedo.Size = new System.Drawing.Size(23, 22);
            this.toolRibbonRedo.Text = "Redo";
            this.toolRibbonRedo.Click += new System.EventHandler(this.toolRibbonRedo_Click);
            // 
            // TurtleIdeForm
            // 
            this.AeroIcon = ((System.Drawing.Image)(resources.GetObject("$this.AeroIcon")));
            this.AeroIconSize = new System.Drawing.Size(48, 48);
            this.ClientSize = new System.Drawing.Size(484, 562);
            this.Controls.Add(this.toolStripRibbon);
            this.MinimumSize = new System.Drawing.Size(300, 400);
            this.Name = "TurtleIdeForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Turtle IDE";
            this.Controls.SetChildIndex(this.toolStripRibbon, 0);
            this.toolStripRibbon.ResumeLayout(false);
            this.toolStripRibbon.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStripRibbon;
        private System.Windows.Forms.ToolStripButton toolRibbonSave;
        private System.Windows.Forms.ToolStripButton toolRibbonUndo;
        private System.Windows.Forms.ToolStripButton toolRibbonRedo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    }
}
