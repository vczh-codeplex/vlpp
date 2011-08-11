using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation;
using System.Drawing.Imaging;
using System.Drawing;

namespace FvGUI
{
    partial class MainForm
    {
        private void RenderFunction()
        {
            int w = this.imageBuffer.Width - 2;
            int h = this.imageBuffer.Height - 2;
            int cx = this.imageBuffer.Width / 2;
            int cy = this.imageBuffer.Height / 2;
            double[, ,] points = new double[h, w, 4];

            for (int y = 1; y <= h; y++)
            {
                double py = this.originY + (double)(y - cy) / this.unitPixels;
                for (int x = 1; x <= w; x++)
                {
                    double px = this.originX + (double)(cx - x) / this.unitPixels;
                    points[y - 1, x - 1, 0] = px;
                    points[y - 1, x - 1, 1] = py;
                    points[y - 1, x - 1, 2] = px;
                    points[y - 1, x - 1, 3] = py;
                }
            }

            for (int y = 1; y <= h; y++)
            {
                double py = this.originY + (double)(y - cy) / this.unitPixels;
                Expression fx = this.function.Apply("y", py).Simplify();
                Expression dfx = fx.Different("x").Simplify();
                for (int x = 1; x <= w; x++)
                {
                    points[y - 1, x - 1, 0] = fx.Solve(dfx, "x", points[y - 1, x - 1, 0]);
                }
                if (y % 10 == 0)
                {
                    labelErrorMessage.Text = y.ToString() + "/" + (w + h).ToString();
                    labelErrorMessage.Refresh();
                }
            }

            for (int x = 1; x <= w; x++)
            {
                double px = this.originX + (double)(cx - x) / this.unitPixels;
                Expression fy = this.function.Apply("x", px).Simplify();
                Expression dfy = fy.Different("y").Simplify();
                for (int y = 1; y <= h; y++)
                {
                    points[y - 1, x - 1, 3] = fy.Solve(dfy, "y", points[y - 1, x - 1, 3]);
                }
                if (x % 10 == 0)
                {
                    labelErrorMessage.Text = (h + x).ToString() + "/" + (w + h).ToString();
                    labelErrorMessage.Refresh();
                }
            }

            labelErrorMessage.Text = "Rendering...";
            labelErrorMessage.Refresh();

            BitmapData data = this.imageBuffer.LockBits(new Rectangle(0, 0, this.imageBuffer.Width, this.imageBuffer.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            for (int y = 1; y <= h; y++)
            {
                for (int x = 1; x <= w; x++)
                {
                    double x1 = points[y - 1, x - 1, 0];
                    double y1 = points[y - 1, x - 1, 1];
                    double x2 = points[y - 1, x - 1, 2];
                    double y2 = points[y - 1, x - 1, 3];

                    if (!double.IsInfinity(x1) && !double.IsNaN(x1))
                    {
                        Fill(data, x1, y1, cx, cy, w, h);
                    }
                    if (!double.IsInfinity(y2) && !double.IsNaN(y2))
                    {
                        Fill(data, x2, y2, cx, cy, w, h);
                    }
                }
            }
            this.imageBuffer.UnlockBits(data);

            labelErrorMessage.Text = "(Ready)";
            labelErrorMessage.Refresh();
        }
    }
}
