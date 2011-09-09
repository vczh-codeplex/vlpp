namespace ImageDisplayer
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
            this.panelImage = new System.Windows.Forms.Panel();
            this.pictureBoxImage = new System.Windows.Forms.PictureBox();
            this.buttonPaste = new System.Windows.Forms.Button();
            this.comboBoxImage = new System.Windows.Forms.ComboBox();
            this.panelImage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxImage)).BeginInit();
            this.SuspendLayout();
            // 
            // panelImage
            // 
            this.panelImage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelImage.AutoScroll = true;
            this.panelImage.Controls.Add(this.pictureBoxImage);
            this.panelImage.Location = new System.Drawing.Point(12, 44);
            this.panelImage.Name = "panelImage";
            this.panelImage.Size = new System.Drawing.Size(628, 442);
            this.panelImage.TabIndex = 0;
            // 
            // pictureBoxImage
            // 
            this.pictureBoxImage.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxImage.Name = "pictureBoxImage";
            this.pictureBoxImage.Size = new System.Drawing.Size(32, 30);
            this.pictureBoxImage.TabIndex = 2;
            this.pictureBoxImage.TabStop = false;
            // 
            // buttonPaste
            // 
            this.buttonPaste.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPaste.Location = new System.Drawing.Point(565, 11);
            this.buttonPaste.Name = "buttonPaste";
            this.buttonPaste.Size = new System.Drawing.Size(75, 21);
            this.buttonPaste.TabIndex = 2;
            this.buttonPaste.Text = "Paste";
            this.buttonPaste.UseVisualStyleBackColor = true;
            this.buttonPaste.Click += new System.EventHandler(this.buttonPaste_Click);
            // 
            // comboBoxImage
            // 
            this.comboBoxImage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxImage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxImage.FormattingEnabled = true;
            this.comboBoxImage.Location = new System.Drawing.Point(12, 12);
            this.comboBoxImage.Name = "comboBoxImage";
            this.comboBoxImage.Size = new System.Drawing.Size(547, 20);
            this.comboBoxImage.TabIndex = 3;
            this.comboBoxImage.SelectedIndexChanged += new System.EventHandler(this.comboBoxImage_SelectedIndexChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(652, 498);
            this.Controls.Add(this.comboBoxImage);
            this.Controls.Add(this.buttonPaste);
            this.Controls.Add(this.panelImage);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ImageDisplayer";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.panelImage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxImage)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelImage;
        private System.Windows.Forms.Button buttonPaste;
        private System.Windows.Forms.PictureBox pictureBoxImage;
        private System.Windows.Forms.ComboBox comboBoxImage;
    }
}

