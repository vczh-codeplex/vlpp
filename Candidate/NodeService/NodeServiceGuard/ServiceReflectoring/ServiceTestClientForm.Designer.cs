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
            this.tabPageResponseString = new System.Windows.Forms.TabPage();
            this.tabControlResponseString = new System.Windows.Forms.TabControl();
            this.tabPageResponseStringAscii = new System.Windows.Forms.TabPage();
            this.textBoxResponseStringAscii = new System.Windows.Forms.TextBox();
            this.tabPageResponseStringUtf16 = new System.Windows.Forms.TabPage();
            this.textBoxResponseStringUtf16 = new System.Windows.Forms.TextBox();
            this.tabPageResponseStringUtf16BE = new System.Windows.Forms.TabPage();
            this.textBoxResponseStringUtf16BE = new System.Windows.Forms.TextBox();
            this.tabPageResponseStringUtf8 = new System.Windows.Forms.TabPage();
            this.textBoxResponseStringUtf8 = new System.Windows.Forms.TextBox();
            this.tabPageResponseImage = new System.Windows.Forms.TabPage();
            this.labelResponseImageException = new System.Windows.Forms.Label();
            this.pictureBoxResponseImage = new System.Windows.Forms.PictureBox();
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
            this.tabPageResponseString.SuspendLayout();
            this.tabControlResponseString.SuspendLayout();
            this.tabPageResponseStringAscii.SuspendLayout();
            this.tabPageResponseStringUtf16.SuspendLayout();
            this.tabPageResponseStringUtf16BE.SuspendLayout();
            this.tabPageResponseStringUtf8.SuspendLayout();
            this.tabPageResponseImage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResponseImage)).BeginInit();
            this.tableLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxAddress
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxAddress, 3);
            this.textBoxAddress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxAddress.Location = new System.Drawing.Point(99, 3);
            this.textBoxAddress.Name = "textBoxAddress";
            this.textBoxAddress.Size = new System.Drawing.Size(420, 20);
            this.textBoxAddress.TabIndex = 0;
            // 
            // textBoxProtocolAddress
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxProtocolAddress, 3);
            this.textBoxProtocolAddress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxProtocolAddress.Location = new System.Drawing.Point(99, 29);
            this.textBoxProtocolAddress.Name = "textBoxProtocolAddress";
            this.textBoxProtocolAddress.ReadOnly = true;
            this.textBoxProtocolAddress.Size = new System.Drawing.Size(420, 20);
            this.textBoxProtocolAddress.TabIndex = 1;
            // 
            // textBoxEndpointName
            // 
            this.tableLayoutPanel.SetColumnSpan(this.textBoxEndpointName, 3);
            this.textBoxEndpointName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxEndpointName.Location = new System.Drawing.Point(99, 55);
            this.textBoxEndpointName.Name = "textBoxEndpointName";
            this.textBoxEndpointName.ReadOnly = true;
            this.textBoxEndpointName.Size = new System.Drawing.Size(420, 20);
            this.textBoxEndpointName.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(90, 26);
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
            this.label2.Size = new System.Drawing.Size(90, 26);
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
            this.label3.Size = new System.Drawing.Size(90, 26);
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
            this.tabControlResponse.Controls.Add(this.tabPageResponseString);
            this.tabControlResponse.Controls.Add(this.tabPageResponseImage);
            this.tabControlResponse.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlResponse.Location = new System.Drawing.Point(0, 0);
            this.tabControlResponse.Name = "tabControlResponse";
            this.tabControlResponse.SelectedIndex = 0;
            this.tabControlResponse.Size = new System.Drawing.Size(516, 230);
            this.tabControlResponse.TabIndex = 0;
            this.tabControlResponse.Selected += new System.Windows.Forms.TabControlEventHandler(this.tabControlResponse_Selected);
            // 
            // tabPageResponseXml
            // 
            this.tabPageResponseXml.Controls.Add(this.textBoxResponseXml);
            this.tabPageResponseXml.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseXml.Name = "tabPageResponseXml";
            this.tabPageResponseXml.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseXml.Size = new System.Drawing.Size(508, 204);
            this.tabPageResponseXml.TabIndex = 0;
            this.tabPageResponseXml.Text = "Response (Text Display)";
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
            // tabPageResponseString
            // 
            this.tabPageResponseString.Controls.Add(this.tabControlResponseString);
            this.tabPageResponseString.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseString.Name = "tabPageResponseString";
            this.tabPageResponseString.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseString.Size = new System.Drawing.Size(508, 204);
            this.tabPageResponseString.TabIndex = 1;
            this.tabPageResponseString.Text = "Stream (Text)";
            this.tabPageResponseString.UseVisualStyleBackColor = true;
            // 
            // tabControlResponseString
            // 
            this.tabControlResponseString.Controls.Add(this.tabPageResponseStringAscii);
            this.tabControlResponseString.Controls.Add(this.tabPageResponseStringUtf16);
            this.tabControlResponseString.Controls.Add(this.tabPageResponseStringUtf16BE);
            this.tabControlResponseString.Controls.Add(this.tabPageResponseStringUtf8);
            this.tabControlResponseString.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlResponseString.Location = new System.Drawing.Point(3, 3);
            this.tabControlResponseString.Name = "tabControlResponseString";
            this.tabControlResponseString.SelectedIndex = 0;
            this.tabControlResponseString.Size = new System.Drawing.Size(502, 198);
            this.tabControlResponseString.TabIndex = 0;
            // 
            // tabPageResponseStringAscii
            // 
            this.tabPageResponseStringAscii.Controls.Add(this.textBoxResponseStringAscii);
            this.tabPageResponseStringAscii.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseStringAscii.Name = "tabPageResponseStringAscii";
            this.tabPageResponseStringAscii.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseStringAscii.Size = new System.Drawing.Size(494, 172);
            this.tabPageResponseStringAscii.TabIndex = 0;
            this.tabPageResponseStringAscii.Text = "ASCII";
            this.tabPageResponseStringAscii.UseVisualStyleBackColor = true;
            // 
            // textBoxResponseStringAscii
            // 
            this.textBoxResponseStringAscii.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseStringAscii.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseStringAscii.Multiline = true;
            this.textBoxResponseStringAscii.Name = "textBoxResponseStringAscii";
            this.textBoxResponseStringAscii.ReadOnly = true;
            this.textBoxResponseStringAscii.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseStringAscii.Size = new System.Drawing.Size(488, 166);
            this.textBoxResponseStringAscii.TabIndex = 0;
            // 
            // tabPageResponseStringUtf16
            // 
            this.tabPageResponseStringUtf16.Controls.Add(this.textBoxResponseStringUtf16);
            this.tabPageResponseStringUtf16.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseStringUtf16.Name = "tabPageResponseStringUtf16";
            this.tabPageResponseStringUtf16.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseStringUtf16.Size = new System.Drawing.Size(494, 172);
            this.tabPageResponseStringUtf16.TabIndex = 1;
            this.tabPageResponseStringUtf16.Text = "UTF-16";
            this.tabPageResponseStringUtf16.UseVisualStyleBackColor = true;
            // 
            // textBoxResponseStringUtf16
            // 
            this.textBoxResponseStringUtf16.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseStringUtf16.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseStringUtf16.Multiline = true;
            this.textBoxResponseStringUtf16.Name = "textBoxResponseStringUtf16";
            this.textBoxResponseStringUtf16.ReadOnly = true;
            this.textBoxResponseStringUtf16.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseStringUtf16.Size = new System.Drawing.Size(488, 166);
            this.textBoxResponseStringUtf16.TabIndex = 1;
            // 
            // tabPageResponseStringUtf16BE
            // 
            this.tabPageResponseStringUtf16BE.Controls.Add(this.textBoxResponseStringUtf16BE);
            this.tabPageResponseStringUtf16BE.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseStringUtf16BE.Name = "tabPageResponseStringUtf16BE";
            this.tabPageResponseStringUtf16BE.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseStringUtf16BE.Size = new System.Drawing.Size(494, 172);
            this.tabPageResponseStringUtf16BE.TabIndex = 2;
            this.tabPageResponseStringUtf16BE.Text = "UTF-16 (Big endian)";
            this.tabPageResponseStringUtf16BE.UseVisualStyleBackColor = true;
            // 
            // textBoxResponseStringUtf16BE
            // 
            this.textBoxResponseStringUtf16BE.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseStringUtf16BE.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseStringUtf16BE.Multiline = true;
            this.textBoxResponseStringUtf16BE.Name = "textBoxResponseStringUtf16BE";
            this.textBoxResponseStringUtf16BE.ReadOnly = true;
            this.textBoxResponseStringUtf16BE.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseStringUtf16BE.Size = new System.Drawing.Size(488, 166);
            this.textBoxResponseStringUtf16BE.TabIndex = 1;
            // 
            // tabPageResponseStringUtf8
            // 
            this.tabPageResponseStringUtf8.Controls.Add(this.textBoxResponseStringUtf8);
            this.tabPageResponseStringUtf8.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseStringUtf8.Name = "tabPageResponseStringUtf8";
            this.tabPageResponseStringUtf8.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseStringUtf8.Size = new System.Drawing.Size(494, 172);
            this.tabPageResponseStringUtf8.TabIndex = 3;
            this.tabPageResponseStringUtf8.Text = "UTF-8";
            this.tabPageResponseStringUtf8.UseVisualStyleBackColor = true;
            // 
            // textBoxResponseStringUtf8
            // 
            this.textBoxResponseStringUtf8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResponseStringUtf8.Location = new System.Drawing.Point(3, 3);
            this.textBoxResponseStringUtf8.Multiline = true;
            this.textBoxResponseStringUtf8.Name = "textBoxResponseStringUtf8";
            this.textBoxResponseStringUtf8.ReadOnly = true;
            this.textBoxResponseStringUtf8.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResponseStringUtf8.Size = new System.Drawing.Size(488, 166);
            this.textBoxResponseStringUtf8.TabIndex = 1;
            // 
            // tabPageResponseImage
            // 
            this.tabPageResponseImage.AutoScroll = true;
            this.tabPageResponseImage.Controls.Add(this.labelResponseImageException);
            this.tabPageResponseImage.Controls.Add(this.pictureBoxResponseImage);
            this.tabPageResponseImage.Location = new System.Drawing.Point(4, 22);
            this.tabPageResponseImage.Name = "tabPageResponseImage";
            this.tabPageResponseImage.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResponseImage.Size = new System.Drawing.Size(508, 204);
            this.tabPageResponseImage.TabIndex = 2;
            this.tabPageResponseImage.Text = "Stream (Image)";
            this.tabPageResponseImage.UseVisualStyleBackColor = true;
            // 
            // labelResponseImageException
            // 
            this.labelResponseImageException.AutoSize = true;
            this.labelResponseImageException.Location = new System.Drawing.Point(0, 0);
            this.labelResponseImageException.Name = "labelResponseImageException";
            this.labelResponseImageException.Size = new System.Drawing.Size(42, 13);
            this.labelResponseImageException.TabIndex = 2;
            this.labelResponseImageException.Text = "(Empty)";
            // 
            // pictureBoxResponseImage
            // 
            this.pictureBoxResponseImage.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxResponseImage.Name = "pictureBoxResponseImage";
            this.pictureBoxResponseImage.Size = new System.Drawing.Size(32, 32);
            this.pictureBoxResponseImage.TabIndex = 1;
            this.pictureBoxResponseImage.TabStop = false;
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
            this.comboBoxMethod.Location = new System.Drawing.Point(99, 112);
            this.comboBoxMethod.Name = "comboBoxMethod";
            this.comboBoxMethod.Size = new System.Drawing.Size(420, 21);
            this.comboBoxMethod.TabIndex = 11;
            this.comboBoxMethod.SelectedIndexChanged += new System.EventHandler(this.comboBoxMethod_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Location = new System.Drawing.Point(3, 109);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(90, 27);
            this.label4.TabIndex = 12;
            this.label4.Text = "Method:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 4;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
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
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
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
            this.tabPageResponseString.ResumeLayout(false);
            this.tabControlResponseString.ResumeLayout(false);
            this.tabPageResponseStringAscii.ResumeLayout(false);
            this.tabPageResponseStringAscii.PerformLayout();
            this.tabPageResponseStringUtf16.ResumeLayout(false);
            this.tabPageResponseStringUtf16.PerformLayout();
            this.tabPageResponseStringUtf16BE.ResumeLayout(false);
            this.tabPageResponseStringUtf16BE.PerformLayout();
            this.tabPageResponseStringUtf8.ResumeLayout(false);
            this.tabPageResponseStringUtf8.PerformLayout();
            this.tabPageResponseImage.ResumeLayout(false);
            this.tabPageResponseImage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResponseImage)).EndInit();
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
        private System.Windows.Forms.TabPage tabPageResponseString;
        private System.Windows.Forms.TabPage tabPageResponseImage;
        private System.Windows.Forms.PictureBox pictureBoxResponseImage;
        private System.Windows.Forms.TabControl tabControlResponseString;
        private System.Windows.Forms.TabPage tabPageResponseStringAscii;
        private System.Windows.Forms.TabPage tabPageResponseStringUtf16;
        private System.Windows.Forms.TabPage tabPageResponseStringUtf16BE;
        private System.Windows.Forms.TabPage tabPageResponseStringUtf8;
        private System.Windows.Forms.TextBox textBoxResponseStringAscii;
        private System.Windows.Forms.TextBox textBoxResponseStringUtf16;
        private System.Windows.Forms.TextBox textBoxResponseStringUtf16BE;
        private System.Windows.Forms.TextBox textBoxResponseStringUtf8;
        private System.Windows.Forms.Label labelResponseImageException;
    }
}