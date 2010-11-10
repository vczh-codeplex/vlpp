namespace Developer.WinFormControls
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
            this.panelContent = new Developer.WinFormControls.ScrollableContentControl.ImePanel();
            this.tableLayoutContent = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutContent.SuspendLayout();
            this.SuspendLayout();
            // 
            // scrollHorizontal
            // 
            this.scrollHorizontal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scrollHorizontal.Location = new System.Drawing.Point(0, 349);
            this.scrollHorizontal.Name = "scrollHorizontal";
            this.scrollHorizontal.Size = new System.Drawing.Size(357, 20);
            this.scrollHorizontal.TabIndex = 0;
            this.scrollHorizontal.ValueChanged += new System.EventHandler(this.scrollHorizontal_ValueChanged);
            // 
            // scrollVertical
            // 
            this.scrollVertical.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scrollVertical.Location = new System.Drawing.Point(357, 0);
            this.scrollVertical.Name = "scrollVertical";
            this.scrollVertical.Size = new System.Drawing.Size(20, 349);
            this.scrollVertical.TabIndex = 1;
            this.scrollVertical.ValueChanged += new System.EventHandler(this.scrollVertical_ValueChanged);
            // 
            // panelContent
            // 
            this.panelContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelContent.ImeEnabled = false;
            this.panelContent.Location = new System.Drawing.Point(0, 0);
            this.panelContent.Margin = new System.Windows.Forms.Padding(0);
            this.panelContent.Name = "panelContent";
            this.panelContent.ParentControl = null;
            this.panelContent.Size = new System.Drawing.Size(357, 349);
            this.panelContent.TabIndex = 2;
            this.panelContent.Paint += new System.Windows.Forms.PaintEventHandler(this.panelContent_Paint);
            this.panelContent.Resize += new System.EventHandler(this.panelContent_Resize);
            // 
            // tableLayoutContent
            // 
            this.tableLayoutContent.ColumnCount = 2;
            this.tableLayoutContent.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutContent.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutContent.Controls.Add(this.scrollVertical, 1, 0);
            this.tableLayoutContent.Controls.Add(this.panelContent, 0, 0);
            this.tableLayoutContent.Controls.Add(this.scrollHorizontal, 0, 1);
            this.tableLayoutContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutContent.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutContent.Name = "tableLayoutContent";
            this.tableLayoutContent.RowCount = 2;
            this.tableLayoutContent.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutContent.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutContent.Size = new System.Drawing.Size(377, 369);
            this.tableLayoutContent.TabIndex = 3;
            // 
            // ScrollableContentControl
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.Controls.Add(this.tableLayoutContent);
            this.Name = "ScrollableContentControl";
            this.Size = new System.Drawing.Size(377, 369);
            this.tableLayoutContent.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.HScrollBar scrollHorizontal;
        private System.Windows.Forms.VScrollBar scrollVertical;
        private ImePanel panelContent;
        private System.Windows.Forms.TableLayoutPanel tableLayoutContent;
    }
}
