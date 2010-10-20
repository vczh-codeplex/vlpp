using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Tools.TokenizerBuilder
{
    public partial class StringEditorForm : Form
    {
        public StringEditorForm()
        {
            InitializeComponent();
        }

        public string Value
        {
            get
            {
                return textValue.Text;
            }
            set
            {
                textValue.Text = value;
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
