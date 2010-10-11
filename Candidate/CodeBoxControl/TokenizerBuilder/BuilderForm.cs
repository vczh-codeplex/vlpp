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

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dialogOpen.ShowDialog() == DialogResult.OK)
            {
                tokenizerEditorBox.LoadFromFile(dialogOpen.FileName);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dialogSave.ShowDialog() == DialogResult.OK)
            {
                tokenizerEditorBox.SaveToFile(dialogSave.FileName);
            }
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            tokenizerEditorBox.Clear();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void colorizerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (GenerateColorizerForm form = new GenerateColorizerForm())
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        Clipboard.SetText(ColorizerCodeGenerator.GenerateCSharpCode(tokenizerEditorBox.Document, form.NamespaceName, form.ClassName, form.CommentOutColors));
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, this.Text);
                    }
                }
            }
        }
    }
}
