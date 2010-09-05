namespace CodeBoxControl
{
    partial class ScrollableContentControl
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
            this.scrollHorizontal = new System.Windows.Forms.HScrollBar();
            this.scrollVertical = new System.Windows.Forms.VScrollBar();
            this.panelContent = new CodeBoxControl.ScrollableContentControl.ImePanel();
            this.SuspendLayout();
            // 
            // scrollHorizontal
            // 
            this.scrollHorizontal.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.scrollHorizontal.Location = new System.Drawing.Point(0, 345);
            this.scrollHorizontal.Name = "scrollHorizontal";
            this.scrollHorizontal.Size = new System.Drawing.Size(346, 20);
            this.scrollHorizontal.TabIndex = 0;
            this.scrollHorizontal.Scroll += new System.Windows.Forms.ScrollEventHandler(this.scrollHorizontal_Scroll);
            // 
            // scrollVertical
            // 
            this.scrollVertical.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.scrollVertical.Location = new System.Drawing.Point(346, 0);
            this.scrollVertical.Name = "scrollVertical";
            this.scrollVertical.Size = new System.Drawing.Size(20, 345);
            this.scrollVertical.TabIndex = 1;
            this.scrollVertical.Scroll += new System.Windows.Forms.ScrollEventHandler(this.scrollVertical_Scroll);
            // 
            // panelContent
            // 
            this.panelContent.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelContent.ImeEnabled = false;
            this.panelContent.Location = new System.Drawing.Point(0, 0);
            this.panelContent.Name = "panelContent";
            this.panelContent.ParentControl = null;
            this.panelContent.Size = new System.Drawing.Size(346, 345);
            this.panelContent.TabIndex = 2;
            this.panelContent.Paint += new System.Windows.Forms.PaintEventHandler(this.panelContent_Paint);
            this.panelContent.Resize += new System.EventHandler(this.panelContent_Resize);
            // 
            // ScrollableContentControl
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.Controls.Add(this.panelContent);
            this.Controls.Add(this.scrollVertical);
            this.Controls.Add(this.scrollHorizontal);
            this.Name = "ScrollableContentControl";
            this.Size = new System.Drawing.Size(366, 365);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.HScrollBar scrollHorizontal;
        private System.Windows.Forms.VScrollBar scrollVertical;
        private ImePanel panelContent;
    }
}
