using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;

namespace AutoShell.UI
{
    internal partial class NewFileForm : Form
    {
        private IServiceContainer serviceContainer;
        private IEditorService editorService = null;

        public Guid FileType { get; set; }

        public NewFileForm(IServiceContainer serviceContainer)
        {
            InitializeComponent();
            this.serviceContainer = serviceContainer;
            this.editorService = this.serviceContainer.QueryService<IEditorService>();
            this.FileType = Guid.Empty;
            BuildCategories();
        }

        private void BuildCategory(Dictionary<Guid, TreeNode> nodes, Guid categoryGuid)
        {
            FileCategory category = this.editorService.GetCategory(categoryGuid);
            TreeNode node = new TreeNode(category.Name);
            if (category.Parent == Guid.Empty)
            {
                treeViewCategory.Nodes.Add(node);
            }
            else
            {
                nodes[category.Parent].Nodes.Add(node);
            }
            node.Tag = categoryGuid;
            nodes.Add(categoryGuid, node);

            foreach (Guid childGuid in this.editorService.GetFileCategories().Where(guid => this.editorService.GetCategory(guid).Parent == categoryGuid))
            {
                BuildCategory(nodes, childGuid);
            }
        }

        private void BuildCategories()
        {
            Dictionary<Guid, TreeNode> nodes = new Dictionary<Guid, TreeNode>();
            foreach (Guid childGuid in this.editorService.GetFileCategories().Where(guid => this.editorService.GetCategory(guid).Parent == Guid.Empty))
            {
                BuildCategory(nodes, childGuid);
            }
            ShowCategory((Guid)treeViewCategory.Nodes[0].Tag);
        }

        private void ShowCategory(Guid categoryGuid)
        {
            listViewType.Items.Clear();
            imageIcons.Images.Clear();
            FileCategory category = this.editorService.GetCategory(categoryGuid);
            foreach (Guid typeGuid in category.FileTypes)
            {
                FileType fileType = this.editorService.GetFileType(typeGuid);
                if (fileType.Icon == null)
                {
                    imageIcons.Images.Add(Icons.UtilityText);
                }
                else
                {
                    imageIcons.Images.Add(fileType.Icon);
                }
                ListViewItem item = new ListViewItem(fileType.FileTypeName);
                item.ImageIndex = imageIcons.Images.Count - 1;
                item.SubItems.Add(fileType.FileTypeDescription);
                item.Tag = typeGuid;
                listViewType.Items.Add(item);
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void treeViewCategory_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node != null)
            {
                ShowCategory((Guid)e.Node.Tag);
            }
        }

        private void listViewType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewType.SelectedIndices.Count == 0)
            {
                this.FileType = Guid.Empty;
                textBoxDescription.Text = "";
                buttonOK.Enabled = false;
            }
            else
            {
                this.FileType = (Guid)listViewType.SelectedItems[0].Tag;
                textBoxDescription.Text = this.editorService.GetFileType(this.FileType).FileTypeDescription;
                buttonOK.Enabled = true;
            }
        }
    }
}
