using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;

namespace AutoShell.UI.ToolWindow
{
    internal partial class FindResultWindowControl : UserControl
    {
        private List<FindResult> results = new List<FindResult>();

        public FindResultWindowControl()
        {
            InitializeComponent();
        }

        public IUIShell UIShell { get; set; }

        public void SetResults(FindResult[] results)
        {
            ListViewItem[] items = results
                .Select(result => new ListViewItem(new string[]
                {
                    result.FindTarget.Name,
                    result.Line
                }))
                .ToArray();
            listViewFindResult.BeginUpdate();
            listViewFindResult.Items.Clear();
            listViewFindResult.Items.AddRange(items);
            listViewFindResult.EndUpdate();
            this.results.Clear();
            this.results.AddRange(results);
            UIShell.QueryUIStatus();
        }

        public void UpdateResults(FindResult[] results)
        {
            int[] indices = results
                .Select(result => this.results.IndexOf(result))
                .Where(i => i != -1)
                .ToArray();
            listViewFindResult.BeginUpdate();
            for (int i = 0; i < indices.Length; i++)
            {
                FindResult result = results[i];
                ListViewItem item = listViewFindResult.Items[indices[i]];
                item.SubItems[0].Text = result.FindTarget.Name;
                item.SubItems[1].Text = result.Line;
            }
            listViewFindResult.EndUpdate();
            UIShell.QueryUIStatus();
        }

        private int SelectedIndex
        {
            get
            {
                if (listViewFindResult.SelectedIndices.Count == 0)
                {
                    return -1;
                }
                else
                {
                    return listViewFindResult.SelectedIndices[0];
                }
            }
        }

        #region ToolBarItem QueryStatus

        public void itemOpen_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Enabled = this.SelectedIndex != -1;
        }

        public void itemPrevious_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Enabled = this.SelectedIndex != -1 && this.SelectedIndex != 0;
        }

        public void itemNext_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Enabled = this.SelectedIndex != -1 && this.SelectedIndex != this.results.Count - 1;
        }

        public void itemClear_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Enabled = this.results.Count > 0;
        }

        public void itemStop_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Enabled = false;
        }

        #endregion

        #region ToolBarItem Executed

        public void itemOpen_Executed(object sender, EventArgs e)
        {
            FindResult result = this.results[this.SelectedIndex];
            result.FindTarget.Focus(result);
        }

        public void itemPrevious_Executed(object sender, EventArgs e)
        {
            int index = this.SelectedIndex - 1;
            listViewFindResult.SelectedIndices.Clear();
            listViewFindResult.SelectedIndices.Add(index);
        }

        public void itemNext_Executed(object sender, EventArgs e)
        {
            int index = this.SelectedIndex + 1;
            listViewFindResult.SelectedIndices.Clear();
            listViewFindResult.SelectedIndices.Add(index);
        }

        #endregion

        private void listViewFindResult_SelectedIndexChanged(object sender, EventArgs e)
        {
            UIShell.QueryUIStatus();
            if (this.SelectedIndex != -1)
            {
                FindResult result = this.results[this.SelectedIndex];
                result.FindTarget.Focus(result);
                this.listViewFindResult.EnsureVisible(this.SelectedIndex);
            }
        }
    }
}
