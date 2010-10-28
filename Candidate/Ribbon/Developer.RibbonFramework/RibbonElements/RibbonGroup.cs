using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonGroup
    {
        public const int GroupPadding = 3;
        public const int GroupHeaderPadding = 3;
        public const int GroupBorder = 2;

        public RibbonTab Tab { get; internal set; }
        public string Name { get; set; }
        public int WidthLevel { get; set; }
        public int HeaderHeight { get; set; }
        public int HeaderMinWidth { get; private set; }
        public int HeaderMinHeight { get; private set; }

        public RibbonGroup()
        {
            this.Name = "RibbonGroup";
        }

        public virtual int WidthLevelCount
        {
            get
            {
                return this.HeaderMinWidth < 100 ? 2 : 1;
            }
        }

        public virtual int GetWidth(int level)
        {
            if (this.HeaderMinWidth >= 100 || level == 0)
            {
                return this.HeaderMinWidth;
            }
            else
            {
                return 100;
            }
        }

        public virtual IEnumerable<RibbonItem> Items
        {
            get
            {
                return new RibbonItem[] { };
            }
        }

        public virtual void Render(Graphics g, Rectangle groupBounds)
        {
            var settings = this.Tab.Container.Settings;
            Rectangle headerBounds = groupBounds;
            {
                headerBounds.Y += groupBounds.Height - HeaderHeight;
                headerBounds.Width -= 1;
                headerBounds.Height = this.HeaderHeight - 1;
            }
            g.FillRectangle(settings.Header.Brush, headerBounds);
            settings.DrawCarvedBorder(g, settings.LightBorder, settings.DarkBorder, groupBounds);
            settings.DrawCarvedText(g, settings.LightText, settings.DarkText, headerBounds, this.Name, this.Tab.Container.Font);
        }

        public virtual void Update(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Tab.Container.Font);
            this.HeaderMinWidth = (int)size.Width + 2 * GroupHeaderPadding;
            this.HeaderMinHeight = (int)size.Height + 2 * GroupHeaderPadding;
        }

        public virtual void UpdateWithSizeDecided(Graphics g)
        {
        }

        public virtual RibbonItem GetItemFromPoint(Point location)
        {
            return null;
        }

        public virtual bool OnMouseDown(MouseEventArgs e)
        {
            RibbonItem item = GetItemFromPoint(e.Location);
            if (item != null)
            {
                return item.OnMouseDown(e);
            }
            return false;
        }

        public virtual bool OnMouseMove(MouseEventArgs e)
        {
            RibbonItem item = GetItemFromPoint(e.Location);
            bool result = false;
            foreach (var ribbonItem in this.Items)
            {
                if (ribbonItem != item)
                {
                    result = ribbonItem.OnMouseLeave(new EventArgs()) || result;
                }
            }
            if (item != null)
            {
                result = item.OnMouseMove(e) || result;
            }
            return result;
        }

        public virtual bool OnMouseUp(MouseEventArgs e)
        {
            RibbonItem item = GetItemFromPoint(e.Location);
            if (item != null)
            {
                return item.OnMouseUp(e);
            }
            return false;
        }

        public virtual bool OnMouseLeave(EventArgs e)
        {
            return this.Items.Select(item => item.OnMouseLeave(e)).ToArray().Any();
        }
    }
}
