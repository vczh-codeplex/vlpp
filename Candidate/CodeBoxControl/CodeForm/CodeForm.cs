using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CodeBoxControl;
using System.IO;
using System.ServiceModel;

namespace CodeForm
{
    public partial class CodeForm : Form
    {
        private ServiceHost serviceHost = null;

        public CodeForm()
        {
            InitializeComponent();
            textEditorBox.Colorizer = new CSharpColorizer();
            TextEditorService.CodeForm = this;

            this.serviceHost = new ServiceHost(typeof(TextEditorService));
            this.serviceHost.Open();
        }

        private void CodeForm_Shown(object sender, EventArgs e)
        {
            if (File.Exists("CSharpCode.txt"))
            {
                using (StreamReader reader = new StreamReader("CSharpCode.txt"))
                {
                    textEditorBox.Text = reader.ReadToEnd();
                }
            }
        }

        private void CodeForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.serviceHost.Close();
        }
    }
}
