using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Runtime.InteropServices;

namespace ShaofeiCaptcha
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Copy(byte[,] destPixels, byte[,] sourcePixels, int width, int height)
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    destPixels[y, x] = sourcePixels[y, x];
                }
            }
        }

        private void Search(byte[,] destPixels, byte[,] sourcePixels, int width, int height, int range, Func<int, int, int, byte> transformation)
        {
            for (int y = range; y < height - range; y++)
            {
                for (int x = range; x < width - range; x++)
                {
                    int counter = 0;
                    for (int i = -range; i <= range; i++)
                    {
                        for (int j = -range; j <= range; j++)
                        {
                            counter += sourcePixels[y + i, x + j];
                        }
                    }
                    destPixels[y, x] = transformation(x, y, counter);
                }
            }
        }

        private Bitmap Process(Bitmap sourceBitmap)
        {
            var destBitmap = new Bitmap(sourceBitmap);
            var sourceData = sourceBitmap.LockBits(new Rectangle(0, 0, sourceBitmap.Width, sourceBitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
            var destData = destBitmap.LockBits(new Rectangle(0, 0, sourceBitmap.Width, sourceBitmap.Height), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);

            byte[,] sourcePixels = new byte[sourceBitmap.Height, sourceBitmap.Width];
            byte[,] destPixels = new byte[sourceBitmap.Height, sourceBitmap.Width];

            for (int y = 0; y < sourceBitmap.Height; y++)
            {
                IntPtr read = sourceData.Scan0 + y * sourceData.Stride;
                for (int x = 0; x < sourceBitmap.Width; x++)
                {
                    sourcePixels[y, x] = Marshal.ReadByte(read + x);
                }
            }

            Search(destPixels, sourcePixels, sourceBitmap.Width, sourceBitmap.Height, 3,
                (x, y, c) => (byte)(sourcePixels[y, x] == 0 | c < 6 ? 0 : 1)
                );
            Copy(sourcePixels, destPixels, sourceBitmap.Width, sourceBitmap.Height);
            Search(destPixels, sourcePixels, sourceBitmap.Width, sourceBitmap.Height, 2,
                (x, y, c) => (byte)(c < 4 ? 0 : 1)
                );

            Copy(sourcePixels, destPixels, sourceBitmap.Width, sourceBitmap.Height);
            Search(destPixels, sourcePixels, sourceBitmap.Width, sourceBitmap.Height, 1,
                (x, y, c) => (byte)(sourcePixels[y, x] == 0 || c < 7 ? 0 : 1)
                );

            Copy(sourcePixels, destPixels, sourceBitmap.Width, sourceBitmap.Height);
            Search(destPixels, sourcePixels, sourceBitmap.Width, sourceBitmap.Height, 3,
                (x, y, c) => (byte)(sourcePixels[y, x] == 0 || c < 16 ? 0 : 1)
                );

            Search(destPixels, destPixels, sourceBitmap.Width, sourceBitmap.Height, 0,
                (x, y, c) => (byte)(c * 255)
                );

            for (int y = 0; y < sourceBitmap.Height; y++)
            {
                IntPtr write = destData.Scan0 + y * destData.Stride;
                for (int x = 0; x < sourceBitmap.Width; x++)
                {
                    Marshal.WriteByte(write + x, destPixels[y, x]);
                }
            }

            sourceBitmap.UnlockBits(sourceData);
            destBitmap.UnlockBits(destData);

            //using (var g = Graphics.FromImage(destBitmap))
            //{
            //    int[] counters = Enumerable.Range(0, sourceBitmap.Width)
            //        .Select(x => Enumerable.Range(0, sourceBitmap.Height)
            //            .Select(y => destPixels[y, x] / 255)
            //            .Sum()
            //            )
            //        .ToArray();
            //    int max = counters.Max();
            //    double[] factors = counters.Select(c => (double)c / max).ToArray();

            //    Point[] points = factors
            //        .Select(f => (int)(f * (sourceBitmap.Height - 4)))
            //        .Select((f, i) => new Point(i, sourceBitmap.Height - f - 2))
            //        .ToArray();
            //    g.DrawLines(Pens.Yellow, points);
            //}

            return destBitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(@"http://shaofei.name/captcha.php");
            request.Method = "GET";
            var response = request.GetResponse();
            using (var stream = response.GetResponseStream())
            {
                var bitmap = new Bitmap(stream);
                pictureBox1.Image = bitmap;
                pictureBox2.Image = Process(bitmap);
            }
            response.Close();
        }
    }
}
