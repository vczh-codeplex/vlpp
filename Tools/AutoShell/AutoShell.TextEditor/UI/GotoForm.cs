using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoShell.UI
{
    internal partial class GotoForm : Form
    {
        public GotoForm(int maxLine)
        {
            InitializeComponent();
            labelLine.Text = string.Format(Strings.GotoFormLabelText, maxLine);
            numericLine.Maximum = maxLine;
        }

        public int LineNumber
        {
            get
            {
                return (int)numericLine.Value - 1;
            }
            set
            {
                numericLine.Value = value + 1;
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
    }
}
