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
            this.splitEditor = new System.Windows.Forms.SplitContainer();
            this.textBoxOutput = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitEditor)).BeginInit();
            this.splitEditor.Panel1.SuspendLayout();
            this.splitEditor.Panel2.SuspendLayout();
            this.splitEditor.SuspendLayout();
            this.SuspendLayout();
            // 
            // codeEditorNativeX
            // 
            this.codeEditorNativeX.BackColor = System.Drawing.SystemColors.Window;
            this.codeEditorNativeX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.codeEditorNativeX.EnableDefaultCommands = true;
            this.codeEditorNativeX.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.codeEditorNativeX.Location = new System.Drawing.Point(0, 0);
            this.codeEditorNativeX.Name = "codeEditorNativeX";
            this.codeEditorNativeX.PressingChar = false;
            this.codeEditorNativeX.Size = new System.Drawing.Size(374, 214);
            this.codeEditorNativeX.TabIndex = 0;
            this.codeEditorNativeX.TabSpaceCount = 4;
            this.codeEditorNativeX.SelectionChanged += new System.EventHandler(this.codeEditorNativeX_SelectionChanged);
            // 
            // splitEditor
            // 
            this.splitEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitEditor.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitEditor.Location = new System.Drawing.Point(0, 0);
            this.splitEditor.Name = "splitEditor";
            this.splitEditor.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitEditor.Panel1
            // 
            this.splitEditor.Panel1.Controls.Add(this.codeEditorNativeX);
            // 
            // splitEditor.Panel2
            // 
            this.splitEditor.Panel2.Controls.Add(this.textBoxOutput);
            this.splitEditor.Size = new System.Drawing.Size(374, 352);
            this.splitEditor.SplitterDistance = 214;
            this.splitEditor.TabIndex = 1;
            // 
            // textBoxOutput
            // 
            this.textBoxOutput.BackColor = System.Drawing.SystemColors.Window;
            this.textBoxOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxOutput.Location = new System.Drawing.Point(0, 0);
            this.textBoxOutput.Multiline = true;
            this.textBoxOutput.Name = "textBoxOutput";
            this.textBoxOutput.ReadOnly = true;
            this.textBoxOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxOutput.Size = new System.Drawing.Size(374, 134);
            this.textBoxOutput.TabIndex = 0;
            // 
            // TurtleIdeFormContent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.splitEditor);
            this.Name = "TurtleIdeFormContent";
            this.Size = new System.Drawing.Size(374, 352);
            this.splitEditor.Panel1.ResumeLayout(false);
            this.splitEditor.Panel2.ResumeLayout(false);
            this.splitEditor.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitEditor)).EndInit();
            this.splitEditor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private EditorControls.NativeX.NativeXCodeEditor codeEditorNativeX;
        private System.Windows.Forms.SplitContainer splitEditor;
        private System.Windows.Forms.TextBox textBoxOutput;
    }
}
