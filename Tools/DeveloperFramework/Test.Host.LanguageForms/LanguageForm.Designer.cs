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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tabEditor.SuspendLayout();
            this.tabPageEditor.SuspendLayout();
            this.panelEditor.SuspendLayout();
            this.tabPageTree.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
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
            this.textEditorBox.Size = new System.Drawing.Size(707, 656);
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
            this.tabEditor.SelectedIndex = 0;
            this.tabEditor.Size = new System.Drawing.Size(723, 690);
            this.tabEditor.TabIndex = 1;
            // 
            // tabPageEditor
            // 
            this.tabPageEditor.Controls.Add(this.panelEditor);
            this.tabPageEditor.Location = new System.Drawing.Point(4, 22);
            this.tabPageEditor.Name = "tabPageEditor";
            this.tabPageEditor.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageEditor.Size = new System.Drawing.Size(715, 664);
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
            this.panelEditor.Size = new System.Drawing.Size(709, 658);
            this.panelEditor.TabIndex = 0;
            // 
            // tabPageTree
            // 
            this.tabPageTree.Controls.Add(this.textTree);
            this.tabPageTree.Location = new System.Drawing.Point(4, 22);
            this.tabPageTree.Name = "tabPageTree";
            this.tabPageTree.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTree.Size = new System.Drawing.Size(599, 527);
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
            this.textTree.Size = new System.Drawing.Size(593, 547);
            this.textTree.TabIndex = 0;
            // 
            // textStatus
            // 
            this.textStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textStatus.Location = new System.Drawing.Point(3, 699);
            this.textStatus.Name = "textStatus";
            this.textStatus.ReadOnly = true;
            this.textStatus.Size = new System.Drawing.Size(723, 20);
            this.textStatus.TabIndex = 2;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.textStatus, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tabEditor, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(729, 722);
            this.tableLayoutPanel1.TabIndex = 3;
            // 
            // LanguageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(729, 722);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "LanguageForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "LanguageForm";
            this.tabEditor.ResumeLayout(false);
            this.tabPageEditor.ResumeLayout(false);
            this.panelEditor.ResumeLayout(false);
            this.tabPageTree.ResumeLayout(false);
            this.tabPageTree.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
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
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;

    }
}

