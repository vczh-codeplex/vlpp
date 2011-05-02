using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace Jumper.Playing
{
    abstract class WorldObject : IDisposable
    {
        public WorldObject AdditionalObject { get; set; }

        public abstract Rectangle Bounds { get; }
        public abstract void AttachWorld(World world);
        public abstract void Step();
        public abstract void Paint(Graphics graphics);
        public abstract void Dispose();
        public abstract bool ContactPlayer(bool falling);

        #region API

        protected Point GetRenderingPosition(World world)
        {
            int y = world.WorldTop - this.Bounds.Top;
            return new Point(this.Bounds.Left, y);
        }

        protected Bitmap Initialize(Bitmap original)
        {
            if (original.PixelFormat == PixelFormat.Format32bppArgb
                || original.PixelFormat == PixelFormat.Format32bppPArgb
                || original.PixelFormat == PixelFormat.Format16bppArgb1555)
            {
                Bitmap bitmap = new Bitmap(original);
                original.Dispose();
                return bitmap;
            }
            else
            {
                Bitmap bitmap = new Bitmap(original.Width, original.Height, PixelFormat.Format32bppArgb);
                using (Graphics graphics = Graphics.FromImage(bitmap))
                {
                    graphics.DrawImage(original, 0, 0);
                }
                original.Dispose();

                var data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                for (int i = 0; i < data.Height; i++)
                {
                    IntPtr scanline = data.Scan0 + i * data.Width * 4;
                    for (int j = 0; j < data.Width; j++)
                    {
                        IntPtr pcolor = scanline + j * 4;
                        Int32 color = Marshal.ReadInt32(pcolor);
                        if (color == -1)
                        {
                            Marshal.WriteInt32(pcolor, 0);
                        }
                    }
                }
                bitmap.UnlockBits(data);
                return bitmap;
            }
        }

        #endregion
    }
}
