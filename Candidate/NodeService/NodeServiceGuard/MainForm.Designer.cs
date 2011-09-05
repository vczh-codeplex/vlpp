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
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("Name");
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("Token");
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("Semaphore Name");
            System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("Execution Path");
            System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("Arguments");
            this.listViewServices = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuService = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextMenuServiceViewDescription = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuServiceConnect = new System.Windows.Forms.ToolStripMenuItem();
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
            this.tableLayoutPanelTracing = new System.Windows.Forms.TableLayoutPanel();
            this.buttonReadTracingResult = new System.Windows.Forms.Button();
            this.textBoxTracingResult = new System.Windows.Forms.TextBox();
            this.buttonStopTracing = new System.Windows.Forms.Button();
            this.labelServiceTracing = new System.Windows.Forms.Label();
            this.buttonStartTracing = new System.Windows.Forms.Button();
            this.tabPageRemote = new System.Windows.Forms.TabPage();
            this.tableLayoutPanelRemote = new System.Windows.Forms.TableLayoutPanel();
            this.buttonViewRemoteServiceDescription = new System.Windows.Forms.Button();
            this.listViewRemoteServices = new System.Windows.Forms.ListView();
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.labelConnectedRemoteServiceGuard = new System.Windows.Forms.Label();
            this.buttonConnectRemoteServiceGuard = new System.Windows.Forms.Button();
            this.buttonConnectRemoteService = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxRemoteServiceGuard = new System.Windows.Forms.TextBox();
            this.contextMenuService.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerService)).BeginInit();
            this.splitContainerService.Panel1.SuspendLayout();
            this.splitContainerService.Panel2.SuspendLayout();
            this.splitContainerService.SuspendLayout();
            this.tabControlProperties.SuspendLayout();
            this.tabPageInformation.SuspendLayout();
            this.tabPageTracing.SuspendLayout();
            this.tableLayoutPanelTracing.SuspendLayout();
            this.tabPageRemote.SuspendLayout();
            this.tableLayoutPanelRemote.SuspendLayout();
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
            this.listViewServices.Size = new System.Drawing.Size(794, 346);
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
            this.columnHeader2.Width = 397;
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
            this.contextMenuService.Size = new System.Drawing.Size(172, 98);
            this.contextMenuService.Closing += new System.Windows.Forms.ToolStripDropDownClosingEventHandler(this.contextMenuService_Closing);
            this.contextMenuService.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuService_Opening);
            // 
            // contextMenuServiceViewDescription
            // 
            this.contextMenuServiceViewDescription.Name = "contextMenuServiceViewDescription";
            this.contextMenuServiceViewDescription.Size = new System.Drawing.Size(171, 22);
            this.contextMenuServiceViewDescription.Text = "View Description...";
            this.contextMenuServiceViewDescription.Click += new System.EventHandler(this.contextMenuServiceViewDescription_Click);
            // 
            // contextMenuServiceConnect
            // 
            this.contextMenuServiceConnect.Name = "contextMenuServiceConnect";
            this.contextMenuServiceConnect.Size = new System.Drawing.Size(171, 22);
            this.contextMenuServiceConnect.Text = "Connect...";
            this.contextMenuServiceConnect.Click += new System.EventHandler(this.contextMenuServiceConnect_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(168, 6);
            // 
            // contextMenuServiceRestart
            // 
            this.contextMenuServiceRestart.Name = "contextMenuServiceRestart";
            this.contextMenuServiceRestart.Size = new System.Drawing.Size(171, 22);
            this.contextMenuServiceRestart.Text = "Restart";
            this.contextMenuServiceRestart.Click += new System.EventHandler(this.contextMenuServiceRestart_Click);
            // 
            // contextMenuServiceStop
            // 
            this.contextMenuServiceStop.Name = "contextMenuServiceStop";
            this.contextMenuServiceStop.Size = new System.Drawing.Size(171, 22);
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
            this.splitContainerService.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerService.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerService.Location = new System.Drawing.Point(0, 0);
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
            this.splitContainerService.Size = new System.Drawing.Size(794, 652);
            this.splitContainerService.SplitterDistance = 346;
            this.splitContainerService.TabIndex = 1;
            // 
            // tabControlProperties
            // 
            this.tabControlProperties.Controls.Add(this.tabPageInformation);
            this.tabControlProperties.Controls.Add(this.tabPageTracing);
            this.tabControlProperties.Controls.Add(this.tabPageRemote);
            this.tabControlProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlProperties.Location = new System.Drawing.Point(0, 0);
            this.tabControlProperties.Name = "tabControlProperties";
            this.tabControlProperties.SelectedIndex = 0;
            this.tabControlProperties.Size = new System.Drawing.Size(794, 302);
            this.tabControlProperties.TabIndex = 0;
            // 
            // tabPageInformation
            // 
            this.tabPageInformation.Controls.Add(this.listViewServiceInformation);
            this.tabPageInformation.Location = new System.Drawing.Point(4, 22);
            this.tabPageInformation.Name = "tabPageInformation";
            this.tabPageInformation.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageInformation.Size = new System.Drawing.Size(786, 276);
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
            listViewItem1,
            listViewItem2,
            listViewItem3,
            listViewItem4,
            listViewItem5});
            this.listViewServiceInformation.Location = new System.Drawing.Point(3, 3);
            this.listViewServiceInformation.MultiSelect = false;
            this.listViewServiceInformation.Name = "listViewServiceInformation";
            this.listViewServiceInformation.Size = new System.Drawing.Size(780, 270);
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
            this.tabPageTracing.Controls.Add(this.tableLayoutPanelTracing);
            this.tabPageTracing.Location = new System.Drawing.Point(4, 22);
            this.tabPageTracing.Name = "tabPageTracing";
            this.tabPageTracing.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTracing.Size = new System.Drawing.Size(762, 276);
            this.tabPageTracing.TabIndex = 1;
            this.tabPageTracing.Text = "Tracing";
            this.tabPageTracing.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelTracing
            // 
            this.tableLayoutPanelTracing.ColumnCount = 5;
            this.tableLayoutPanelTracing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 160F));
            this.tableLayoutPanelTracing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelTracing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelTracing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelTracing.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelTracing.Controls.Add(this.buttonReadTracingResult, 4, 0);
            this.tableLayoutPanelTracing.Controls.Add(this.textBoxTracingResult, 0, 1);
            this.tableLayoutPanelTracing.Controls.Add(this.buttonStopTracing, 2, 0);
            this.tableLayoutPanelTracing.Controls.Add(this.labelServiceTracing, 0, 0);
            this.tableLayoutPanelTracing.Controls.Add(this.buttonStartTracing, 1, 0);
            this.tableLayoutPanelTracing.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelTracing.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanelTracing.Name = "tableLayoutPanelTracing";
            this.tableLayoutPanelTracing.RowCount = 2;
            this.tableLayoutPanelTracing.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelTracing.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelTracing.Size = new System.Drawing.Size(756, 270);
            this.tableLayoutPanelTracing.TabIndex = 5;
            // 
            // buttonReadTracingResult
            // 
            this.buttonReadTracingResult.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonReadTracingResult.Location = new System.Drawing.Point(610, 3);
            this.buttonReadTracingResult.Name = "buttonReadTracingResult";
            this.buttonReadTracingResult.Size = new System.Drawing.Size(143, 23);
            this.buttonReadTracingResult.TabIndex = 4;
            this.buttonReadTracingResult.Text = "Read Tracing Result";
            this.buttonReadTracingResult.UseVisualStyleBackColor = true;
            this.buttonReadTracingResult.Click += new System.EventHandler(this.buttonReadTracingResult_Click);
            // 
            // textBoxTracingResult
            // 
            this.tableLayoutPanelTracing.SetColumnSpan(this.textBoxTracingResult, 5);
            this.textBoxTracingResult.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxTracingResult.Location = new System.Drawing.Point(3, 32);
            this.textBoxTracingResult.Multiline = true;
            this.textBoxTracingResult.Name = "textBoxTracingResult";
            this.textBoxTracingResult.ReadOnly = true;
            this.textBoxTracingResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxTracingResult.Size = new System.Drawing.Size(750, 235);
            this.textBoxTracingResult.TabIndex = 3;
            // 
            // buttonStopTracing
            // 
            this.buttonStopTracing.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonStopTracing.Location = new System.Drawing.Point(244, 3);
            this.buttonStopTracing.Name = "buttonStopTracing";
            this.buttonStopTracing.Size = new System.Drawing.Size(75, 23);
            this.buttonStopTracing.TabIndex = 2;
            this.buttonStopTracing.Text = "Stop";
            this.buttonStopTracing.UseVisualStyleBackColor = true;
            this.buttonStopTracing.Click += new System.EventHandler(this.buttonStopTracing_Click);
            // 
            // labelServiceTracing
            // 
            this.labelServiceTracing.AutoSize = true;
            this.labelServiceTracing.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelServiceTracing.Location = new System.Drawing.Point(3, 0);
            this.labelServiceTracing.Name = "labelServiceTracing";
            this.labelServiceTracing.Size = new System.Drawing.Size(154, 29);
            this.labelServiceTracing.TabIndex = 0;
            this.labelServiceTracing.Text = "Tracing: Off";
            this.labelServiceTracing.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // buttonStartTracing
            // 
            this.buttonStartTracing.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonStartTracing.Location = new System.Drawing.Point(163, 3);
            this.buttonStartTracing.Name = "buttonStartTracing";
            this.buttonStartTracing.Size = new System.Drawing.Size(75, 23);
            this.buttonStartTracing.TabIndex = 1;
            this.buttonStartTracing.Text = "Start";
            this.buttonStartTracing.UseVisualStyleBackColor = true;
            this.buttonStartTracing.Click += new System.EventHandler(this.buttonStartTracing_Click);
            // 
            // tabPageRemote
            // 
            this.tabPageRemote.Controls.Add(this.tableLayoutPanelRemote);
            this.tabPageRemote.Location = new System.Drawing.Point(4, 22);
            this.tabPageRemote.Name = "tabPageRemote";
            this.tabPageRemote.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageRemote.Size = new System.Drawing.Size(762, 276);
            this.tabPageRemote.TabIndex = 2;
            this.tabPageRemote.Text = "Remote Node Service Guard";
            this.tabPageRemote.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanelRemote
            // 
            this.tableLayoutPanelRemote.ColumnCount = 5;
            this.tableLayoutPanelRemote.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelRemote.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelRemote.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 84.55882F));
            this.tableLayoutPanelRemote.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15.44118F));
            this.tableLayoutPanelRemote.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelRemote.Controls.Add(this.buttonConnectRemoteServiceGuard, 4, 0);
            this.tableLayoutPanelRemote.Controls.Add(this.listViewRemoteServices, 0, 2);
            this.tableLayoutPanelRemote.Controls.Add(this.buttonViewRemoteServiceDescription, 0, 3);
            this.tableLayoutPanelRemote.Controls.Add(this.labelConnectedRemoteServiceGuard, 0, 1);
            this.tableLayoutPanelRemote.Controls.Add(this.buttonConnectRemoteService, 4, 3);
            this.tableLayoutPanelRemote.Controls.Add(this.label2, 2, 0);
            this.tableLayoutPanelRemote.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanelRemote.Controls.Add(this.textBoxRemoteServiceGuard, 1, 0);
            this.tableLayoutPanelRemote.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelRemote.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanelRemote.Name = "tableLayoutPanelRemote";
            this.tableLayoutPanelRemote.RowCount = 4;
            this.tableLayoutPanelRemote.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelRemote.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelRemote.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelRemote.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelRemote.Size = new System.Drawing.Size(756, 270);
            this.tableLayoutPanelRemote.TabIndex = 8;
            // 
            // buttonViewRemoteServiceDescription
            // 
            this.buttonViewRemoteServiceDescription.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanelRemote.SetColumnSpan(this.buttonViewRemoteServiceDescription, 4);
            this.buttonViewRemoteServiceDescription.Enabled = false;
            this.buttonViewRemoteServiceDescription.Location = new System.Drawing.Point(306, 242);
            this.buttonViewRemoteServiceDescription.Name = "buttonViewRemoteServiceDescription";
            this.buttonViewRemoteServiceDescription.Size = new System.Drawing.Size(218, 25);
            this.buttonViewRemoteServiceDescription.TabIndex = 7;
            this.buttonViewRemoteServiceDescription.Text = "View Service Description...";
            this.buttonViewRemoteServiceDescription.UseVisualStyleBackColor = true;
            this.buttonViewRemoteServiceDescription.Click += new System.EventHandler(this.buttonViewRemoteServiceDescription_Click);
            // 
            // listViewRemoteServices
            // 
            this.listViewRemoteServices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6});
            this.tableLayoutPanelRemote.SetColumnSpan(this.listViewRemoteServices, 5);
            this.listViewRemoteServices.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewRemoteServices.FullRowSelect = true;
            this.listViewRemoteServices.GridLines = true;
            this.listViewRemoteServices.HideSelection = false;
            this.listViewRemoteServices.Location = new System.Drawing.Point(3, 47);
            this.listViewRemoteServices.MultiSelect = false;
            this.listViewRemoteServices.Name = "listViewRemoteServices";
            this.listViewRemoteServices.Size = new System.Drawing.Size(750, 189);
            this.listViewRemoteServices.TabIndex = 5;
            this.listViewRemoteServices.UseCompatibleStateImageBehavior = false;
            this.listViewRemoteServices.View = System.Windows.Forms.View.Details;
            this.listViewRemoteServices.SelectedIndexChanged += new System.EventHandler(this.listViewRemoteServices_SelectedIndexChanged);
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Name";
            this.columnHeader5.Width = 245;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Service Description URL";
            this.columnHeader6.Width = 500;
            // 
            // labelConnectedRemoteServiceGuard
            // 
            this.labelConnectedRemoteServiceGuard.AutoSize = true;
            this.tableLayoutPanelRemote.SetColumnSpan(this.labelConnectedRemoteServiceGuard, 5);
            this.labelConnectedRemoteServiceGuard.Location = new System.Drawing.Point(3, 31);
            this.labelConnectedRemoteServiceGuard.Name = "labelConnectedRemoteServiceGuard";
            this.labelConnectedRemoteServiceGuard.Size = new System.Drawing.Size(195, 13);
            this.labelConnectedRemoteServiceGuard.TabIndex = 4;
            this.labelConnectedRemoteServiceGuard.Text = "Currently connected to: (not connected)";
            // 
            // buttonConnectRemoteServiceGuard
            // 
            this.buttonConnectRemoteServiceGuard.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonConnectRemoteServiceGuard.Location = new System.Drawing.Point(530, 3);
            this.buttonConnectRemoteServiceGuard.Name = "buttonConnectRemoteServiceGuard";
            this.buttonConnectRemoteServiceGuard.Size = new System.Drawing.Size(223, 25);
            this.buttonConnectRemoteServiceGuard.TabIndex = 3;
            this.buttonConnectRemoteServiceGuard.Text = "Connect to Remote Service Guard";
            this.buttonConnectRemoteServiceGuard.UseVisualStyleBackColor = true;
            this.buttonConnectRemoteServiceGuard.Click += new System.EventHandler(this.buttonConnectRemoteServiceGuard_Click);
            // 
            // buttonConnectRemoteService
            // 
            this.buttonConnectRemoteService.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonConnectRemoteService.Enabled = false;
            this.buttonConnectRemoteService.Location = new System.Drawing.Point(530, 242);
            this.buttonConnectRemoteService.Name = "buttonConnectRemoteService";
            this.buttonConnectRemoteService.Size = new System.Drawing.Size(223, 25);
            this.buttonConnectRemoteService.TabIndex = 6;
            this.buttonConnectRemoteService.Text = "Connect to Remote Service...";
            this.buttonConnectRemoteService.UseVisualStyleBackColor = true;
            this.buttonConnectRemoteService.Click += new System.EventHandler(this.buttonConnectRemoteService_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(192, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(280, 31);
            this.label2.TabIndex = 2;
            this.label2.Text = ":9000/NodeServices/ServiceList.xml";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 31);
            this.label1.TabIndex = 1;
            this.label1.Text = "http://";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // textBoxRemoteServiceGuard
            // 
            this.textBoxRemoteServiceGuard.Location = new System.Drawing.Point(47, 3);
            this.textBoxRemoteServiceGuard.Name = "textBoxRemoteServiceGuard";
            this.textBoxRemoteServiceGuard.Size = new System.Drawing.Size(139, 20);
            this.textBoxRemoteServiceGuard.TabIndex = 0;
            this.textBoxRemoteServiceGuard.Text = "localhost";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(794, 652);
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
            this.tableLayoutPanelTracing.ResumeLayout(false);
            this.tableLayoutPanelTracing.PerformLayout();
            this.tabPageRemote.ResumeLayout(false);
            this.tableLayoutPanelRemote.ResumeLayout(false);
            this.tableLayoutPanelRemote.PerformLayout();
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
        private System.Windows.Forms.TabPage tabPageRemote;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxRemoteServiceGuard;
        private System.Windows.Forms.Label labelConnectedRemoteServiceGuard;
        private System.Windows.Forms.Button buttonConnectRemoteServiceGuard;
        private System.Windows.Forms.ListView listViewRemoteServices;
        private System.Windows.Forms.Button buttonConnectRemoteService;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.Button buttonViewRemoteServiceDescription;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelTracing;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelRemote;
    }
}

