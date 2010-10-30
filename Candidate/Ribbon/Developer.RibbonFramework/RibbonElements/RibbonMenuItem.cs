using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonMenuItem : RibbonItem
    {
        public const int MenuIconAreaWidth = RibbonMenu.MenuIconAreaSize + 1;
        public const int MenuIconAreaHeight = RibbonMenu.MenuIconAreaSize;

        public int UpdatedHeight { get; protected set; }

        public abstract int GetSuggestedHeight(System.Drawing.Graphics g, RibbonThemaSettingsBase settings);

        public Rectangle GetIconBounds(Rectangle itemBounds)
        {
            Rectangle r = itemBounds;
            r.Width = RibbonMenu.MenuIconAreaSize;
            return r;
        }

        public Rectangle GetPanelBounds(Rectangle itemBounds)
        {
            Rectangle r = itemBounds;
            r.X += MenuIconAreaWidth;
            r.Width -= MenuIconAreaWidth;
            return r;
        }

        public override void Render(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            g.FillRectangle(settings.MenuIconBackground.Brush, GetIconBounds(itemBounds));
            g.FillRectangle(settings.Menu.Brush, GetPanelBounds(itemBounds));
            int x = itemBounds.Left + RibbonMenu.MenuIconAreaSize;
            int y1 = itemBounds.Top;
            int y2 = itemBounds.Bottom - 1;
            g.DrawLine(settings.MenuSeparator.Pen, x, y1, x, y2);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
        }

        public override void UpdateWithSizeDecided(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            base.UpdateWithSizeDecided(g, settings);
            this.UpdatedHeight = GetSuggestedHeight(g, settings);
        }
    }
}
