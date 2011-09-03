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
            this.SuspendLayout();
            // 
            // textBoxAddress
            // 
            this.textBoxAddress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxAddress.Location = new System.Drawing.Point(125, 12);
            this.textBoxAddress.Name = "textBoxAddress";
            this.textBoxAddress.Size = new System.Drawing.Size(416, 21);
            this.textBoxAddress.TabIndex = 0;
            // 
            // textBoxProtocolAddress
            // 
            this.textBoxProtocolAddress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxProtocolAddress.Location = new System.Drawing.Point(125, 39);
            this.textBoxProtocolAddress.Name = "textBoxProtocolAddress";
            this.textBoxProtocolAddress.ReadOnly = true;
            this.textBoxProtocolAddress.Size = new System.Drawing.Size(416, 21);
            this.textBoxProtocolAddress.TabIndex = 1;
            // 
            // textBoxEndpointName
            // 
            this.textBoxEndpointName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxEndpointName.Location = new System.Drawing.Point(125, 66);
            this.textBoxEndpointName.Name = "textBoxEndpointName";
            this.textBoxEndpointName.ReadOnly = true;
            this.textBoxEndpointName.Size = new System.Drawing.Size(416, 21);
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
            this.buttonClose.Location = new System.Drawing.Point(466, 538);
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
            this.buttonConnect.Location = new System.Drawing.Point(466, 93);
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
            // ServiceTestClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonClose;
            this.ClientSize = new System.Drawing.Size(553, 573);
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
    }
}