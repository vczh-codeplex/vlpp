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

namespace CodeForm
{
    public partial class CodeForm : Form
    {
        public CodeForm()
        {
            InitializeComponent();
            textEditorBox.Colorizer = new CSharpColorizer();
        }

        private void CodeForm_Shown(object sender, EventArgs e)
        {
            using (StreamReader reader = new StreamReader("CSharpCode.txt"))
            {
                textEditorBox.Text = reader.ReadToEnd();
            }
        }
    }
}
