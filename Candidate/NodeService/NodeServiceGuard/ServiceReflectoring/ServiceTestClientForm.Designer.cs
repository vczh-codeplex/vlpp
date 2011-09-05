namespace NodeServiceGuard.ServiceReflectoring
{
    partial class ServiceTestClientForm
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
            this.textBoxAddress = new System.Windows.Forms.TextBox();
            this.textBoxProtocolAddress = new System.Windows.Forms.TextBox();
            this.textBoxEndpointName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonClose = new System.Windows.Forms.Button();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.labelConnectionState = new System.Windows.Forms.Label();
            this.splitContainerData = new System.Windows.Forms.SplitContainer();
            this.tabControlParameter = new System.Windows.Forms.TabControl();
            this.tabPageParameterXml = new System.Windows.Forms.TabPage();
            this.textBoxParameterXml = new System.Windows.Forms.TextBox();
            this.tabControlResponse = new System.Windows.Forms.TabControl();
            this.tabPageResponseXml = new System.Windows.Forms.TabPage();
            this.textBoxResponseXml = new System.Windows.Forms.TextBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.comboBoxMethod = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerData)).BeginInit();
            this.splitContainerData.Panel1.SuspendLayout();
            this.splitContainerData.Panel2.SuspendLayout();
            this.splitContainerData.SuspendLayout();
            this.tabControlParameter.SuspendLayout();
            this.tabPageParameterXml.SuspendLayout();
            this.tabControlResponse.SuspendLayout();
            this.tabPageResponseXml.SuspendLayout();
            this.tableLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxAddress
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxAddress, 3);
            this.textBoxAddress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxAddress.Location = new System.Drawing.Point(183, 3);
            this.textBoxAddress.Name = "textBoxAddress";
            this.textBoxAddress.Size = new System.Drawing.Size(336, 20);
            this.textBoxAddress.TabIndex = 0;
            // 
            // textBoxProtocolAddress
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxProtocolAddress, 3);
            this.textBoxProtocolAddress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxProtocolAddress.Location = new System.Drawing.Point(183, 29);
            this.textBoxProtocolAddress.Name = "textBoxProtocolAddress";
            this.textBoxProtocolAddress.ReadOnly = true;
            this.textBoxProtocolAddress.Size = new System.Drawing.Size(336, 20);
            this.textBoxProtocolAddress.TabIndex = 1;
            // 
            // textBoxEndpointName
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxEndpointName, 3);
            this.textBoxEndpointName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxEndpointName.Location = new System.Drawing.Point(183, 55);
            this.textBoxEndpointName.Name = "textBoxEndpointName";
            this.textBoxEndpointName.ReadOnly = true;
            this.textBoxEndpointName.Size = new System.Drawing.Size(336, 20);
            this.textBoxEndpointName.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(174, 26);
            this.label1.TabIndex = 3;
            this.label1.Text = "Address:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(3, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(174, 26);
            this.label2.TabIndex = 4;
            this.label2.Text = "Protocol Address:";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Location = new System.Drawing.Point(3, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(174, 26);
            this.label3.TabIndex = 5;
            this.label3.Text = "Endpoint Name:";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // buttonClose
            // 
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonClose.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonClose.Location = new System.Drawing.Point(444, 605);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 25);
            this.buttonClose.TabIndex = 6;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonConnect.Location = new System.Drawing.Point(444, 81);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 25);
            this.buttonConnect.TabIndex = 7;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // labelConnectionState
            // 
            this.labelConnectionState.AutoSize = true;
            this.tableLayoutPanel.SetColumnSpan(this.labelConnectionState, 3);
            this.labelConnectionState.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelConnectionState.Location = new System.Drawing.Point(3, 78);
            this.labelConnectionState.Name = "labelConnectionState";
            this.labelConnectionState.Size = new System.Drawing.Size(435, 31);
            this.labelConnectionState.TabIndex = 8;
            this.labelConnectionState.Text = "Connecting...";
            this.labelConnectionState.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // splitContainerData
            // 
            this.tableLayoutPanel.SetColumnSpan(this.splitContainerData, 4);
            this.splitContainerData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerData.Location = new System.Drawing.Point(3, 139);
            this.splitContainerData.Name = "splitContainerData";
            this.splitContainerData.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerData.Panel1
            // 
            this.splitContainerData.Panel1.Controls.Add(this.tabControlParameter);
            // 
            // splitContainerData.Panel2
            // 
            this.splitContainerData.Panel2.Controls.Add(this.tabControlResponse);
            this.splitContainerData.Size = new System.Drawing.Size(516, 460);
            this.splitContainerData.SplitterDistance = 226;
            this.splitContainerData.TabIndex = 9;
            // 
            // tabControlParameter
            // 
            this.tabControlParameter.Controls.Add(this.tabPageParameterXml);
            this.tabControlParameter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlParameter.Location = new System.Drawing.Point(0, 0);
            this.tabControlParameter.Name = "tabControlParameter";
            this.tabControlParameter.SelectedIndex = 0;
            this.tabControlParameter.Size = new System.Drawing.Size(516, 226);
            this.tabControlParameter.TabIndex = 0;
            // 
            // tabPageParameterXml
            // 
            this.tabPageParameterXml.Controls.Add(this.textBoxParameterXml);
            this.tabPageParameterXml.Location = new System.Drawing.Point(4, 22);
            this.tabPageParameterXml.Name = "tabPageParameterXml";
            this.tabPageParameterXml.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageParameterXml.Size = new System.Drawing.Size(508, 200);
            this.tabPageParameterXml.TabIndex = 0;
            this.tabPageParameterXml.Text = "Request (XML)";
            this.tabPageParameterXml.UseVisualStyleBackColor = true;
            // 
            // textBoxParameterXml
            // 
            this.textBoxParameterXml.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxParameterXml.Location = new System.Drawing.Point(3, 3);
            this.textBoxParameterXml.Multiline = true;
            this.textBoxParameterXml.Name = "textBoxParameterXml";
            this.textBoxParameterXml.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxParameterXml.Size = new System.Drawing.Size(502, 194);
            this.textBoxParameterXml.TabIndex = 0;
            // 
            // tabControlResponse
            // 
            this.tabControlResponse.Controls.Add(this.tabPageResponseXml);
            this.tabControlResponse.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlResponse.Location = new System.Drawing.Point(0, 0);
            this.tabControlResponse.Name = "tabControlResponse";
            this.tabControlResponse.SelectedIndex = 0;
            this.tabControlResponse.Size = new System.Drawing.Size(516, 230);
            this.tabControlResponse.TabIndex = 0;
            // 
            // tabPageResponseXml
            // 
            this.tabPageResponseXml.Controls.Add(this.textBoxResponseXml);
            this.tabPageResponseXml.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseXml.Name = "tabPageResponseXml";
            this.tabPageResponseXml.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseXml.Size = new System.Drawing.Size(508, 204);
            this.tabPageResponseXml.TabIndex = 0;
            this.tabPageResponseXml.Text = "Response (XML)";
            this.tabPageResponseXml.UseVisualStyleBackColor = true;
            // 
            // textBoxResponseXml
            // 
            this.textBoxResponseXml.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseXml.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseXml.Multiline = true;
            this.textBoxResponseXml.Name = "textBoxResponseXml";
            this.textBoxResponseXml.ReadOnly = true;
            this.textBoxResponseXml.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseXml.Size = new System.Drawing.Size(502, 198);
            this.textBoxResponseXml.TabIndex = 0;
            // 
            // buttonSend
            // 
            this.buttonSend.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonSend.Location = new System.Drawing.Point(363, 605);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 25);
            this.buttonSend.TabIndex = 10;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // comboBoxMethod
            // 
            this.tableLayoutPanel.SetColumnSpan(this.comboBoxMethod, 3);
            this.comboBoxMethod.Dock = System.Windows.Forms.DockStyle.Fill;
            this.comboBoxMethod.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxMethod.FormattingEnabled = true;
            this.comboBoxMethod.Location = new System.Drawing.Point(183, 112);
            this.comboBoxMethod.Name = "comboBoxMethod";
            this.comboBoxMethod.Size = new System.Drawing.Size(336, 21);
            this.comboBoxMethod.TabIndex = 11;
            this.comboBoxMethod.SelectedIndexChanged += new System.EventHandler(this.comboBoxMethod_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Location = new System.Drawing.Point(3, 109);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(174, 27);
            this.label4.TabIndex = 12;
            this.label4.Text = "Method:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 4;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel.Controls.Add(this.splitContainerData, 0, 5);
            this.tableLayoutPanel.Controls.Add(this.buttonSend, 2, 6);
            this.tableLayoutPanel.Controls.Add(this.comboBoxMethod, 1, 4);
            this.tableLayoutPanel.Controls.Add(this.label4, 0, 4);
            this.tableLayoutPanel.Controls.Add(this.buttonClose, 3, 6);
            this.tableLayoutPanel.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel.Controls.Add(this.buttonConnect, 3, 3);
            this.tableLayoutPanel.Controls.Add(this.labelConnectionState, 0, 3);
            this.tableLayoutPanel.Controls.Add(this.textBoxAddress, 1, 0);
            this.tableLayoutPanel.Controls.Add(this.textBoxEndpointName, 1, 2);
            this.tableLayoutPanel.Controls.Add(this.textBoxProtocolAddress, 1, 1);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 7;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 14.28571F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.Size = new System.Drawing.Size(522, 633);
            this.tableLayoutPanel.TabIndex = 13;
            // 
            // ServiceTestClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonClose;
            this.ClientSize = new System.Drawing.Size(522, 633);
            this.Controls.Add(this.tableLayoutPanel);
            this.MinimizeBox = false;
            this.Name = "ServiceTestClientForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Service Test Client";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ServiceTestClientForm_FormClosing);
            this.Shown += new System.EventHandler(this.ServiceTestClientForm_Shown);
            this.splitContainerData.Panel1.ResumeLayout(false);
            this.splitContainerData.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerData)).EndInit();
            this.splitContainerData.ResumeLayout(false);
            this.tabControlParameter.ResumeLayout(false);
            this.tabPageParameterXml.ResumeLayout(false);
            this.tabPageParameterXml.PerformLayout();
            this.tabControlResponse.ResumeLayout(false);
            this.tabPageResponseXml.ResumeLayout(false);
            this.tabPageResponseXml.PerformLayout();
            this.tableLayoutPanel.ResumeLayout(false);
            this.tableLayoutPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxAddress;
        private System.Windows.Forms.TextBox textBoxProtocolAddress;
        private System.Windows.Forms.TextBox textBoxEndpointName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label labelConnectionState;
        private System.Windows.Forms.SplitContainer splitContainerData;
        private System.Windows.Forms.TabControl tabControlParameter;
        private System.Windows.Forms.TabPage tabPageParameterXml;
        private System.Windows.Forms.TabControl tabControlResponse;
        private System.Windows.Forms.TabPage tabPageResponseXml;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.ComboBox comboBoxMethod;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxParameterXml;
        private System.Windows.Forms.TextBox textBoxResponseXml;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
    }
}