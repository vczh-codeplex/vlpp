namespace TokenizerBuilder
{
    partial class BuilderForm
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
            this.panelContainer = new System.Windows.Forms.Panel();
            this.tokenizerEditorBox = new TokenizerBuilder.TokenizerEditorBox();
            this.panelContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelContainer
            // 
            this.panelContainer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelContainer.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelContainer.Controls.Add(this.tokenizerEditorBox);
            this.panelContainer.Location = new System.Drawing.Point(12, 12);
            this.panelContainer.Name = "panelContainer";
            this.panelContainer.Size = new System.Drawing.Size(616, 524);
            this.panelContainer.TabIndex = 0;
            // 
            // tokenizerEditorBox
            // 
            this.tokenizerEditorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tokenizerEditorBox.HorizontalSmallChange = 1;
            this.tokenizerEditorBox.Location = new System.Drawing.Point(0, 0);
            this.tokenizerEditorBox.Name = "tokenizerEditorBox";
            this.tokenizerEditorBox.Size = new System.Drawing.Size(614, 522);
            this.tokenizerEditorBox.TabIndex = 0;
            this.tokenizerEditorBox.VerticalSmallChange = 1;
            // 
            // BuilderForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(640, 548);
            this.Controls.Add(this.panelContainer);
            this.Name = "BuilderForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Tokenizer Builder v1.0 (vczh)";
            this.panelContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelContainer;
        private TokenizerEditorBox tokenizerEditorBox;
    }
}

