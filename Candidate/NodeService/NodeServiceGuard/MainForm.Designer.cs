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
            this.timerRestart = new System.Windows.Forms.Timer(this.components);
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
            this.listViewServices.Location = new System.Drawing.Point(12, 12);
            this.listViewServices.Name = "listViewServices";
            this.listViewServices.Size = new System.Drawing.Size(630, 606);
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
            // timerRestart
            // 
            this.timerRestart.Enabled = true;
            this.timerRestart.Interval = 1000;
            this.timerRestart.Tick += new System.EventHandler(this.timerRestart_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(654, 630);
            this.Controls.Add(this.listViewServices);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Node Service Guard v1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewServices;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.Timer timerRestart;
    }
}

