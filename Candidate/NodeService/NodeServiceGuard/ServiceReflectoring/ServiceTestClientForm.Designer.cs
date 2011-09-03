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
            this.tabControlResponse = new System.Windows.Forms.TabControl();
            this.tabPageResponseXml = new System.Windows.Forms.TabPage();
            this.buttonSend = new System.Windows.Forms.Button();
            this.comboBoxMethod = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxParameterXml = new System.Windows.Forms.TextBox();
            this.textBoxResponseXml = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerData)).BeginInit();
            this.splitContainerData.Panel1.SuspendLayout();
            this.splitContainerData.Panel2.SuspendLayout();
            this.splitContainerData.SuspendLayout();
            this.tabControlParameter.SuspendLayout();
            this.tabPageParameterXml.SuspendLayout();
            this.tabControlResponse.SuspendLayout();
            this.tabPageResponseXml.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxAddress
            // 
            this.textBoxAddress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxAddress.Location = new System.Drawing.Point(125, 12);
            this.textBoxAddress.Name = "textBoxAddress";
            this.textBoxAddress.Size = new System.Drawing.Size(425, 21);
            this.textBoxAddress.TabIndex = 0;
            // 
            // textBoxProtocolAddress
            // 
            this.textBoxProtocolAddress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxProtocolAddress.Location = new System.Drawing.Point(125, 39);
            this.textBoxProtocolAddress.Name = "textBoxProtocolAddress";
            this.textBoxProtocolAddress.ReadOnly = true;
            this.textBoxProtocolAddress.Size = new System.Drawing.Size(425, 21);
            this.textBoxProtocolAddress.TabIndex = 1;
            // 
            // textBoxEndpointName
            // 
            this.textBoxEndpointName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxEndpointName.Location = new System.Drawing.Point(125, 66);
            this.textBoxEndpointName.Name = "textBoxEndpointName";
            this.textBoxEndpointName.ReadOnly = true;
            this.textBoxEndpointName.Size = new System.Drawing.Size(425, 21);
            this.textBoxEndpointName.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Address:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(107, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "Protocol Address:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(89, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "Endpoint Name:";
            // 
            // buttonClose
            // 
            this.buttonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonClose.Location = new System.Drawing.Point(475, 549);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 6;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonConnect.Location = new System.Drawing.Point(475, 93);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 23);
            this.buttonConnect.TabIndex = 7;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // labelConnectionState
            // 
            this.labelConnectionState.AutoSize = true;
            this.labelConnectionState.Location = new System.Drawing.Point(12, 98);
            this.labelConnectionState.Name = "labelConnectionState";
            this.labelConnectionState.Size = new System.Drawing.Size(83, 12);
            this.labelConnectionState.TabIndex = 8;
            this.labelConnectionState.Text = "Connecting...";
            // 
            // splitContainerData
            // 
            this.splitContainerData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainerData.Location = new System.Drawing.Point(14, 148);
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
            this.splitContainerData.Size = new System.Drawing.Size(536, 395);
            this.splitContainerData.SplitterDistance = 197;
            this.splitContainerData.TabIndex = 9;
            // 
            // tabControlParameter
            // 
            this.tabControlParameter.Controls.Add(this.tabPageParameterXml);
            this.tabControlParameter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlParameter.Location = new System.Drawing.Point(0, 0);
            this.tabControlParameter.Name = "tabControlParameter";
            this.tabControlParameter.SelectedIndex = 0;
            this.tabControlParameter.Size = new System.Drawing.Size(536, 197);
            this.tabControlParameter.TabIndex = 0;
            // 
            // tabPageParameterXml
            // 
            this.tabPageParameterXml.Controls.Add(this.textBoxParameterXml);
            this.tabPageParameterXml.Location = new System.Drawing.Point(4, 22);
            this.tabPageParameterXml.Name = "tabPageParameterXml";
            this.tabPageParameterXml.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageParameterXml.Size = new System.Drawing.Size(528, 171);
            this.tabPageParameterXml.TabIndex = 0;
            this.tabPageParameterXml.Text = "Request (XML)";
            this.tabPageParameterXml.UseVisualStyleBackColor = true;
            // 
            // tabControlResponse
            // 
            this.tabControlResponse.Controls.Add(this.tabPageResponseXml);
            this.tabControlResponse.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlResponse.Location = new System.Drawing.Point(0, 0);
            this.tabControlResponse.Name = "tabControlResponse";
            this.tabControlResponse.SelectedIndex = 0;
            this.tabControlResponse.Size = new System.Drawing.Size(536, 194);
            this.tabControlResponse.TabIndex = 0;
            // 
            // tabPageResponseXml
            // 
            this.tabPageResponseXml.Controls.Add(this.textBoxResponseXml);
            this.tabPageResponseXml.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseXml.Name = "tabPageResponseXml";
            this.tabPageResponseXml.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseXml.Size = new System.Drawing.Size(528, 168);
            this.tabPageResponseXml.TabIndex = 0;
            this.tabPageResponseXml.Text = "Response (XML)";
            this.tabPageResponseXml.UseVisualStyleBackColor = true;
            // 
            // buttonSend
            // 
            this.buttonSend.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSend.Location = new System.Drawing.Point(394, 549);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 10;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // comboBoxMethod
            // 
            this.comboBoxMethod.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxMethod.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxMethod.FormattingEnabled = true;
            this.comboBoxMethod.Location = new System.Drawing.Point(125, 122);
            this.comboBoxMethod.Name = "comboBoxMethod";
            this.comboBoxMethod.Size = new System.Drawing.Size(425, 20);
            this.comboBoxMethod.TabIndex = 11;
            this.comboBoxMethod.SelectedIndexChanged += new System.EventHandler(this.comboBoxMethod_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 125);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 12;
            this.label4.Text = "Method:";
            // 
            // textBoxParameterXml
            // 
            this.textBoxParameterXml.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxParameterXml.Location = new System.Drawing.Point(3, 3);
            this.textBoxParameterXml.Multiline = true;
            this.textBoxParameterXml.Name = "textBoxParameterXml";
            this.textBoxParameterXml.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxParameterXml.Size = new System.Drawing.Size(522, 165);
            this.textBoxParameterXml.TabIndex = 0;
            // 
            // textBoxResponseXml
            // 
            this.textBoxResponseXml.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseXml.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseXml.Multiline = true;
            this.textBoxResponseXml.Name = "textBoxResponseXml";
            this.textBoxResponseXml.ReadOnly = true;
            this.textBoxResponseXml.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseXml.Size = new System.Drawing.Size(522, 162);
            this.textBoxResponseXml.TabIndex = 0;
            // 
            // ServiceTestClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonClose;
            this.ClientSize = new System.Drawing.Size(562, 584);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.comboBoxMethod);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.splitContainerData);
            this.Controls.Add(this.labelConnectionState);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxEndpointName);
            this.Controls.Add(this.textBoxProtocolAddress);
            this.Controls.Add(this.textBoxAddress);
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
            this.ResumeLayout(false);
            this.PerformLayout();

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
    }
}