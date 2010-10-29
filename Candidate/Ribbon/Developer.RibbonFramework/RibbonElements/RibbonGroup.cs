using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonGroup : RibbonItemContainer
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

        public virtual int GetWidthInternal(int level)
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

        public virtual int GetWidth(int level)
        {
            return Math.Max(this.HeaderMinWidth, GetWidthInternal(level));
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle groupBounds)
        {
            Rectangle headerBounds = groupBounds;
            {
                headerBounds.Y += groupBounds.Height - HeaderHeight;
                headerBounds.Width -= 1;
                headerBounds.Height = this.HeaderHeight - 1;
            }
            g.FillRectangle(settings.Header.Brush, headerBounds);
            settings.DrawCarvedBorder(g, settings.LightBorder, settings.DarkBorder, groupBounds);
            settings.DrawCarvedText(g, settings.LightText, settings.DarkText, headerBounds, this.Name, settings.Font);
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            this.HeaderMinWidth = (int)size.Width + 2 * GroupHeaderPadding;
            this.HeaderMinHeight = (int)size.Height + 2 * GroupHeaderPadding;
            this.HeaderHeight = this.HeaderMinHeight;
        }
    }
}
