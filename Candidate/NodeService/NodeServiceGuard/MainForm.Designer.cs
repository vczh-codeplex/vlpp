namespace NodeServiceGuard
{
    partial class MainForm
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
            this.listViewServices = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuService = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextMenuServiceViewDescription = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.contextMenuServiceStop = new System.Windows.Forms.ToolStripMenuItem();
            this.timerRestart = new System.Windows.Forms.Timer(this.components);
            this.contextMenuServiceRestart = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuService.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewServices
            // 
            this.listViewServices.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewServices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader4,
            this.columnHeader5});
            this.listViewServices.ContextMenuStrip = this.contextMenuService;
            this.listViewServices.FullRowSelect = true;
            this.listViewServices.GridLines = true;
            this.listViewServices.HideSelection = false;
            this.listViewServices.Location = new System.Drawing.Point(12, 11);
            this.listViewServices.MultiSelect = false;
            this.listViewServices.Name = "listViewServices";
            this.listViewServices.Size = new System.Drawing.Size(630, 560);
            this.listViewServices.TabIndex = 0;
            this.listViewServices.UseCompatibleStateImageBehavior = false;
            this.listViewServices.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 152;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Token";
            this.columnHeader2.Width = 196;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Executable Path";
            this.columnHeader4.Width = 139;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Arguments";
            this.columnHeader5.Width = 98;
            // 
            // contextMenuService
            // 
            this.contextMenuService.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextMenuServiceViewDescription,
            this.toolStripMenuItem1,
            this.contextMenuServiceRestart,
            this.contextMenuServiceStop});
            this.contextMenuService.Name = "contextMenuService";
            this.contextMenuService.Size = new System.Drawing.Size(183, 98);
            this.contextMenuService.Closing += new System.Windows.Forms.ToolStripDropDownClosingEventHandler(this.contextMenuService_Closing);
            this.contextMenuService.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuService_Opening);
            // 
            // contextMenuServiceViewDescription
            // 
            this.contextMenuServiceViewDescription.Name = "contextMenuServiceViewDescription";
            this.contextMenuServiceViewDescription.Size = new System.Drawing.Size(182, 22);
            this.contextMenuServiceViewDescription.Text = "View Description...";
            this.contextMenuServiceViewDescription.Click += new System.EventHandler(this.contextMenuServiceViewDescription_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(179, 6);
            // 
            // contextMenuServiceStop
            // 
            this.contextMenuServiceStop.Name = "contextMenuServiceStop";
            this.contextMenuServiceStop.Size = new System.Drawing.Size(182, 22);
            this.contextMenuServiceStop.Text = "Stop";
            this.contextMenuServiceStop.Click += new System.EventHandler(this.contextMenuServiceStop_Click);
            // 
            // timerRestart
            // 
            this.timerRestart.Enabled = true;
            this.timerRestart.Interval = 1000;
            this.timerRestart.Tick += new System.EventHandler(this.timerRestart_Tick);
            // 
            // contextMenuServiceRestart
            // 
            this.contextMenuServiceRestart.Name = "contextMenuServiceRestart";
            this.contextMenuServiceRestart.Size = new System.Drawing.Size(182, 22);
            this.contextMenuServiceRestart.Text = "Restart";
            this.contextMenuServiceRestart.Click += new System.EventHandler(this.contextMenuServiceRestart_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(654, 582);
            this.Controls.Add(this.listViewServices);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Node Service Guard v1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.contextMenuService.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewServices;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.Timer timerRestart;
        private System.Windows.Forms.ContextMenuStrip contextMenuService;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceViewDescription;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceStop;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceRestart;
    }
}

