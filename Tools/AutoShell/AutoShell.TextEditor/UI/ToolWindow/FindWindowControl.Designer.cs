namespace AutoShell.UI.ToolWindow
{
    partial class FindWindowControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FindWindowControl));
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxFind = new System.Windows.Forms.TextBox();
            this.textBoxReplace = new System.Windows.Forms.TextBox();
            this.checkBoxRegexReplace = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxTarget = new System.Windows.Forms.ComboBox();
            this.checkBoxCase = new System.Windows.Forms.CheckBox();
            this.checkBoxWord = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.comboBoxMethod = new System.Windows.Forms.ComboBox();
            this.buttonFind = new System.Windows.Forms.Button();
            this.checkBoxBackward = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // textBoxFind
            // 
            resources.ApplyResources(this.textBoxFind, "textBoxFind");
            this.textBoxFind.Name = "textBoxFind";
            this.textBoxFind.TextChanged += new System.EventHandler(this.textBoxFind_TextChanged);
            // 
            // textBoxReplace
            // 
            resources.ApplyResources(this.textBoxReplace, "textBoxReplace");
            this.textBoxReplace.Name = "textBoxReplace";
            // 
            // checkBoxRegexReplace
            // 
            resources.ApplyResources(this.checkBoxRegexReplace, "checkBoxRegexReplace");
            this.checkBoxRegexReplace.Name = "checkBoxRegexReplace";
            this.checkBoxRegexReplace.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // comboBoxTarget
            // 
            resources.ApplyResources(this.comboBoxTarget, "comboBoxTarget");
            this.comboBoxTarget.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTarget.FormattingEnabled = true;
            this.comboBoxTarget.Name = "comboBoxTarget";
            this.comboBoxTarget.SelectedIndexChanged += new System.EventHandler(this.comboBoxTarget_SelectedIndexChanged);
            // 
            // checkBoxCase
            // 
            resources.ApplyResources(this.checkBoxCase, "checkBoxCase");
            this.checkBoxCase.Name = "checkBoxCase";
            this.checkBoxCase.UseVisualStyleBackColor = true;
            // 
            // checkBoxWord
            // 
            resources.ApplyResources(this.checkBoxWord, "checkBoxWord");
            this.checkBoxWord.Name = "checkBoxWord";
            this.checkBoxWord.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // comboBoxMethod
            // 
            resources.ApplyResources(this.comboBoxMethod, "comboBoxMethod");
            this.comboBoxMethod.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxMethod.FormattingEnabled = true;
            this.comboBoxMethod.Items.AddRange(new object[] {
            resources.GetString("comboBoxMethod.Items"),
            resources.GetString("comboBoxMethod.Items1"),
            resources.GetString("comboBoxMethod.Items2")});
            this.comboBoxMethod.Name = "comboBoxMethod";
            // 
            // buttonFind
            // 
            resources.ApplyResources(this.buttonFind, "buttonFind");
            this.buttonFind.Name = "buttonFind";
            this.buttonFind.UseVisualStyleBackColor = true;
            this.buttonFind.Click += new System.EventHandler(this.buttonFind_Click);
            // 
            // checkBoxBackward
            // 
            resources.ApplyResources(this.checkBoxBackward, "checkBoxBackward");
            this.checkBoxBackward.Name = "checkBoxBackward";
            this.checkBoxBackward.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // FindWindowControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.checkBoxBackward);
            this.Controls.Add(this.buttonFind);
            this.Controls.Add(this.comboBoxMethod);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.checkBoxWord);
            this.Controls.Add(this.checkBoxCase);
            this.Controls.Add(this.comboBoxTarget);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.checkBoxRegexReplace);
            this.Controls.Add(this.textBoxReplace);
            this.Controls.Add(this.textBoxFind);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(300, 310);
            this.Name = "FindWindowControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxFind;
        private System.Windows.Forms.TextBox textBoxReplace;
        private System.Windows.Forms.CheckBox checkBoxRegexReplace;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxTarget;
        private System.Windows.Forms.CheckBox checkBoxCase;
        private System.Windows.Forms.CheckBox checkBoxWord;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxMethod;
        private System.Windows.Forms.Button buttonFind;
        private System.Windows.Forms.CheckBox checkBoxBackward;
        private System.Windows.Forms.Label label4;
    }
}
