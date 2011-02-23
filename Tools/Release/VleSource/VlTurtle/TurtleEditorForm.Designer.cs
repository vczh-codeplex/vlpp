namespace VlTurtle
{
    partial class TurtleEditorForm
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
            this.codeEditorNativeX = new VlTurtle.EditorControls.NativeX.NativeXCodeEditor();
            this.panelEditor = new System.Windows.Forms.Panel();
            this.panelEditor.SuspendLayout();
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
            this.codeEditorNativeX.Size = new System.Drawing.Size(492, 479);
            this.codeEditorNativeX.TabIndex = 0;
            this.codeEditorNativeX.TabSpaceCount = 4;
            // 
            // panelEditor
            // 
            this.panelEditor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelEditor.Controls.Add(this.codeEditorNativeX);
            this.panelEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEditor.Location = new System.Drawing.Point(0, 0);
            this.panelEditor.Name = "panelEditor";
            this.panelEditor.Size = new System.Drawing.Size(494, 481);
            this.panelEditor.TabIndex = 1;
            // 
            // TurtleEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 481);
            this.Controls.Add(this.panelEditor);
            this.Name = "TurtleEditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Turtle Editor(Language: NativeX)";
            this.panelEditor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private EditorControls.NativeX.NativeXCodeEditor codeEditorNativeX;
        private System.Windows.Forms.Panel panelEditor;
    }
}