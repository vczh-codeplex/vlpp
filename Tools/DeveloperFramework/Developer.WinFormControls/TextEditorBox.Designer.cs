namespace Developer.WinFormControls
{
    partial class TextEditorBox
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
            this.components = new System.ComponentModel.Container();
            this.timerCaret = new System.Windows.Forms.Timer(this.components);
            this.toolTipSimple = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // timerCaret
            // 
            this.timerCaret.Enabled = true;
            this.timerCaret.Interval = 500;
            this.timerCaret.Tick += new System.EventHandler(this.timerCaret_Tick);
            // 
            // TextEditorBox
            // 
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Name = "TextEditorBox";
            this.Size = new System.Drawing.Size(371, 368);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer timerCaret;
        private System.Windows.Forms.ToolTip toolTipSimple;
    }
}
