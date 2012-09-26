namespace Chess
{
    partial class ChessForm
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
            this.panelChess = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // panelChess
            // 
            this.panelChess.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelChess.Location = new System.Drawing.Point(12, 12);
            this.panelChess.Name = "panelChess";
            this.panelChess.Size = new System.Drawing.Size(1025, 513);
            this.panelChess.TabIndex = 0;
            this.panelChess.Paint += new System.Windows.Forms.PaintEventHandler(this.panelChess_Paint);
            this.panelChess.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panelChess_MouseDown);
            this.panelChess.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panelChess_MouseMove);
            this.panelChess.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panelChess_MouseUp);
            // 
            // ChessForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1049, 537);
            this.Controls.Add(this.panelChess);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "ChessForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Chess";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelChess;
    }
}

