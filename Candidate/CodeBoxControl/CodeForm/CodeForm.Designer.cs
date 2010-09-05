namespace CodeForm
{
    partial class CodeForm
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
            this.textEditorBox = new CodeBoxControl.TextEditorBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textEditorBox
            // 
            this.textEditorBox.BackColor = System.Drawing.Color.Honeydew;
            this.textEditorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textEditorBox.EnableDefaultCommands = true;
            this.textEditorBox.Font = new System.Drawing.Font("SimSun", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textEditorBox.ForeColor = System.Drawing.SystemColors.ControlText;
            this.textEditorBox.Location = new System.Drawing.Point(0, 0);
            this.textEditorBox.Name = "textEditorBox";
            this.textEditorBox.SelectedText = "";
            this.textEditorBox.Size = new System.Drawing.Size(346, 348);
            this.textEditorBox.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.textEditorBox);
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(348, 350);
            this.panel1.TabIndex = 1;
            // 
            // CodeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(372, 374);
            this.Controls.Add(this.panel1);
            this.Name = "CodeForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CodeForm";
            this.Shown += new System.EventHandler(this.CodeForm_Shown);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private CodeBoxControl.TextEditorBox textEditorBox;
        private System.Windows.Forms.Panel panel1;

    }
}

