using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonMenu : RibbonItemContainer
    {
        public const int MenuBorder = 1;
        public const int MenuIconAreaSize = 24;

        public IList<RibbonItem> MenuItems { get; private set; }
        public Size TotalSize { get; protected set; }

        public RibbonMenu()
        {
            this.MenuItems = new List<RibbonItem>();
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return this.MenuItems;
            }
        }

        public override Rectangle GetItemBounds(RibbonItem targetItem)
        {
            Rectangle itemBounds = this.Services.GetBounds(this);
            int x = itemBounds.Left + MenuBorder;
            int y = itemBounds.Top + MenuBorder;
            int w = itemBounds.Width - 2 * MenuBorder;
            foreach (var item in this.MenuItems)
            {
                int h = item.UpdatedSize.Height;
                Rectangle r = new Rectangle(x, y, w, h);
                if (item == targetItem)
                {
                    return r;
                }
                y += h;
            }
            return Rectangle.Empty;
        }

        public override RibbonItem GetItemFromPoint(Point location)
        {
            foreach (var item in this.Items)
            {
                if (GetItemBounds(item).Contains(location))
                {
                    return item;
                }
            }
            return null;
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            {
                Rectangle r = itemBounds;
                r.Width -= 1;
                r.Height -= 1;
                g.DrawRectangle(settings.MenuSeparator.Pen, r);
            }
            int x = itemBounds.Left + MenuBorder;
            int y = itemBounds.Top + MenuBorder;
            int w = itemBounds.Width - 2 * MenuBorder;
            foreach (var item in this.MenuItems)
            {
                int h = item.UpdatedSize.Height;
                Rectangle r = new Rectangle(x, y, w, h);
                item.Render(g, settings, r);
                y += h;
            }
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            foreach (var item in this.MenuItems)
            {
                item.ItemContainer = this;
                item.ItemSize = RibbonItemSize.MenuItem;
                item.Update(g, settings);
            }
            int w = MenuBorder * 2 + (this.MenuItems.Count == 0 ? 0 : this.MenuItems.Select(i => i.GetWidth(g, settings, i.ItemSize)).Max());
            int h = MenuBorder * 2 + this.MenuItems.Select(i => i.GetHeight(g, settings, i.ItemSize)).Sum();
            this.TotalSize = new Size(w, h);
        }

        public override void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.UpdateWithSizeDecided(g, settings);
            foreach (var item in this.MenuItems)
            {
                item.UpdateWithSizeDecided(g, settings);
            }
        }
    }
}
