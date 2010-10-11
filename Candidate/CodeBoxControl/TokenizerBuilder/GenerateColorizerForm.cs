using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TokenizerBuilder
{
    public partial class GenerateColorizerForm : Form
    {
        public GenerateColorizerForm()
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

        public bool CommentOutColors
        {
            get
            {
                return checkComment.Checked;
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
