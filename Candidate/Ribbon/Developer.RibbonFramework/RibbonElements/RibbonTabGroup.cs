using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonTabGroup
    {
        public RibbonContainer Container { get; internal set; }
        public string Name { get; set; }
        public int FirstIndex { get; set; }
        public int LastIndex { get; set; }

        public RibbonTabGroup()
        {
            this.Name = "";
            this.FirstIndex = -1;
            this.LastIndex = -1;
        }

        public void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle groupBounds, bool drawOnAeroFrame)
        {
            Font font = SystemFonts.CaptionFont;
            SizeF size = g.MeasureString(this.Name, font);
            int x = groupBounds.Left + (int)(groupBounds.Width - size.Width) / 2;
            int y = groupBounds.Top + (int)(groupBounds.Height - size.Height) / 2;
            if (drawOnAeroFrame)
            {
                Rectangle textBounds = new Rectangle(x, y, (int)size.Width, (int)size.Height);
                Action<Graphics> backgroundPainter = memoryGraphics =>
                {
                    Rectangle gradientBounds = new Rectangle(0, 0, groupBounds.Width - 1, groupBounds.Height - 1);
                    int upHeight = gradientBounds.Height / 4 * 3;
                    Rectangle upBounds = gradientBounds;
                    Rectangle downBounds = gradientBounds;
                    upBounds.Height = upHeight;
                    downBounds.Y += upHeight;
                    downBounds.Height -= upHeight;

                    Color c1 = Color.FromArgb(0, 0, 0, 0);
                    Color c3 = settings.TabGroupHighlight.Color;
                    Color c2 = Color.FromArgb(c3.A / 3, c3.R / 3, c3.G / 3, c3.B / 3);
                    using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(upBounds, 1, 1), c1, c2, 90.0f))
                    {
                        memoryGraphics.FillRectangle(brush, upBounds);
                    }
                    using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(downBounds, 1, 1), c2, c3, 90.0f))
                    {
                        memoryGraphics.FillRectangle(brush, downBounds);
                    }
                };
                WindowsAPI.DrawAeroText(g, this.Name, font, groupBounds, textBounds, settings.TabGroupText.Color, TextFormatFlags.Default, WindowsAPI.TextStyle.Glowing, backgroundPainter);
            }
            else
            {
                g.FillRectangle(settings.TabGroupHighlight.Brush, groupBounds);
                g.DrawString(this.Name, settings.Font, settings.TabGroupText.Brush, x, y);
            }
        }

        public void Update(Graphics g)
        {
        }
    }
}
