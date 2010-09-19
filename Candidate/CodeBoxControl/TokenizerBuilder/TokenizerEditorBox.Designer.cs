namespace TokenizerBuilder
{
    partial class TokenizerEditorBox
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
            this.contextMenuState = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuItemStateEditName = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemStateSwitchType = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemStateSwitchTypeNormal = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemStateSwitchTypeStart = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemStateSwitchTypeFinish = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemStateSwitchTypePartial = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuArrow = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuItemArrowEditName = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuState.SuspendLayout();
            this.contextMenuArrow.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuState
            // 
            this.contextMenuState.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuItemStateEditName,
            this.menuItemStateSwitchType});
            this.contextMenuState.Name = "contextMenuState";
            this.contextMenuState.Size = new System.Drawing.Size(153, 70);
            // 
            // menuItemStateEditName
            // 
            this.menuItemStateEditName.Name = "menuItemStateEditName";
            this.menuItemStateEditName.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateEditName.Text = "Edit Name...";
            this.menuItemStateEditName.Click += new System.EventHandler(this.menuItemStateEditName_Click);
            // 
            // menuItemStateSwitchType
            // 
            this.menuItemStateSwitchType.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuItemStateSwitchTypeNormal,
            this.menuItemStateSwitchTypeStart,
            this.menuItemStateSwitchTypeFinish,
            this.menuItemStateSwitchTypePartial});
            this.menuItemStateSwitchType.Name = "menuItemStateSwitchType";
            this.menuItemStateSwitchType.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateSwitchType.Text = "Switch Type";
            // 
            // menuItemStateSwitchTypeNormal
            // 
            this.menuItemStateSwitchTypeNormal.Name = "menuItemStateSwitchTypeNormal";
            this.menuItemStateSwitchTypeNormal.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateSwitchTypeNormal.Text = "Normal";
            this.menuItemStateSwitchTypeNormal.Click += new System.EventHandler(this.menuItemStateSwitchTypeNormal_Click);
            // 
            // menuItemStateSwitchTypeStart
            // 
            this.menuItemStateSwitchTypeStart.Name = "menuItemStateSwitchTypeStart";
            this.menuItemStateSwitchTypeStart.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateSwitchTypeStart.Text = "Start";
            this.menuItemStateSwitchTypeStart.Click += new System.EventHandler(this.menuItemStateSwitchTypeStart_Click);
            // 
            // menuItemStateSwitchTypeFinish
            // 
            this.menuItemStateSwitchTypeFinish.Name = "menuItemStateSwitchTypeFinish";
            this.menuItemStateSwitchTypeFinish.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateSwitchTypeFinish.Text = "Finish";
            this.menuItemStateSwitchTypeFinish.Click += new System.EventHandler(this.menuItemStateSwitchTypeFinish_Click);
            // 
            // menuItemStateSwitchTypePartial
            // 
            this.menuItemStateSwitchTypePartial.Name = "menuItemStateSwitchTypePartial";
            this.menuItemStateSwitchTypePartial.Size = new System.Drawing.Size(152, 22);
            this.menuItemStateSwitchTypePartial.Text = "Partial";
            this.menuItemStateSwitchTypePartial.Click += new System.EventHandler(this.menuItemStateSwitchTypePartial_Click);
            // 
            // contextMenuArrow
            // 
            this.contextMenuArrow.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuItemArrowEditName});
            this.contextMenuArrow.Name = "contextMenuArrow";
            this.contextMenuArrow.Size = new System.Drawing.Size(153, 48);
            // 
            // menuItemArrowEditName
            // 
            this.menuItemArrowEditName.Name = "menuItemArrowEditName";
            this.menuItemArrowEditName.Size = new System.Drawing.Size(152, 22);
            this.menuItemArrowEditName.Text = "Edit Name...";
            this.menuItemArrowEditName.Click += new System.EventHandler(this.menuItemArrowEditName_Click);
            // 
            // TokenizerEditorBox
            // 
            this.Name = "TokenizerEditorBox";
            this.Size = new System.Drawing.Size(380, 378);
            this.contextMenuState.ResumeLayout(false);
            this.contextMenuArrow.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenuState;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateEditName;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateSwitchType;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateSwitchTypeNormal;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateSwitchTypeStart;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateSwitchTypeFinish;
        private System.Windows.Forms.ToolStripMenuItem menuItemStateSwitchTypePartial;
        private System.Windows.Forms.ContextMenuStrip contextMenuArrow;
        private System.Windows.Forms.ToolStripMenuItem menuItemArrowEditName;
    }
}
