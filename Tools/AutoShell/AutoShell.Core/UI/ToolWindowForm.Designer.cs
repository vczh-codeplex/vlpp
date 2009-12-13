namespace AutoShell.UI
{
    partial class ToolWindowForm
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
            this.SuspendLayout();
            // 
            // ToolWindowForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 264);
            this.ImeMode = System.Windows.Forms.ImeMode.On;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ToolWindowForm";
            this.ShowInTaskbar = false;
            this.Text = "ToolWindowForm";
            this.Deactivate += new System.EventHandler(this.ToolWindowForm_Deactivate);
            this.Activated += new System.EventHandler(this.ToolWindowForm_Activated);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ToolWindowForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion
    }
}