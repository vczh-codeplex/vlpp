using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonColorItem : IDisposable
    {
        public Color Color { get; set; }
        public Brush Brush { get; set; }
        public Pen Pen { get; set; }

        public RibbonColorItem(Color color)
        {
            this.Color = color;
            this.Brush = new SolidBrush(color);
            this.Pen = new Pen(color);
        }

        public void Dispose()
        {
            this.Brush.Dispose();
            this.Pen.Dispose();
        }
    }

    public class RibbonThemaSettingsBase : IDisposable
    {
        public RibbonColorItem Border { get; protected set; }
        public RibbonColorItem DarkBorder { get; protected set; }
        public RibbonColorItem LightBorder { get; protected set; }
        public RibbonColorItem TabText { get; protected set; }
        public RibbonColorItem TabGroupText { get; protected set; }
        public RibbonColorItem DarkText { get; protected set; }
        public RibbonColorItem LightText { get; protected set; }
        public RibbonColorItem Panel { get; protected set; }
        public RibbonColorItem Panel1 { get; protected set; }
        public RibbonColorItem Panel2 { get; protected set; }
        public RibbonColorItem Panel3 { get; protected set; }
        public RibbonColorItem Panel4 { get; protected set; }
        public RibbonColorItem Header { get; protected set; }
        public RibbonColorItem TabGroupHighlight { get; protected set; }

        public RibbonColorItem ButtonReleasedBackground1 { get; protected set; }
        public RibbonColorItem ButtonReleasedBackground2 { get; protected set; }
        public RibbonColorItem ButtonReleasedBackground3 { get; protected set; }
        public RibbonColorItem ButtonReleasedBackground4 { get; protected set; }
        public RibbonColorItem ButtonReleasedOuterBorderTop { get; protected set; }
        public RibbonColorItem ButtonReleasedOuterBorderBottom { get; protected set; }
        public RibbonColorItem ButtonReleasedInnerBorderTop { get; protected set; }
        public RibbonColorItem ButtonReleasedInnerBorderBottom { get; protected set; }
        public RibbonColorItem ButtonPressedBackground1 { get; protected set; }
        public RibbonColorItem ButtonPressedBackground2 { get; protected set; }
        public RibbonColorItem ButtonPressedBackground3 { get; protected set; }
        public RibbonColorItem ButtonPressedBackground4 { get; protected set; }
        public RibbonColorItem ButtonPressedOuterBorderTop { get; protected set; }
        public RibbonColorItem ButtonPressedOuterBorderBottom { get; protected set; }
        public RibbonColorItem ButtonPressedInnerBorderTop { get; protected set; }
        public RibbonColorItem ButtonPressedInnerBorderBottom { get; protected set; }
        public RibbonColorItem ButtonDropDownDark { get; protected set; }
        public RibbonColorItem ButtonDropDownLight { get; protected set; }

        public RibbonColorItem Menu { get; protected set; }
        public RibbonColorItem MenuSeparator { get; protected set; }
        public RibbonColorItem MenuIconBackground { get; protected set; }

        public Font Font { get; set; }

        public void Dispose()
        {
            foreach (var property in this.GetType().GetProperties().Where(p => p.PropertyType == typeof(RibbonColorItem)))
            {
                RibbonColorItem item = (RibbonColorItem)property.GetValue(this, new object[] { });
                item.Dispose();
            }
        }

        #region Helper Functions

        public virtual void DrawDoubleGradientPanel(Graphics g, RibbonColorItem i1, RibbonColorItem i2, RibbonColorItem i3, RibbonColorItem i4, Rectangle bounds, double upRatio)
        {
            Rectangle upBounds = bounds;
            Rectangle downBounds = bounds;
            int upHeight = (int)(bounds.Height * upRatio);
            {
                upBounds.Width -= 1;
                upBounds.Height = upHeight;
                downBounds.Y += upHeight;
                downBounds.Width -= 1;
                downBounds.Height -= upHeight;
            }
            using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(upBounds, 1, 1), i1.Color, i2.Color, 90.0f))
            {
                g.FillRectangle(brush, upBounds);
            }
            using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(downBounds, 1, 1), i3.Color, i4.Color, 90.0f))
            {
                g.FillRectangle(brush, downBounds);
            }
        }

        public virtual void DrawDoubleGradientBorder(Graphics g, RibbonColorItem outTop, RibbonColorItem outBottom, RibbonColorItem inTop, RibbonColorItem inBottom, Rectangle bounds)
        {
            int ox1 = bounds.Left;
            int oy1 = bounds.Top;
            int ox2 = bounds.Right - 1;
            int oy2 = bounds.Bottom - 1;
            int ix1 = ox1 + 1;
            int iy1 = oy1 + 1;
            int ix2 = ox2 - 1;
            int iy2 = oy2 - 1;
            using (GraphicsPath path = new GraphicsPath())
            {
                path.AddLine(ix1, oy1, ix2, oy1);
                path.AddLine(ox2, iy1, ox2, iy2);
                path.AddLine(ix2, oy2, ix1, oy2);
                path.AddLine(ox1, iy2, ox1, iy1);
                using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(bounds, 1, 1), outTop.Color, outBottom.Color, 90.0f))
                using (Pen pen = new Pen(brush))
                {
                    path.Widen(pen);
                    g.FillPath(brush, path);
                }
            }
            using (GraphicsPath path = new GraphicsPath())
            {
                path.AddLine(ix1, iy1, ix2, iy1);
                path.AddLine(ix2, iy1, ix2, iy2);
                path.AddLine(ix2, iy2, ix1, iy2);
                path.AddLine(ix1, iy2, ix1, iy1);
                using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(bounds, 1, 1), inTop.Color, inBottom.Color, 90.0f))
                using (Pen pen = new Pen(brush))
                {
                    path.Widen(pen);
                    g.FillPath(brush, path);
                }
            }
        }

        public virtual void DrawCarvedBorder(Graphics g, RibbonColorItem light, RibbonColorItem dark, Rectangle bounds)
        {
            Rectangle darkBounds = bounds;
            Rectangle lightBounds = bounds;
            {
                darkBounds.Width -= 2;
                darkBounds.Height -= 2;
                lightBounds.X += 1;
                lightBounds.Y += 1;
                lightBounds.Width -= 2;
                lightBounds.Height -= 2;
            }
            g.DrawRectangle(light.Pen, lightBounds);
            g.DrawRectangle(dark.Pen, darkBounds);
        }

        public virtual void DrawCarvedText(Graphics g, RibbonColorItem light, RibbonColorItem dark, Rectangle bounds, string text, Font font)
        {
            SizeF size = g.MeasureString(text, font);
            int x = bounds.Left + (int)(bounds.Width - size.Width) / 2;
            int y = bounds.Top + (int)(bounds.Height - size.Height) / 2;
            g.DrawString(text, font, light.Brush, x + 1, y + 1);
            g.DrawString(text, font, dark.Brush, x, y);
        }

        public virtual void DrawDropDown(Graphics g, RibbonColorItem light, RibbonColorItem dark, Rectangle bounds)
        {
            int x1 = bounds.Left;
            int x2 = bounds.Left + bounds.Width / 2;
            int x3 = bounds.Right;
            int y1 = bounds.Top;
            int y2 = bounds.Bottom;

            Point[] points = new Point[3];
            points[0].X = x1;
            points[0].Y = y1;
            points[1].X = x2;
            points[1].Y = y2;
            points[2].X = x3;
            points[2].Y = y1;

            using (LinearGradientBrush brush = new LinearGradientBrush(Rectangle.Inflate(bounds, 1, 1), light.Color, dark.Color, 180.0f))
            {
                g.FillPolygon(brush, points);
            }
        }

        #endregion
    }

    public class RibbonThemaSettings : RibbonThemaSettingsBase
    {
        public RibbonThemaSettings()
        {
            this.Border
                = new RibbonColorItem(Color.FromArgb(141, 178, 227));
            this.DarkBorder
                = new RibbonColorItem(Color.FromArgb(197, 210, 223));
            this.LightBorder
                = new RibbonColorItem(Color.FromArgb(235, 241, 247));
            this.TabText
                = new RibbonColorItem(Color.FromArgb(53, 58, 127));
            this.TabGroupText
                = new RibbonColorItem(SystemColors.ActiveCaptionText);
            this.DarkText
                = new RibbonColorItem(Color.FromArgb(53, 58, 127));
            this.LightText
                = new RibbonColorItem(Color.FromArgb(227, 244, 255));
            this.Panel
                = new RibbonColorItem(Color.FromArgb(191, 219, 255));
            this.Panel1
                = new RibbonColorItem(Color.FromArgb(223, 236, 247));
            this.Panel2
                = new RibbonColorItem(Color.FromArgb(209, 223, 240));
            this.Panel3
                = new RibbonColorItem(Color.FromArgb(199, 216, 237));
            this.Panel4
                = new RibbonColorItem(Color.FromArgb(227, 244, 255));
            this.Header
                = new RibbonColorItem(Color.FromArgb(193, 217, 240));
            this.TabGroupHighlight
                = new RibbonColorItem(Color.FromArgb(224, 255, 72));

            this.ButtonReleasedBackground1
                = new RibbonColorItem(Color.FromArgb(255, 252, 217));
            this.ButtonReleasedBackground2
                = new RibbonColorItem(Color.FromArgb(255, 231, 143));
            this.ButtonReleasedBackground3
                = new RibbonColorItem(Color.FromArgb(255, 215, 74));
            this.ButtonReleasedBackground4
                = new RibbonColorItem(Color.FromArgb(255, 231, 152));
            this.ButtonReleasedOuterBorderTop
                = new RibbonColorItem(Color.FromArgb(221, 207, 155));
            this.ButtonReleasedOuterBorderBottom
                = new RibbonColorItem(Color.FromArgb(211, 207, 188));
            this.ButtonReleasedInnerBorderTop
                = new RibbonColorItem(Color.FromArgb(255, 255, 247));
            this.ButtonReleasedInnerBorderBottom
                = new RibbonColorItem(Color.FromArgb(255, 243, 93));
            this.ButtonPressedBackground1
                = new RibbonColorItem(Color.FromArgb(198, 150, 97));
            this.ButtonPressedBackground2
                = new RibbonColorItem(Color.FromArgb(252, 161, 96));
            this.ButtonPressedBackground3
                = new RibbonColorItem(Color.FromArgb(245, 133, 58));
            this.ButtonPressedBackground4
                = new RibbonColorItem(Color.FromArgb(254, 188, 96));
            this.ButtonPressedOuterBorderTop
                = new RibbonColorItem(Color.FromArgb(139, 118, 84));
            this.ButtonPressedOuterBorderBottom
                = new RibbonColorItem(Color.FromArgb(243, 202, 122));
            this.ButtonPressedInnerBorderTop
                = new RibbonColorItem(Color.FromArgb(172, 150, 116));
            this.ButtonPressedInnerBorderBottom
                = new RibbonColorItem(Color.FromArgb(253, 173, 17));
            this.ButtonDropDownDark
                = new RibbonColorItem(Color.FromArgb(138, 141, 143));
            this.ButtonDropDownLight
                = new RibbonColorItem(Color.FromArgb(92, 95, 96));

            this.Menu
                = new RibbonColorItem(Color.FromArgb(250, 250, 250));
            this.MenuSeparator
                = new RibbonColorItem(Color.FromArgb(197, 197, 197));
            this.MenuIconBackground
                = new RibbonColorItem(Color.FromArgb(233, 238, 238));
        }
    }
}
