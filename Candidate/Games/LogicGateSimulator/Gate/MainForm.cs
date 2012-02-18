using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Gate
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void listViewTools_SelectedToolChanged(object sender, EventArgs e)
        {
            var tool = listViewTools.SelectedTool;
            if (tool != null)
            {
                panelBoard.CurrentCommand = tool.CreateCommand();
            }
        }
    }
}
