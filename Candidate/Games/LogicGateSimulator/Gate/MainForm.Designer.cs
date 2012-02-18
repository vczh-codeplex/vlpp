namespace Gate
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
            this.listViewTools = new Gate.BoardEditorToolBox();
            this.panelBoard = new Gate.BoardEditorPanel();
            this.SuspendLayout();
            // 
            // listViewTools
            // 
            this.listViewTools.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.listViewTools.FullRowSelect = true;
            this.listViewTools.HideSelection = false;
            this.listViewTools.Location = new System.Drawing.Point(12, 12);
            this.listViewTools.MultiSelect = false;
            this.listViewTools.Name = "listViewTools";
            this.listViewTools.Size = new System.Drawing.Size(238, 648);
            this.listViewTools.TabIndex = 0;
            this.listViewTools.UseCompatibleStateImageBehavior = false;
            this.listViewTools.View = System.Windows.Forms.View.Details;
            this.listViewTools.SelectedToolChanged += new System.EventHandler(this.listViewTools_SelectedToolChanged);
            // 
            // panelBoard
            // 
            this.panelBoard.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelBoard.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelBoard.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.panelBoard.Location = new System.Drawing.Point(256, 12);
            this.panelBoard.Name = "panelBoard";
            this.panelBoard.Size = new System.Drawing.Size(564, 648);
            this.panelBoard.TabIndex = 1;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(832, 672);
            this.Controls.Add(this.panelBoard);
            this.Controls.Add(this.listViewTools);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Vczh Gate Simulator";
            this.ResumeLayout(false);

        }

        #endregion

        private Gate.BoardEditorToolBox listViewTools;
        private Gate.BoardEditorPanel panelBoard;
    }
}

