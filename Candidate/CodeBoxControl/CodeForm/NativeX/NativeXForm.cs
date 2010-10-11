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
using CodeBoxControl.Core;

namespace CodeForm
{
    public partial class NativeXForm : Form
    {
        private ServiceHost serviceHost = null;

        public NativeXForm()
        {
            InitializeComponent();
            textEditorBox.Colorizer = new NativeXColorizer();
            TextEditorService.EditorControl = textEditorBox;
            this.serviceHost = new ServiceHost(typeof(TextEditorService));
            this.serviceHost.Open();
        }

        private void NativeXForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.serviceHost.Close();
        }

        private void NativeXForm_Shown(object sender, EventArgs e)
        {
            if (File.Exists("NativeXCode.txt"))
            {
                using (StreamReader reader = new StreamReader("NativeXCode.txt"))
                {
                    textEditorBox.Text = reader.ReadToEnd();
                }
            }
        }
    }
}
