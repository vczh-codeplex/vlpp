using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace ImagePattern
{
    public class ImageData
    {
        public byte[] RawData { get; private set; }
        public int Width { get; private set; }
        public int Height { get; private set; }
        public int Stride { get; private set; }
        public int ColorSize { get; private set; }
        public bool SingleByte { get; private set; }

        private void Resize(int width, int height, bool singleByte)
        {
            this.Width = width;
            this.Height = height;
            this.SingleByte = singleByte;
            this.ColorSize = singleByte ? 1 : 3;
            this.Stride = width * this.ColorSize;
            this.RawData = new byte[this.Stride * this.Height];
        }

        public ImageData(Bitmap bitmap)
        {
            Resize(bitmap.Width, bitmap.Height, false);

            BitmapData data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            for (int y = 0; y < bitmap.Height; y++)
            {
                IntPtr read = data.Scan0 + y * data.Stride;
                Marshal.Copy(read, this.RawData, y * this.Stride, this.Stride);
            }
            bitmap.UnlockBits(data);
        }

        public ImageData(int width, int height, bool singleByte)
        {
            Resize(width, height, singleByte);
        }

        public ImageData(ImageData data)
        {
            Resize(data.Width, data.Height, data.SingleByte);
            Array.Copy(data.RawData, this.RawData, data.RawData.Length);
        }

        public Bitmap GetBitmap()
        {
            if (this.SingleByte)
            {
                return this.UnzipAndCopy().GetBitmap();
            }
            else
            {
                Bitmap bitmap = new Bitmap(this.Width, this.Height);
                BitmapData data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);
                for (int y = 0; y < bitmap.Height; y++)
                {
                    IntPtr write = data.Scan0 + y * data.Stride;
                    Marshal.Copy(this.RawData, y * this.Stride, write, this.Stride);
                }
                bitmap.UnlockBits(data);
                return bitmap;
            }
        }

    };
}
