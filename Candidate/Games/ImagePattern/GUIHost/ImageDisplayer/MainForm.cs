using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ImagePattern;
using System.Drawing.Drawing2D;

namespace ImageDisplayer
{
    public partial class MainForm : Form
    {
        private const int ZoomScale = 3;

        private Dictionary<string, Bitmap> imageMap = new Dictionary<string, Bitmap>();

        private void ShowImage(Bitmap image)
        {
            pictureBoxImage.Visible = false;
            pictureBoxImage.Image = image;
            pictureBoxImage.Size = image.Size;
            pictureBoxImage.Visible = true;
        }

        private void GenerateImage(ImageData imageData)
        {
            string caption = this.Text;

            this.Text = caption + " Normal";
            this.imageMap["(01) Normal"] = imageData.GetBitmap();

            this.Text = caption + " Different";
            imageData = imageData.DifferentAbsAndCopy(2);
            this.imageMap["(02) Different"] = imageData.GetBitmap();

            this.Text = caption + " Zip";
            imageData = imageData.ZipAndCopy();
            this.imageMap["(03) Zip"] = imageData.GetBitmap();

            this.Text = caption + " Reduce";
            imageData.ReduceColorRange(4, 251);
            this.imageMap["(04) Reduce"] = imageData.GetBitmap();

            imageData = imageData.EnlargeColorRangeAndCopy(20);
            this.imageMap["(05) Enlarge"] = imageData.GetBitmap();

            this.Text = caption + " Reduce";
            imageData.ReduceColorRange(8, 247);
            this.imageMap["(06) Reduce"] = imageData.GetBitmap();

            this.Text = caption + " Booling";
            imageData = imageData.EnlargeColorRangeAndCopy(5);
            this.imageMap["(07) Enlarge"] = imageData.GetBitmap();

            imageData.Booling(96);
            this.imageMap["(08) Booling"] = imageData.GetBitmap();

            this.Text = caption;
        }

        private void LoadImage(Bitmap image)
        {
            pictureBoxImage.Visible = false;
            pictureBoxImage.Image = null;

            comboBoxImage.Items.Clear();
            foreach (var bitmap in this.imageMap.Values)
            {
                bitmap.Dispose();
            }
            this.imageMap.Clear();

            using (Bitmap largeImage = new Bitmap(image.Width * ZoomScale, image.Height * ZoomScale))
            {
                using (Graphics g = Graphics.FromImage(largeImage))
                {
                    g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                    g.PixelOffsetMode = PixelOffsetMode.Half;
                    g.DrawImage(image, new Rectangle(new Point(0, 0), largeImage.Size));
                }
                ImageData largeImageData = new ImageData(largeImage);
                GenerateImage(largeImageData);
            }
            comboBoxImage.Items.AddRange(imageMap.Keys.ToArray());
            comboBoxImage.SelectedIndex = 0;
        }

        public MainForm()
        {
            InitializeComponent();
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

        private void comboBoxImage_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxImage.SelectedIndex != -1)
            {
                ShowImage(this.imageMap[comboBoxImage.SelectedItem.ToString()]);
            }
        }
    }
}
