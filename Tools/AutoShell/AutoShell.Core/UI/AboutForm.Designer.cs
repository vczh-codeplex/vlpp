namespace AutoShell.UI
{
    partial class AboutForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutForm));
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabAbout = new System.Windows.Forms.TabControl();
            this.tabPageProduct = new System.Windows.Forms.TabPage();
            this.listViewProduct = new System.Windows.Forms.ListView();
            this.columnHeader11 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader12 = new System.Windows.Forms.ColumnHeader();
            this.tabPagePackages = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.listViewPackages = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader9 = new System.Windows.Forms.ColumnHeader();
            this.imageIcon = new System.Windows.Forms.ImageList(this.components);
            this.listViewPackageDetail = new System.Windows.Forms.ListView();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
            this.tagPageServices = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.listViewServices = new System.Windows.Forms.ListView();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader10 = new System.Windows.Forms.ColumnHeader();
            this.listViewServiceDetail = new System.Windows.Forms.ListView();
            this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader8 = new System.Windows.Forms.ColumnHeader();
            this.buttonClose = new System.Windows.Forms.Button();
            this.labelProduct = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabAbout.SuspendLayout();
            this.tabPageProduct.SuspendLayout();
            this.tabPagePackages.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tagPageServices.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // pictureBox1
            // 
            resources.ApplyResources(this.pictureBox1, "pictureBox1");
            this.pictureBox1.InitialImage = null;
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.TabStop = false;
            // 
            // tabAbout
            // 
            resources.ApplyResources(this.tabAbout, "tabAbout");
            this.tabAbout.Controls.Add(this.tabPageProduct);
            this.tabAbout.Controls.Add(this.tabPagePackages);
            this.tabAbout.Controls.Add(this.tagPageServices);
            this.tabAbout.Name = "tabAbout";
            this.tabAbout.SelectedIndex = 0;
            // 
            // tabPageProduct
            // 
            this.tabPageProduct.Controls.Add(this.listViewProduct);
            this.tabPageProduct.Controls.Add(this.label1);
            resources.ApplyResources(this.tabPageProduct, "tabPageProduct");
            this.tabPageProduct.Name = "tabPageProduct";
            this.tabPageProduct.UseVisualStyleBackColor = true;
            // 
            // listViewProduct
            // 
            this.listViewProduct.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader11,
            this.columnHeader12});
            resources.ApplyResources(this.listViewProduct, "listViewProduct");
            this.listViewProduct.FullRowSelect = true;
            this.listViewProduct.GridLines = true;
            this.listViewProduct.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items1"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items2"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items3"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items4"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewProduct.Items5")))});
            this.listViewProduct.MultiSelect = false;
            this.listViewProduct.Name = "listViewProduct";
            this.listViewProduct.UseCompatibleStateImageBehavior = false;
            this.listViewProduct.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader11
            // 
            resources.ApplyResources(this.columnHeader11, "columnHeader11");
            // 
            // columnHeader12
            // 
            resources.ApplyResources(this.columnHeader12, "columnHeader12");
            // 
            // tabPagePackages
            // 
            this.tabPagePackages.Controls.Add(this.tableLayoutPanel2);
            resources.ApplyResources(this.tabPagePackages, "tabPagePackages");
            this.tabPagePackages.Name = "tabPagePackages";
            this.tabPagePackages.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel2
            // 
            resources.ApplyResources(this.tableLayoutPanel2, "tableLayoutPanel2");
            this.tableLayoutPanel2.Controls.Add(this.listViewPackages, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.listViewPackageDetail, 0, 1);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            // 
            // listViewPackages
            // 
            this.listViewPackages.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader9});
            resources.ApplyResources(this.listViewPackages, "listViewPackages");
            this.listViewPackages.LargeImageList = this.imageIcon;
            this.listViewPackages.MultiSelect = false;
            this.listViewPackages.Name = "listViewPackages";
            this.listViewPackages.UseCompatibleStateImageBehavior = false;
            this.listViewPackages.View = System.Windows.Forms.View.Tile;
            this.listViewPackages.SelectedIndexChanged += new System.EventHandler(this.listViewPackages_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            resources.ApplyResources(this.columnHeader1, "columnHeader1");
            // 
            // columnHeader2
            // 
            resources.ApplyResources(this.columnHeader2, "columnHeader2");
            // 
            // columnHeader9
            // 
            resources.ApplyResources(this.columnHeader9, "columnHeader9");
            // 
            // imageIcon
            // 
            this.imageIcon.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageIcon.ImageStream")));
            this.imageIcon.TransparentColor = System.Drawing.Color.Teal;
            this.imageIcon.Images.SetKeyName(0, "AboutSmall.png");
            // 
            // listViewPackageDetail
            // 
            this.listViewPackageDetail.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6});
            resources.ApplyResources(this.listViewPackageDetail, "listViewPackageDetail");
            this.listViewPackageDetail.FullRowSelect = true;
            this.listViewPackageDetail.GridLines = true;
            this.listViewPackageDetail.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewPackageDetail.Items"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewPackageDetail.Items1"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewPackageDetail.Items2")))});
            this.listViewPackageDetail.MultiSelect = false;
            this.listViewPackageDetail.Name = "listViewPackageDetail";
            this.listViewPackageDetail.UseCompatibleStateImageBehavior = false;
            this.listViewPackageDetail.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader5
            // 
            resources.ApplyResources(this.columnHeader5, "columnHeader5");
            // 
            // columnHeader6
            // 
            resources.ApplyResources(this.columnHeader6, "columnHeader6");
            // 
            // tagPageServices
            // 
            this.tagPageServices.Controls.Add(this.tableLayoutPanel1);
            resources.ApplyResources(this.tagPageServices, "tagPageServices");
            this.tagPageServices.Name = "tagPageServices";
            this.tagPageServices.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel1
            // 
            resources.ApplyResources(this.tableLayoutPanel1, "tableLayoutPanel1");
            this.tableLayoutPanel1.Controls.Add(this.listViewServices, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.listViewServiceDetail, 0, 1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            // 
            // listViewServices
            // 
            this.listViewServices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader10});
            resources.ApplyResources(this.listViewServices, "listViewServices");
            this.listViewServices.LargeImageList = this.imageIcon;
            this.listViewServices.MultiSelect = false;
            this.listViewServices.Name = "listViewServices";
            this.listViewServices.UseCompatibleStateImageBehavior = false;
            this.listViewServices.View = System.Windows.Forms.View.Tile;
            this.listViewServices.SelectedIndexChanged += new System.EventHandler(this.listViewServices_SelectedIndexChanged);
            // 
            // columnHeader3
            // 
            resources.ApplyResources(this.columnHeader3, "columnHeader3");
            // 
            // columnHeader4
            // 
            resources.ApplyResources(this.columnHeader4, "columnHeader4");
            // 
            // columnHeader10
            // 
            resources.ApplyResources(this.columnHeader10, "columnHeader10");
            // 
            // listViewServiceDetail
            // 
            this.listViewServiceDetail.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader7,
            this.columnHeader8});
            resources.ApplyResources(this.listViewServiceDetail, "listViewServiceDetail");
            this.listViewServiceDetail.FullRowSelect = true;
            this.listViewServiceDetail.GridLines = true;
            this.listViewServiceDetail.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewServiceDetail.Items"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewServiceDetail.Items1"))),
            ((System.Windows.Forms.ListViewItem)(resources.GetObject("listViewServiceDetail.Items2")))});
            this.listViewServiceDetail.MultiSelect = false;
            this.listViewServiceDetail.Name = "listViewServiceDetail";
            this.listViewServiceDetail.UseCompatibleStateImageBehavior = false;
            this.listViewServiceDetail.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader7
            // 
            resources.ApplyResources(this.columnHeader7, "columnHeader7");
            // 
            // columnHeader8
            // 
            resources.ApplyResources(this.columnHeader8, "columnHeader8");
            // 
            // buttonClose
            // 
            resources.ApplyResources(this.buttonClose, "buttonClose");
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // labelProduct
            // 
            resources.ApplyResources(this.labelProduct, "labelProduct");
            this.labelProduct.Name = "labelProduct";
            // 
            // AboutForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.labelProduct);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.tabAbout);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutForm";
            this.ShowInTaskbar = false;
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabAbout.ResumeLayout(false);
            this.tabPageProduct.ResumeLayout(false);
            this.tabPagePackages.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tagPageServices.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TabControl tabAbout;
        private System.Windows.Forms.TabPage tabPageProduct;
        private System.Windows.Forms.TabPage tabPagePackages;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.ListView listViewPackages;
        private System.Windows.Forms.ImageList imageIcon;
        private System.Windows.Forms.TabPage tagPageServices;
        private System.Windows.Forms.ListView listViewServices;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ListView listViewPackageDetail;
        private System.Windows.Forms.ListView listViewServiceDetail;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.ColumnHeader columnHeader9;
        private System.Windows.Forms.ColumnHeader columnHeader10;
        private System.Windows.Forms.ListView listViewProduct;
        private System.Windows.Forms.ColumnHeader columnHeader11;
        private System.Windows.Forms.ColumnHeader columnHeader12;
        private System.Windows.Forms.Label labelProduct;
    }
}