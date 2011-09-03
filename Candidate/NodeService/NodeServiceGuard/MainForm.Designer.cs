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
            System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("Name");
            System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("Token");
            System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem("Semaphore Name");
            System.Windows.Forms.ListViewItem listViewItem9 = new System.Windows.Forms.ListViewItem("Execution Path");
            System.Windows.Forms.ListViewItem listViewItem10 = new System.Windows.Forms.ListViewItem("Arguments");
            this.listViewServices = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuService = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextMenuServiceViewDescription = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.contextMenuServiceRestart = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuServiceStop = new System.Windows.Forms.ToolStripMenuItem();
            this.timerRestart = new System.Windows.Forms.Timer(this.components);
            this.splitContainerService = new System.Windows.Forms.SplitContainer();
            this.tabControlProperties = new System.Windows.Forms.TabControl();
            this.tabPageInformation = new System.Windows.Forms.TabPage();
            this.listViewServiceInformation = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPageTracing = new System.Windows.Forms.TabPage();
            this.buttonReadTracingResult = new System.Windows.Forms.Button();
            this.textBoxTracingResult = new System.Windows.Forms.TextBox();
            this.buttonStopTracing = new System.Windows.Forms.Button();
            this.buttonStartTracing = new System.Windows.Forms.Button();
            this.labelServiceTracing = new System.Windows.Forms.Label();
            this.contextMenuServiceConnect = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuService.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerService)).BeginInit();
            this.splitContainerService.Panel1.SuspendLayout();
            this.splitContainerService.Panel2.SuspendLayout();
            this.splitContainerService.SuspendLayout();
            this.tabControlProperties.SuspendLayout();
            this.tabPageInformation.SuspendLayout();
            this.tabPageTracing.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewServices
            // 
            this.listViewServices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewServices.ContextMenuStrip = this.contextMenuService;
            this.listViewServices.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewServices.FullRowSelect = true;
            this.listViewServices.GridLines = true;
            this.listViewServices.HideSelection = false;
            this.listViewServices.Location = new System.Drawing.Point(0, 0);
            this.listViewServices.MultiSelect = false;
            this.listViewServices.Name = "listViewServices";
            this.listViewServices.Size = new System.Drawing.Size(777, 355);
            this.listViewServices.TabIndex = 0;
            this.listViewServices.UseCompatibleStateImageBehavior = false;
            this.listViewServices.View = System.Windows.Forms.View.Details;
            this.listViewServices.SelectedIndexChanged += new System.EventHandler(this.listViewServices_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 355;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Token";
            this.columnHeader2.Width = 308;
            // 
            // contextMenuService
            // 
            this.contextMenuService.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contextMenuServiceViewDescription,
            this.contextMenuServiceConnect,
            this.toolStripMenuItem1,
            this.contextMenuServiceRestart,
            this.contextMenuServiceStop});
            this.contextMenuService.Name = "contextMenuService";
            this.contextMenuService.Size = new System.Drawing.Size(183, 120);
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
            // contextMenuServiceRestart
            // 
            this.contextMenuServiceRestart.Name = "contextMenuServiceRestart";
            this.contextMenuServiceRestart.Size = new System.Drawing.Size(182, 22);
            this.contextMenuServiceRestart.Text = "Restart";
            this.contextMenuServiceRestart.Click += new System.EventHandler(this.contextMenuServiceRestart_Click);
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
            // splitContainerService
            // 
            this.splitContainerService.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainerService.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerService.Location = new System.Drawing.Point(12, 11);
            this.splitContainerService.Name = "splitContainerService";
            this.splitContainerService.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerService.Panel1
            // 
            this.splitContainerService.Panel1.Controls.Add(this.listViewServices);
            // 
            // splitContainerService.Panel2
            // 
            this.splitContainerService.Panel2.Controls.Add(this.tabControlProperties);
            this.splitContainerService.Size = new System.Drawing.Size(777, 652);
            this.splitContainerService.SplitterDistance = 355;
            this.splitContainerService.TabIndex = 1;
            // 
            // tabControlProperties
            // 
            this.tabControlProperties.Controls.Add(this.tabPageInformation);
            this.tabControlProperties.Controls.Add(this.tabPageTracing);
            this.tabControlProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlProperties.Location = new System.Drawing.Point(0, 0);
            this.tabControlProperties.Name = "tabControlProperties";
            this.tabControlProperties.SelectedIndex = 0;
            this.tabControlProperties.Size = new System.Drawing.Size(777, 293);
            this.tabControlProperties.TabIndex = 0;
            // 
            // tabPageInformation
            // 
            this.tabPageInformation.Controls.Add(this.listViewServiceInformation);
            this.tabPageInformation.Location = new System.Drawing.Point(4, 22);
            this.tabPageInformation.Name = "tabPageInformation";
            this.tabPageInformation.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageInformation.Size = new System.Drawing.Size(769, 267);
            this.tabPageInformation.TabIndex = 0;
            this.tabPageInformation.Text = "Information";
            this.tabPageInformation.UseVisualStyleBackColor = true;
            // 
            // listViewServiceInformation
            // 
            this.listViewServiceInformation.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.listViewServiceInformation.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewServiceInformation.FullRowSelect = true;
            this.listViewServiceInformation.GridLines = true;
            this.listViewServiceInformation.HideSelection = false;
            this.listViewServiceInformation.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem6,
            listViewItem7,
            listViewItem8,
            listViewItem9,
            listViewItem10});
            this.listViewServiceInformation.Location = new System.Drawing.Point(3, 3);
            this.listViewServiceInformation.MultiSelect = false;
            this.listViewServiceInformation.Name = "listViewServiceInformation";
            this.listViewServiceInformation.Size = new System.Drawing.Size(763, 261);
            this.listViewServiceInformation.TabIndex = 0;
            this.listViewServiceInformation.UseCompatibleStateImageBehavior = false;
            this.listViewServiceInformation.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Property";
            this.columnHeader3.Width = 169;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Value";
            this.columnHeader4.Width = 533;
            // 
            // tabPageTracing
            // 
            this.tabPageTracing.Controls.Add(this.buttonReadTracingResult);
            this.tabPageTracing.Controls.Add(this.textBoxTracingResult);
            this.tabPageTracing.Controls.Add(this.buttonStopTracing);
            this.tabPageTracing.Controls.Add(this.buttonStartTracing);
            this.tabPageTracing.Controls.Add(this.labelServiceTracing);
            this.tabPageTracing.Location = new System.Drawing.Point(4, 22);
            this.tabPageTracing.Name = "tabPageTracing";
            this.tabPageTracing.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTracing.Size = new System.Drawing.Size(769, 244);
            this.tabPageTracing.TabIndex = 1;
            this.tabPageTracing.Text = "Tracing";
            this.tabPageTracing.UseVisualStyleBackColor = true;
            // 
            // buttonReadTracingResult
            // 
            this.buttonReadTracingResult.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonReadTracingResult.Location = new System.Drawing.Point(620, 6);
            this.buttonReadTracingResult.Name = "buttonReadTracingResult";
            this.buttonReadTracingResult.Size = new System.Drawing.Size(143, 21);
            this.buttonReadTracingResult.TabIndex = 4;
            this.buttonReadTracingResult.Text = "Read Tracing Result";
            this.buttonReadTracingResult.UseVisualStyleBackColor = true;
            this.buttonReadTracingResult.Click += new System.EventHandler(this.buttonReadTracingResult_Click);
            // 
            // textBoxTracingResult
            // 
            this.textBoxTracingResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxTracingResult.Location = new System.Drawing.Point(9, 32);
            this.textBoxTracingResult.Multiline = true;
            this.textBoxTracingResult.Name = "textBoxTracingResult";
            this.textBoxTracingResult.ReadOnly = true;
            this.textBoxTracingResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxTracingResult.Size = new System.Drawing.Size(754, 209);
            this.textBoxTracingResult.TabIndex = 3;
            // 
            // buttonStopTracing
            // 
            this.buttonStopTracing.Location = new System.Drawing.Point(183, 6);
            this.buttonStopTracing.Name = "buttonStopTracing";
            this.buttonStopTracing.Size = new System.Drawing.Size(75, 21);
            this.buttonStopTracing.TabIndex = 2;
            this.buttonStopTracing.Text = "Stop";
            this.buttonStopTracing.UseVisualStyleBackColor = true;
            this.buttonStopTracing.Click += new System.EventHandler(this.buttonStopTracing_Click);
            // 
            // buttonStartTracing
            // 
            this.buttonStartTracing.Location = new System.Drawing.Point(102, 6);
            this.buttonStartTracing.Name = "buttonStartTracing";
            this.buttonStartTracing.Size = new System.Drawing.Size(75, 21);
            this.buttonStartTracing.TabIndex = 1;
            this.buttonStartTracing.Text = "Start";
            this.buttonStartTracing.UseVisualStyleBackColor = true;
            this.buttonStartTracing.Click += new System.EventHandler(this.buttonStartTracing_Click);
            // 
            // labelServiceTracing
            // 
            this.labelServiceTracing.AutoSize = true;
            this.labelServiceTracing.Location = new System.Drawing.Point(6, 10);
            this.labelServiceTracing.Name = "labelServiceTracing";
            this.labelServiceTracing.Size = new System.Drawing.Size(77, 12);
            this.labelServiceTracing.TabIndex = 0;
            this.labelServiceTracing.Text = "Tracing: Off";
            // 
            // contextMenuServiceConnect
            // 
            this.contextMenuServiceConnect.Name = "contextMenuServiceConnect";
            this.contextMenuServiceConnect.Size = new System.Drawing.Size(182, 22);
            this.contextMenuServiceConnect.Text = "Connect...";
            this.contextMenuServiceConnect.Click += new System.EventHandler(this.contextMenuServiceConnect_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(801, 674);
            this.Controls.Add(this.splitContainerService);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Node Service Guard v1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.contextMenuService.ResumeLayout(false);
            this.splitContainerService.Panel1.ResumeLayout(false);
            this.splitContainerService.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerService)).EndInit();
            this.splitContainerService.ResumeLayout(false);
            this.tabControlProperties.ResumeLayout(false);
            this.tabPageInformation.ResumeLayout(false);
            this.tabPageTracing.ResumeLayout(false);
            this.tabPageTracing.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewServices;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Timer timerRestart;
        private System.Windows.Forms.ContextMenuStrip contextMenuService;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceViewDescription;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceStop;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceRestart;
        private System.Windows.Forms.SplitContainer splitContainerService;
        private System.Windows.Forms.TabControl tabControlProperties;
        private System.Windows.Forms.TabPage tabPageInformation;
        private System.Windows.Forms.ListView listViewServiceInformation;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.TabPage tabPageTracing;
        private System.Windows.Forms.Button buttonStopTracing;
        private System.Windows.Forms.Button buttonStartTracing;
        private System.Windows.Forms.Label labelServiceTracing;
        private System.Windows.Forms.TextBox textBoxTracingResult;
        private System.Windows.Forms.Button buttonReadTracingResult;
        private System.Windows.Forms.ToolStripMenuItem contextMenuServiceConnect;
    }
}

