using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;

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
                        MessageBox.Show("Colorizer generated in the Clipboard.", this.Text);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, this.Text);
                    }
                }
            }
        }

        private void exportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dialogExport.ShowDialog() == DialogResult.OK)
            {
                using (Bitmap bitmap = tokenizerEditorBox.SaveToBitmap())
                {
                    switch (Path.GetExtension(dialogExport.FileName).ToUpper())
                    {
                        case ".BMP":
                            bitmap.Save(dialogExport.FileName, ImageFormat.Bmp);
                            break;
                        case ".JPG":
                            bitmap.Save(dialogExport.FileName, ImageFormat.Jpeg);
                            break;
                        case ".GIF":
                            bitmap.Save(dialogExport.FileName, ImageFormat.Gif);
                            break;
                        case ".PNG":
                            bitmap.Save(dialogExport.FileName, ImageFormat.Png);
                            break;
                    }
                }
            }
        }

        private void tokenizerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (GenerateTokenizerForm form = new GenerateTokenizerForm())
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        Clipboard.SetText(TokenizerCodeGenerator.GenerateCSharpCode(tokenizerEditorBox.Document, form.NamespaceName, form.ClassName));
                        MessageBox.Show("Colorizer generated in the Clipboard.", this.Text);
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
