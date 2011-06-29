namespace SimulatorViewer
{
    partial class SimForm
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
            // SimForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SimForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Vczh Simulator 3.0";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SimForm_FormClosed);
            this.Load += new System.EventHandler(this.SimForm_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.SimForm_Paint);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.SimForm_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.SimForm_KeyUp);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.SimForm_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.SimForm_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.SimForm_MouseUp);
            this.ResumeLayout(false);

        }

        #endregion
    }
}

