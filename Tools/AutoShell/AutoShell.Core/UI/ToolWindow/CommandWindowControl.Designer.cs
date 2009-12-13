namespace AutoShell.UI.ToolWindow
{
    partial class CommandWindowControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textOutput = new System.Windows.Forms.TextBox();
            this.textCommand = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textOutput
            // 
            this.textOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textOutput.Location = new System.Drawing.Point(0, 0);
            this.textOutput.Multiline = true;
            this.textOutput.Name = "textOutput";
            this.textOutput.ReadOnly = true;
            this.textOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textOutput.Size = new System.Drawing.Size(363, 206);
            this.textOutput.TabIndex = 1;
            this.textOutput.WordWrap = false;
            // 
            // textCommand
            // 
            this.textCommand.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textCommand.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.textCommand.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
            this.textCommand.Location = new System.Drawing.Point(0, 210);
            this.textCommand.Name = "textCommand";
            this.textCommand.Size = new System.Drawing.Size(363, 21);
            this.textCommand.TabIndex = 0;
            this.textCommand.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textCommand_KeyDown);
            // 
            // CommandWindowControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.textCommand);
            this.Controls.Add(this.textOutput);
            this.Name = "CommandWindowControl";
            this.Size = new System.Drawing.Size(363, 231);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textOutput;
        private System.Windows.Forms.TextBox textCommand;
    }
}
