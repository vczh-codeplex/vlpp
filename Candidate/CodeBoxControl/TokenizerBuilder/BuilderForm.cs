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
    public partial class BuilderForm : Form
    {
        public BuilderForm()
        {
            InitializeComponent();
        }

        private void buttonOpen_Click(object sender, EventArgs e)
        {
            if (dialogOpen.ShowDialog() == DialogResult.OK)
            {
                tokenizerEditorBox.LoadFromFile(dialogOpen.FileName);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (dialogSave.ShowDialog() == DialogResult.OK)
            {
                tokenizerEditorBox.SaveToFile(dialogSave.FileName);
            }
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            tokenizerEditorBox.Clear();
        }

        private void buttonGenerate_Click(object sender, EventArgs e)
        {
            try
            {
                Clipboard.SetText(ColorizerCodeGenerator.GenerateCSharpCode(tokenizerEditorBox.Document, "Colorizer"));
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, this.Text);
            }
        }
    }
}
