namespace AutoShell.UI
{
    partial class EditingWindowsForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditingWindowsForm));
            this.buttonActive = new System.Windows.Forms.Button();
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.listViewWindows = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // buttonActive
            // 
            resources.ApplyResources(this.buttonActive, "buttonActive");
            this.buttonActive.Name = "buttonActive";
            this.buttonActive.UseVisualStyleBackColor = true;
            this.buttonActive.Click += new System.EventHandler(this.buttonActive_Click);
            // 
            // buttonSave
            // 
            resources.ApplyResources(this.buttonSave, "buttonSave");
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonClose
            // 
            resources.ApplyResources(this.buttonClose, "buttonClose");
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // listViewWindows
            // 
            resources.ApplyResources(this.listViewWindows, "listViewWindows");
            this.listViewWindows.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewWindows.FullRowSelect = true;
            this.listViewWindows.GridLines = true;
            this.listViewWindows.HideSelection = false;
            this.listViewWindows.Name = "listViewWindows";
            this.listViewWindows.UseCompatibleStateImageBehavior = false;
            this.listViewWindows.View = System.Windows.Forms.View.Details;
            this.listViewWindows.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            resources.ApplyResources(this.columnHeader1, "columnHeader1");
            // 
            // columnHeader2
            // 
            resources.ApplyResources(this.columnHeader2, "columnHeader2");
            // 
            // EditingWindowsForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.listViewWindows);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.buttonSave);
            this.Controls.Add(this.buttonActive);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EditingWindowsForm";
            this.ShowInTaskbar = false;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonActive;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.ListView listViewWindows;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
    }
}