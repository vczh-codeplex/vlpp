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
    internal partial class EditingWindowsForm : Form
    {
        private IServiceContainer serviceContainer = null;
        private IEditorService editorService = null;

        public EditingWindowsForm(IServiceContainer serviceContainer)
        {
            InitializeComponent();
            this.serviceContainer = serviceContainer;
            this.editorService = this.serviceContainer.QueryService<IEditorService>();
            foreach (ToolWindowPane pane in this.editorService.GetEditorWindows())
            {
                IEditorWindow editorWindow = pane as IEditorWindow;
                if (editorWindow != null)
                {
                    ListViewItem item = new ListViewItem(pane.Caption);
                    item.SubItems.Add(editorWindow.FileName == null ? "" : editorWindow.FileName);
                    item.Tag = pane;
                    listViewWindows.Items.Add(item);
                }
            }
        }

        private void buttonActive_Click(object sender, EventArgs e)
        {
            ToolWindowPane pane = listViewWindows.SelectedItems[0].Tag as ToolWindowPane;
            pane.Open();
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listViewWindows.SelectedItems)
            {
                ToolWindowPane pane = item.Tag as ToolWindowPane;
                this.editorService.SaveEditingContent(pane, false);
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            ListViewItem[] items = listViewWindows.SelectedItems.OfType<ListViewItem>().ToArray();
            foreach (ListViewItem item in items)
            {
                ToolWindowPane pane = item.Tag as ToolWindowPane;
                if (pane.Close())
                {
                    listViewWindows.Items.Remove(item);
                }
            }
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            buttonActive.Enabled = listViewWindows.SelectedItems.Count == 1;
            buttonSave.Enabled = listViewWindows.SelectedItems.Count >= 1;
            buttonClose.Enabled = listViewWindows.SelectedItems.Count >= 1;
        }
    }
}
