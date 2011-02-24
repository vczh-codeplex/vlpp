namespace VlTurtle
{
    partial class TurtleIdeFormContent
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
            this.codeEditorNativeX = new VlTurtle.EditorControls.NativeX.NativeXCodeEditor();
            this.SuspendLayout();
            // 
            // codeEditorNativeX
            // 
            this.codeEditorNativeX.BackColor = System.Drawing.SystemColors.Window;
            this.codeEditorNativeX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.codeEditorNativeX.EnableDefaultCommands = true;
            this.codeEditorNativeX.Location = new System.Drawing.Point(0, 0);
            this.codeEditorNativeX.Name = "codeEditorNativeX";
            this.codeEditorNativeX.PressingChar = false;
            this.codeEditorNativeX.Size = new System.Drawing.Size(374, 352);
            this.codeEditorNativeX.TabIndex = 0;
            this.codeEditorNativeX.TabSpaceCount = 4;
            // 
            // TurtleIdeFormContent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.codeEditorNativeX);
            this.Name = "TurtleIdeFormContent";
            this.Size = new System.Drawing.Size(374, 352);
            this.ResumeLayout(false);

        }

        #endregion

        private EditorControls.NativeX.NativeXCodeEditor codeEditorNativeX;
    }
}
