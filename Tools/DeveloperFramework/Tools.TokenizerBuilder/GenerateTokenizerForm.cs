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
    public partial class GenerateTokenizerForm : Form
    {
        public GenerateTokenizerForm()
        {
            InitializeComponent();
        }

        public string NamespaceName
        {
            get
            {
                return textNamespace.Text;
            }
        }

        public string ClassName
        {
            get
            {
                return textClassName.Text;
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            Close();
        }
    }
}
