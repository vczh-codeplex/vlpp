using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonMenuItem : RibbonItem
    {
        public Size UpdatedMenuIconAreaSize { get; set; }

        public Rectangle GetIconBounds(Rectangle itemBounds)
        {
            Rectangle r = itemBounds;
            r.Width = this.UpdatedMenuIconAreaSize.Width;
            return r;
        }

        public Rectangle GetPanelBounds(Rectangle itemBounds)
        {
            Rectangle r = itemBounds;
            r.X += this.UpdatedMenuIconAreaSize.Width;
            r.Width -= this.UpdatedMenuIconAreaSize.Width;
            return r;
        }

        public override void Render(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            g.FillRectangle(settings.MenuIconBackground.Brush, GetIconBounds(itemBounds));
            g.FillRectangle(settings.Menu.Brush, GetPanelBounds(itemBounds));
            int x = itemBounds.Left + this.UpdatedMenuIconAreaSize.Width;
            int y1 = itemBounds.Top;
            int y2 = itemBounds.Bottom - 1;
            g.DrawLine(settings.MenuSeparator.Pen, x, y1, x, y2);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            this.UpdatedMenuIconAreaSize = new Size(RibbonMenu.MenuIconAreaSize, RibbonMenu.MenuIconAreaSize);
        }
    }
}
