using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonGroup : RibbonItemContainer
    {
        public const int GroupPadding = 3;
        public const int GroupHeaderPadding = 3;
        public const int GroupBorder = 2;

        private RibbonCompactedGroup compactedGroup = null;

        public RibbonTab Tab { get; internal set; }
        public string Name { get; set; }
        public int WidthLevel { get; set; }
        public int HeaderHeight { get; set; }
        public int HeaderMinWidth { get; private set; }
        public int HeaderMinHeight { get; private set; }

        public RibbonGroup()
        {
            this.Name = "";
        }

        public abstract int WidthLevelCount { get; }
        public abstract int GetWidthInternal(int level);

        public virtual RibbonGroup CompactedGroup
        {
            get
            {
                if (this.compactedGroup == null)
                {
                    this.compactedGroup = new RibbonCompactedGroup(this.Tab.Container, this);
                }
                return this.compactedGroup;
            }
        }

        public int GetWidth(int level)
        {
            return Math.Max(this.HeaderMinWidth, GetWidthInternal(level));
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            Rectangle headerBounds = itemBounds;
            {
                headerBounds.Y += itemBounds.Height - HeaderHeight;
                headerBounds.Width -= 1;
                headerBounds.Height = this.HeaderHeight - 1;
            }
            g.FillRectangle(settings.Header.Brush, headerBounds);
            settings.DrawCarvedBorder(g, settings.LightBorder, settings.DarkBorder, itemBounds);
            settings.DrawCarvedText(g, settings.LightText, settings.DarkText, headerBounds, this.Name, settings.Font);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            SizeF size = g.MeasureString(this.Name, settings.Font);
            this.HeaderMinWidth = (int)size.Width + 2 * GroupHeaderPadding;
            this.HeaderMinHeight = (int)size.Height + 2 * GroupHeaderPadding;
            this.HeaderHeight = this.HeaderMinHeight;
        }
    }
}
