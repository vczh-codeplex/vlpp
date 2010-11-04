namespace Test.Host.LanguageForms
{
    partial class LanguageForm
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
            Developer.WinFormControls.TextEditorControlPanel textEditorControlPanel1 = new Developer.WinFormControls.TextEditorControlPanel();
            this.textEditorBox = new Developer.WinFormControls.TextEditorBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textEditorBox
            // 
            this.textEditorBox.BackColor = System.Drawing.SystemColors.Window;
            this.textEditorBox.ControlPanel = textEditorControlPanel1;
            this.textEditorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textEditorBox.EnableDefaultCommands = true;
            this.textEditorBox.Font = new System.Drawing.Font("SimSun", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textEditorBox.ForeColor = System.Drawing.SystemColors.ControlText;
            this.textEditorBox.HorizontalSmallChange = 7;
            this.textEditorBox.Location = new System.Drawing.Point(0, 0);
            this.textEditorBox.Name = "textEditorBox";
            this.textEditorBox.SelectedText = "";
            this.textEditorBox.Size = new System.Drawing.Size(672, 646);
            this.textEditorBox.TabIndex = 0;
            this.textEditorBox.VerticalSmallChange = 10;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.textEditorBox);
            this.panel1.Location = new System.Drawing.Point(12, 13);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(674, 648);
            this.panel1.TabIndex = 1;
            // 
            // LanguageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(698, 674);
            this.Controls.Add(this.panel1);
            this.Name = "LanguageForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "LanguageForm";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        public Developer.WinFormControls.TextEditorBox textEditorBox;

    }
}

