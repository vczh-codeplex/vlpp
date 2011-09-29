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
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.horizontalScrollBar = new System.Windows.Forms.HScrollBar();
            this.verticalScrollBar = new System.Windows.Forms.VScrollBar();
            this.imeEnabledPanel = new Developer.WinFormControls.ImeEnabledPanel();
            this.tableLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 2;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel.Controls.Add(this.horizontalScrollBar, 0, 1);
            this.tableLayoutPanel.Controls.Add(this.verticalScrollBar, 1, 0);
            this.tableLayoutPanel.Controls.Add(this.imeEnabledPanel, 0, 0);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 2;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.Size = new System.Drawing.Size(692, 640);
            this.tableLayoutPanel.TabIndex = 0;
            // 
            // horizontalScrollBar
            // 
            this.horizontalScrollBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.horizontalScrollBar.Enabled = false;
            this.horizontalScrollBar.Location = new System.Drawing.Point(0, 623);
            this.horizontalScrollBar.Name = "horizontalScrollBar";
            this.horizontalScrollBar.Size = new System.Drawing.Size(675, 17);
            this.horizontalScrollBar.TabIndex = 0;
            this.horizontalScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.horizontalScrollBar_Scroll);
            // 
            // verticalScrollBar
            // 
            this.verticalScrollBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.verticalScrollBar.Enabled = false;
            this.verticalScrollBar.Location = new System.Drawing.Point(675, 0);
            this.verticalScrollBar.Name = "verticalScrollBar";
            this.verticalScrollBar.Size = new System.Drawing.Size(17, 623);
            this.verticalScrollBar.TabIndex = 1;
            this.verticalScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.verticalScrollBar_Scroll);
            // 
            // imeEnabledPanel
            // 
            this.imeEnabledPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imeEnabledPanel.Location = new System.Drawing.Point(3, 3);
            this.imeEnabledPanel.Name = "imeEnabledPanel";
            this.imeEnabledPanel.ParentControl = null;
            this.imeEnabledPanel.Size = new System.Drawing.Size(669, 617);
            this.imeEnabledPanel.TabIndex = 2;
            this.imeEnabledPanel.SizeChanged += new System.EventHandler(this.imeEnabledPanel_SizeChanged);
            // 
            // ScrollableContentControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.tableLayoutPanel);
            this.Name = "ScrollableContentControl";
            this.Size = new System.Drawing.Size(692, 640);
            this.tableLayoutPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.HScrollBar horizontalScrollBar;
        private System.Windows.Forms.VScrollBar verticalScrollBar;
        private ImeEnabledPanel imeEnabledPanel;
    }
}
