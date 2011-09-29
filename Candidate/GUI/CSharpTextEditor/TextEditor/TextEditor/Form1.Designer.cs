namespace TextEditor
{
    partial class Form1
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
            this.scrollableContentControl = new Developer.WinFormControls.ScrollableContentControl();
            this.SuspendLayout();
            // 
            // scrollableContentControl
            // 
            this.scrollableContentControl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.scrollableContentControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scrollableContentControl.Location = new System.Drawing.Point(0, 0);
            this.scrollableContentControl.Name = "scrollableContentControl";
            this.scrollableContentControl.ScrollableContent = null;
            this.scrollableContentControl.Size = new System.Drawing.Size(713, 524);
            this.scrollableContentControl.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(713, 524);
            this.Controls.Add(this.scrollableContentControl);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private Developer.WinFormControls.ScrollableContentControl scrollableContentControl;
    }
}

