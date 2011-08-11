using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FvCalculation;
using System.Reflection;
using System.Globalization;

namespace FvGUI
{
    public partial class MainForm : Form
    {
        private const int SuggestedUnitPixel = 100;

        private Expression function = null;
        private int unitPixels = 0;
        private double originX = 0;
        private double originY = 0;
        private Bitmap imageBuffer = null;

        private void TextBoxChanged()
        {
            Expression tempFunction = null;
            int tempUnitPixels = 0;
            double tempOriginX = 0;
            double tempOriginY = 0;

            try
            {
                tempFunction = Expression.Parse(textBoxFunction.Text);
            }
            catch (Exception e)
            {
                buttonRender.Enabled = false;
                labelErrorMessage.Text = "[Function]" + e.Message;
                return;
            }

            try
            {
                tempUnitPixels = int.Parse(textBoxUnitPixels.Text, NumberStyles.None);
            }
            catch (Exception e)
            {
                buttonRender.Enabled = false;
                labelErrorMessage.Text = "[UnitPixels]" + e.Message;
                return;
            }

            try
            {
                tempOriginX = double.Parse(textBoxOriginX.Text, NumberStyles.AllowDecimalPoint);
            }
            catch (Exception e)
            {
                buttonRender.Enabled = false;
                labelErrorMessage.Text = "[OriginX]" + e.Message;
                return;
            }

            try
            {
                tempOriginY = double.Parse(textBoxOriginY.Text, NumberStyles.AllowDecimalPoint);
            }
            catch (Exception e)
            {
                buttonRender.Enabled = false;
                labelErrorMessage.Text = "[OriginY]" + e.Message;
                return;
            }

            this.function = tempFunction;
            this.unitPixels = tempUnitPixels;
            this.originX = tempOriginX;
            this.originY = tempOriginY;
            buttonRender.Enabled = true;
            labelErrorMessage.Text = "(Ready)";
        }

        private void RebuildBuffer()
        {
            if (this.imageBuffer != null)
            {
                this.imageBuffer.Dispose();
            }
            Size size = panelImage.Size;
            this.imageBuffer = new Bitmap(size.Width, size.Height);
        }

        private void RenderAxis()
        {
            int w = this.imageBuffer.Width;
            int h = this.imageBuffer.Height;
            using (Graphics g = Graphics.FromImage(this.imageBuffer))
            {
                g.FillRectangle(Brushes.White, 0, 0, w, h);

                int cx = (int)Math.Round(w / 2 - this.originX * this.unitPixels);
                int cy = (int)Math.Round(h / 2 - this.originY * this.unitPixels);
                int up = this.unitPixels;
                if (this.unitPixels < SuggestedUnitPixel)
                {
                    up = this.unitPixels * (SuggestedUnitPixel / this.unitPixels);
                    if (up < SuggestedUnitPixel)
                    {
                        up += this.unitPixels;
                    }
                }
                else if (this.unitPixels > SuggestedUnitPixel)
                {
                    up = this.unitPixels / (this.unitPixels / SuggestedUnitPixel);
                }
                double u = (double)up / this.unitPixels;

                int sx = cx - (cx / up * up);
                int sy = cy - (cy / up * up);
                for (int x = sx; x < w; x += up)
                {
                    g.DrawLine((x == cx ? Pens.Black : Pens.LightGray), x, 0, x, h);
                    g.DrawString(((cx - x) / up * u).ToString(), panelImage.Font, Brushes.Black, x, cy);
                }
                for (int y = sy; y < h; y += up)
                {
                    g.DrawLine((y == cy ? Pens.Black : Pens.LightGray), 0, y, w, y);
                    if (y != cy)
                    {
                        g.DrawString(((y - cy) / up * u).ToString(), panelImage.Font, Brushes.Black, cx, y);
                    }
                }

                g.DrawRectangle(Pens.Black, 0, 0, w - 1, h - 1);
            }
        }

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

            labelErrorMessage.Text = "(Ready)";
            labelErrorMessage.Refresh();
        }

        public MainForm()
        {
            InitializeComponent();
            panelImage.GetType()
                .GetProperty("DoubleBuffered", BindingFlags.NonPublic | BindingFlags.Instance)
                .SetValue(panelImage, true, new object[] { });
            TextBoxChanged();
            RebuildBuffer();
            RenderAxis();
        }

        private void buttonRender_Click(object sender, EventArgs e)
        {
            RenderAxis();
            RenderFunction();
            panelImage.Refresh();
        }

        private void textBoxFunction_TextChanged(object sender, EventArgs e)
        {
            TextBoxChanged();
        }

        private void textBoxUnitPixels_TextChanged(object sender, EventArgs e)
        {
            TextBoxChanged();
        }

        private void textBoxOriginX_TextChanged(object sender, EventArgs e)
        {
            TextBoxChanged();
        }

        private void textBoxOriginY_TextChanged(object sender, EventArgs e)
        {
            TextBoxChanged();
        }

        private void panelImage_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.imageBuffer, 0, 0);
        }

        private void panelImage_SizeChanged(object sender, EventArgs e)
        {
            RebuildBuffer();
            RenderAxis();
            panelImage.Refresh();
        }
    }
}
