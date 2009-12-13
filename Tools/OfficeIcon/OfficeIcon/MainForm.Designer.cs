namespace OfficeIcon
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
            this.contextMenuIcons = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.dialogSave = new System.Windows.Forms.SaveFileDialog();
            this.panelIcons = new System.Windows.Forms.Panel();
            this.contextMenuIcons.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuIcons
            // 
            this.contextMenuIcons.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextSaveAs});
            this.contextMenuIcons.Name = "contextMenuIcons";
            this.contextMenuIcons.Size = new System.Drawing.Size(131, 26);
            // 
            // contextSaveAs
            // 
            this.contextSaveAs.Name = "contextSaveAs";
            this.contextSaveAs.Size = new System.Drawing.Size(130, 22);
            this.contextSaveAs.Text = "Save As...";
            this.contextSaveAs.Click += new System.EventHandler(this.contextSaveAs_Click);
            // 
            // dialogSave
            // 
            this.dialogSave.DefaultExt = "png";
            this.dialogSave.Filter = "PNG Files(*.png)|*.png";
            this.dialogSave.Title = "Save Icon";
            // 
            // panelIcons
            // 
            this.panelIcons.ContextMenuStrip = this.contextMenuIcons;
            this.panelIcons.Location = new System.Drawing.Point(39, 61);
            this.panelIcons.Name = "panelIcons";
            this.panelIcons.Size = new System.Drawing.Size(200, 100);
            this.panelIcons.TabIndex = 1;
            this.panelIcons.Paint += new System.Windows.Forms.PaintEventHandler(this.panelIcons_Paint);
            this.panelIcons.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panelIcons_MouseMove);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(502, 377);
            this.Controls.Add(this.panelIcons);
            this.DoubleBuffered = true;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Office Icons";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.contextMenuIcons.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenuIcons;
        private System.Windows.Forms.ToolStripMenuItem contextSaveAs;
        private System.Windows.Forms.SaveFileDialog dialogSave;
        private System.Windows.Forms.Panel panelIcons;
    }
}

