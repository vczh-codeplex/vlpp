namespace AutoShell.UI.Option
{
    partial class FindOptionEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FindOptionEditor));
            this.checkBoxMaintain = new System.Windows.Forms.CheckBox();
            this.checkBoxClose = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // checkBoxMaintain
            // 
            resources.ApplyResources(this.checkBoxMaintain, "checkBoxMaintain");
            this.checkBoxMaintain.Name = "checkBoxMaintain";
            this.checkBoxMaintain.UseVisualStyleBackColor = true;
            // 
            // checkBoxClose
            // 
            resources.ApplyResources(this.checkBoxClose, "checkBoxClose");
            this.checkBoxClose.Name = "checkBoxClose";
            this.checkBoxClose.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // FindOptionEditor
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.checkBoxClose);
            this.Controls.Add(this.checkBoxMaintain);
            this.Name = "FindOptionEditor";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxMaintain;
        private System.Windows.Forms.CheckBox checkBoxClose;
        private System.Windows.Forms.Label label1;
    }
}
