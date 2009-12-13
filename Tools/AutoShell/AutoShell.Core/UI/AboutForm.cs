using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;
using System.IO;
using System.Reflection;

namespace AutoShell.UI
{
    internal partial class AboutForm : Form
    {
        private IServiceContainer serviceContainer;

        public AboutForm(IServiceContainer serviceContainer)
        {
            InitializeComponent();
            this.serviceContainer = serviceContainer;

            string title = null;
            string product = null;
            string company = null;
            string version = null;
            string copyright = null;
            string description = null;

            Assembly entry = Assembly.GetEntryAssembly();
            if (entry != null)
            {
                object[] titleAttribute = entry.GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
                object[] productAttribute = entry.GetCustomAttributes(typeof(AssemblyProductAttribute), false);
                object[] companyAttribute = entry.GetCustomAttributes(typeof(AssemblyCompanyAttribute), false);
                object[] versionAttribute = entry.GetCustomAttributes(typeof(AssemblyFileVersionAttribute), false);
                object[] copyrightAttribute = entry.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false);
                object[] descriptionAttribute = entry.GetCustomAttributes(typeof(AssemblyDescriptionAttribute), false);

                if (titleAttribute.Length > 0)
                {
                    title = (titleAttribute[0] as AssemblyTitleAttribute).Title;
                }
                if (productAttribute.Length > 0)
                {
                    product = (productAttribute[0] as AssemblyProductAttribute).Product;
                }
                if (companyAttribute.Length > 0)
                {
                    company = (companyAttribute[0] as AssemblyCompanyAttribute).Company;
                }
                if (versionAttribute.Length > 0)
                {
                    version = (versionAttribute[0] as AssemblyFileVersionAttribute).Version;
                }
                if (copyrightAttribute.Length > 0)
                {
                    copyright = (copyrightAttribute[0] as AssemblyCopyrightAttribute).Copyright;
                }
                if (descriptionAttribute.Length > 0)
                {
                    description = (descriptionAttribute[0] as AssemblyDescriptionAttribute).Description;
                }
            }
            else
            {
                title = "Vczh AutoShell 2010";
                product = "Vczh AutoShell 2010";
                company = "";
                version = "";
                copyright = "";
                description = "";
            }

            listViewProduct.Items[0].SubItems[1].Text = title;
            listViewProduct.Items[1].SubItems[1].Text = product;
            listViewProduct.Items[2].SubItems[1].Text = company;
            listViewProduct.Items[3].SubItems[1].Text = version;
            listViewProduct.Items[4].SubItems[1].Text = copyright;
            listViewProduct.Items[5].SubItems[1].Text = description;
            labelProduct.Text = title;

            foreach (Package package in this.serviceContainer.GetInstalledPackages())
            {
                Assembly assembly = package.GetType().Assembly;
                ListViewItem item = new ListViewItem();
                item.Text = package.GetType().Name;
                item.ImageIndex = 0;
                item.Tag = package;
                item.SubItems.Add(assembly.FullName);
                item.SubItems.Add(Path.GetFileName(assembly.Location));
                listViewPackages.Items.Add(item);
            }
            listViewPackages.SelectedIndices.Clear();
            listViewPackages.SelectedIndices.Add(0);

            foreach (Type type in this.serviceContainer.GetInstalledServices())
            {
                Assembly assembly = this.serviceContainer.GetServiceByType(type).GetType().Assembly;
                ListViewItem item = new ListViewItem();
                item.Text = type.Name;
                item.ImageIndex = 0;
                item.Tag = type;
                item.SubItems.Add(assembly.FullName);
                item.SubItems.Add(Path.GetFileName(assembly.Location));
                listViewServices.Items.Add(item);
            }
            listViewServices.SelectedIndices.Clear();
            listViewServices.SelectedIndices.Add(0);
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void listViewPackages_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewPackages.SelectedIndices.Count == 0)
            {
                listViewPackageDetail.Items[0].SubItems[1].Text = "";
                listViewPackageDetail.Items[1].SubItems[1].Text = "";
                listViewPackageDetail.Items[2].SubItems[1].Text = "";
            }
            else
            {
                listViewPackageDetail.Items[0].SubItems[1].Text = listViewPackages.SelectedItems[0].Tag.GetType().FullName;
                listViewPackageDetail.Items[1].SubItems[1].Text = listViewPackages.SelectedItems[0].SubItems[1].Text;
                listViewPackageDetail.Items[2].SubItems[1].Text = listViewPackages.SelectedItems[0].SubItems[2].Text;
            }
        }

        private void listViewServices_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewServices.SelectedIndices.Count == 0)
            {
                listViewServiceDetail.Items[0].SubItems[1].Text = "";
                listViewServiceDetail.Items[1].SubItems[1].Text = "";
                listViewServiceDetail.Items[2].SubItems[1].Text = "";
            }
            else
            {
                listViewServiceDetail.Items[0].SubItems[1].Text = (listViewServices.SelectedItems[0].Tag as Type).FullName;
                listViewServiceDetail.Items[1].SubItems[1].Text = listViewServices.SelectedItems[0].SubItems[1].Text;
                listViewServiceDetail.Items[2].SubItems[1].Text = listViewServices.SelectedItems[0].SubItems[2].Text;
            }
        }
    }
}
