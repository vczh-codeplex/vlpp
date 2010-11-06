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
            this.tabEditor = new System.Windows.Forms.TabControl();
            this.tabPageEditor = new System.Windows.Forms.TabPage();
            this.panelEditor = new System.Windows.Forms.Panel();
            this.tabPageTree = new System.Windows.Forms.TabPage();
            this.textTree = new System.Windows.Forms.TextBox();
            this.textStatus = new System.Windows.Forms.TextBox();
            this.tableEditor = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.textContext = new System.Windows.Forms.TextBox();
            this.tabEditor.SuspendLayout();
            this.tabPageEditor.SuspendLayout();
            this.panelEditor.SuspendLayout();
            this.tabPageTree.SuspendLayout();
            this.tableEditor.SuspendLayout();
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
            this.textEditorBox.Size = new System.Drawing.Size(679, 599);
            this.textEditorBox.TabIndex = 0;
            this.textEditorBox.VerticalSmallChange = 10;
            // 
            // tabEditor
            // 
            this.tabEditor.Controls.Add(this.tabPageEditor);
            this.tabEditor.Controls.Add(this.tabPageTree);
            this.tabEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabEditor.Location = new System.Drawing.Point(3, 3);
            this.tabEditor.Name = "tabEditor";
            this.tableEditor.SetRowSpan(this.tabEditor, 2);
            this.tabEditor.SelectedIndex = 0;
            this.tabEditor.Size = new System.Drawing.Size(695, 633);
            this.tabEditor.TabIndex = 1;
            // 
            // tabPageEditor
            // 
            this.tabPageEditor.Controls.Add(this.panelEditor);
            this.tabPageEditor.Location = new System.Drawing.Point(4, 22);
            this.tabPageEditor.Name = "tabPageEditor";
            this.tabPageEditor.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageEditor.Size = new System.Drawing.Size(687, 607);
            this.tabPageEditor.TabIndex = 0;
            this.tabPageEditor.Text = "Editor";
            this.tabPageEditor.UseVisualStyleBackColor = true;
            // 
            // panelEditor
            // 
            this.panelEditor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelEditor.Controls.Add(this.textEditorBox);
            this.panelEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEditor.Location = new System.Drawing.Point(3, 3);
            this.panelEditor.Name = "panelEditor";
            this.panelEditor.Size = new System.Drawing.Size(681, 601);
            this.panelEditor.TabIndex = 0;
            // 
            // tabPageTree
            // 
            this.tabPageTree.Controls.Add(this.textTree);
            this.tabPageTree.Location = new System.Drawing.Point(4, 22);
            this.tabPageTree.Name = "tabPageTree";
            this.tabPageTree.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTree.Size = new System.Drawing.Size(687, 607);
            this.tabPageTree.TabIndex = 1;
            this.tabPageTree.Text = "Tree";
            this.tabPageTree.UseVisualStyleBackColor = true;
            // 
            // textTree
            // 
            this.textTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textTree.Location = new System.Drawing.Point(3, 3);
            this.textTree.Multiline = true;
            this.textTree.Name = "textTree";
            this.textTree.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textTree.Size = new System.Drawing.Size(681, 601);
            this.textTree.TabIndex = 0;
            this.textTree.WordWrap = false;
            // 
            // textStatus
            // 
            this.tableEditor.SetColumnSpan(this.textStatus, 2);
            this.textStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textStatus.Location = new System.Drawing.Point(3, 642);
            this.textStatus.Name = "textStatus";
            this.textStatus.ReadOnly = true;
            this.textStatus.Size = new System.Drawing.Size(1039, 21);
            this.textStatus.TabIndex = 2;
            // 
            // tableEditor
            // 
            this.tableEditor.ColumnCount = 2;
            this.tableEditor.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableEditor.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 344F));
            this.tableEditor.Controls.Add(this.textStatus, 0, 2);
            this.tableEditor.Controls.Add(this.tabEditor, 0, 0);
            this.tableEditor.Controls.Add(this.label1, 1, 0);
            this.tableEditor.Controls.Add(this.textContext, 1, 1);
            this.tableEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableEditor.Location = new System.Drawing.Point(0, 0);
            this.tableEditor.Name = "tableEditor";
            this.tableEditor.RowCount = 3;
            this.tableEditor.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableEditor.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableEditor.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableEditor.Size = new System.Drawing.Size(1045, 666);
            this.tableEditor.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(704, 3);
            this.label1.Margin = new System.Windows.Forms.Padding(3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Context Text";
            // 
            // textContext
            // 
            this.textContext.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textContext.Location = new System.Drawing.Point(704, 21);
            this.textContext.Multiline = true;
            this.textContext.Name = "textContext";
            this.textContext.ReadOnly = true;
            this.textContext.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textContext.Size = new System.Drawing.Size(338, 615);
            this.textContext.TabIndex = 4;
            this.textContext.WordWrap = false;
            // 
            // LanguageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1045, 666);
            this.Controls.Add(this.tableEditor);
            this.Name = "LanguageForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "LanguageForm";
            this.tabEditor.ResumeLayout(false);
            this.tabPageEditor.ResumeLayout(false);
            this.panelEditor.ResumeLayout(false);
            this.tabPageTree.ResumeLayout(false);
            this.tabPageTree.PerformLayout();
            this.tableEditor.ResumeLayout(false);
            this.tableEditor.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        public Developer.WinFormControls.TextEditorBox textEditorBox;
        private System.Windows.Forms.TabControl tabEditor;
        private System.Windows.Forms.TabPage tabPageEditor;
        private System.Windows.Forms.Panel panelEditor;
        private System.Windows.Forms.TabPage tabPageTree;
        private System.Windows.Forms.TextBox textTree;
        private System.Windows.Forms.TextBox textStatus;
        private System.Windows.Forms.TableLayoutPanel tableEditor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textContext;

    }
}

