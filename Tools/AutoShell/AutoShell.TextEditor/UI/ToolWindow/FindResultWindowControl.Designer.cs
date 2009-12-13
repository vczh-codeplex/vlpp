namespace AutoShell.UI.ToolWindow
{
    partial class FindResultWindowControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FindResultWindowControl));
            this.listViewFindResult = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // listViewFindResult
            // 
            this.listViewFindResult.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            resources.ApplyResources(this.listViewFindResult, "listViewFindResult");
            this.listViewFindResult.FullRowSelect = true;
            this.listViewFindResult.GridLines = true;
            this.listViewFindResult.HideSelection = false;
            this.listViewFindResult.MultiSelect = false;
            this.listViewFindResult.Name = "listViewFindResult";
            this.listViewFindResult.UseCompatibleStateImageBehavior = false;
            this.listViewFindResult.View = System.Windows.Forms.View.Details;
            this.listViewFindResult.SelectedIndexChanged += new System.EventHandler(this.listViewFindResult_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            resources.ApplyResources(this.columnHeader1, "columnHeader1");
            // 
            // columnHeader2
            // 
            resources.ApplyResources(this.columnHeader2, "columnHeader2");
            // 
            // FindResultWindowControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.listViewFindResult);
            this.Name = "FindResultWindowControl";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewFindResult;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
    }
}
