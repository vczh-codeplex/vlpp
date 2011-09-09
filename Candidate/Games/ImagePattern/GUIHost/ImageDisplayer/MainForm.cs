using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ImagePattern;

namespace ImageDisplayer
{
    public partial class MainForm : Form
    {
        private Bitmap imageNormal = new Bitmap(32, 32);
        private Bitmap imageDifferent = new Bitmap(32, 32);

        private void ShowImage(Bitmap image)
        {
            pictureBoxImage.Visible = false;
            pictureBoxImage.Image = image;
            pictureBoxImage.Size = image.Size;
            pictureBoxImage.Visible = true;
        }

        private void ReplaceImage(ref Bitmap oldImage, Bitmap newImage)
        {
            oldImage.Dispose();
            oldImage = newImage;
        }

        private void LoadImage(Bitmap image)
        {
            pictureBoxImage.Visible = false;
            pictureBoxImage.Image = null;

            ImageData imageData = new ImageData(image);
            Bitmap normal = imageData.GetBitmap();

            ImageData differentImageData = imageData.DifferentAbsAndCopy();
            Bitmap different = differentImageData.GetBitmap();

            ReplaceImage(ref this.imageNormal, normal);
            ReplaceImage(ref this.imageDifferent, different);

            pictureBoxImage.Image = this.imageNormal;
            pictureBoxImage.Size = this.imageNormal.Size;
            pictureBoxImage.Visible = true;
        }

        public MainForm()
        {
            InitializeComponent();
            ShowImage(this.imageNormal);
        }

        private void buttonNormal_Click(object sender, EventArgs e)
        {
            ShowImage(this.imageNormal);
        }

        private void buttonDifferent_Click(object sender, EventArgs e)
        {
            ShowImage(this.imageDifferent);
        }

        private void buttonPaste_Click(object sender, EventArgs e)
        {
            if (Clipboard.ContainsImage())
            {
                using (Image clipboardImage = Clipboard.GetImage())
                using (Bitmap image = new Bitmap(clipboardImage))
                {
                    LoadImage(image);
                }
            }
            else
            {
                MessageBox.Show("Clipboard contains no image.");
            }
        }
    }
}
