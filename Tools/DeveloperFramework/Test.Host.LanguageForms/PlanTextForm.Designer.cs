namespace Test.Host.LanguageForms
{
    partial class PlanTextForm
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
            this.panelEditor = new System.Windows.Forms.Panel();
            this.textEditorBox = new Developer.WinFormControls.TextEditorBox();
            this.panelEditor.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelEditor
            // 
            this.panelEditor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelEditor.Controls.Add(this.textEditorBox);
            this.panelEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEditor.Location = new System.Drawing.Point(0, 0);
            this.panelEditor.Name = "panelEditor";
            this.panelEditor.Size = new System.Drawing.Size(656, 666);
            this.panelEditor.TabIndex = 1;
            // 
            // textEditorBox
            // 
            this.textEditorBox.BackColor = System.Drawing.SystemColors.Window;
            this.textEditorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textEditorBox.EnableDefaultCommands = true;
            this.textEditorBox.ForeColor = System.Drawing.SystemColors.ControlText;
            this.textEditorBox.HorizontalSmallChange = 7;
            this.textEditorBox.Location = new System.Drawing.Point(0, 0);
            this.textEditorBox.Name = "textEditorBox";
            this.textEditorBox.SelectedText = "";
            this.textEditorBox.Size = new System.Drawing.Size(654, 664);
            this.textEditorBox.TabIndex = 0;
            this.textEditorBox.VerticalSmallChange = 10;
            // 
            // PlanTextForm
            // 
            this.ClientSize = new System.Drawing.Size(656, 666);
            this.Controls.Add(this.panelEditor);
            this.Name = "PlanTextForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Plan Text Form";
            this.panelEditor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelEditor;
        public Developer.WinFormControls.TextEditorBox textEditorBox;
    }
}
