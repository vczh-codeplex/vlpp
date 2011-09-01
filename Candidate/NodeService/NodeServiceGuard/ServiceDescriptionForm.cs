using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Linq;

namespace NodeServiceGuard
{
    public partial class ServiceDescriptionForm : Form
    {
        public ServiceDescriptionForm()
        {
            InitializeComponent();
        }

        public DialogResult ShowDescription(XElement description)
        {
            this.textBoxDescription.Text = description.ToString();
            return ShowDialog();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            Close();
        }
    }
}
