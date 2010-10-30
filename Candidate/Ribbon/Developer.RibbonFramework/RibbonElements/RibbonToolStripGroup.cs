using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonToolStripGroup : RibbonGroup
    {
        public const int NormalWidthLevel = 1;
        public const int CompactWidthLevel = 0;

        public IList<RibbonToolStrip> ToolStrips { get; private set; }

        private List<RibbonToolStrip>[] placedToolStrips;

        public RibbonToolStripGroup()
        {
            this.ToolStrips = new List<RibbonToolStrip>();
        }

        public override int WidthLevelCount
        {
            get
            {
                return 2;
            }
        }

        public override int GetWidthInternal(int level)
        {
            int totalLines = level == NormalWidthLevel ? 2 : 3;
            RibbonToolStrip[][] tools = Enumerable.Range(0, totalLines)
                .Select(l => this.ToolStrips.Where(t => (level == NormalWidthLevel ? t.NormalLine : t.CompactLine) == l).ToArray())
                .ToArray();
            return GroupBorder * 2 + tools
                .Select(ts => (ts.Length + 1) * GroupPadding + ts.Select(t => t.UpdatedWidth).Sum())
                .Max();
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return this.ToolStrips.SelectMany(t => t.ToolItems);
            }
        }

        public override Rectangle GetItemBounds(RibbonItem item)
        {
            return item.ToolStrip.GetItemBounds(item);
        }

        public override RibbonItem GetItemFromPoint(Point location)
        {
            RibbonToolStrip targetTool = null;
            foreach (var tool in this.ToolStrips)
            {
                if (GetToolStripBounds(tool).Contains(location))
                {
                    targetTool = tool;
                    break;
                }
            }
            if (targetTool == null) return null;

            foreach (var item in targetTool.ToolItems)
            {
                if (targetTool.GetItemBounds(item).Contains(location))
                {
                    return item;
                }
            }
            return null;
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle groupBounds)
        {
            base.Render(g, settings, groupBounds);
            foreach (var tool in this.ToolStrips)
            {
                tool.Render(g, settings);
            }
        }

        public override void Update(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            foreach (var tool in this.ToolStrips)
            {
                tool.Group = this;
                tool.Update(g, settings);
            }
        }

        public override void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.UpdateWithSizeDecided(g, settings);
            foreach (var tool in this.ToolStrips)
            {
                tool.UpdateWithSizeDecided(g, settings);
            }
            int totalLines = this.WidthLevel == NormalWidthLevel ? 2 : 3;
            this.placedToolStrips = Enumerable.Range(0, totalLines)
                .Select(l => this.ToolStrips.Where(t => t.LineNumber == l).ToList())
                .ToArray();
        }

        public Rectangle GetToolStripBounds(RibbonToolStrip tool)
        {
            Rectangle groupBounds = this.Services.GetBounds(this);
            int totalHeight = groupBounds.Height - HeaderHeight - 2 * GroupBorder;
            int totalLines = this.placedToolStrips.Length;
            int yPadding = Math.Max(1, (totalHeight - RibbonToolStrip.ToolStripHeight * totalLines) / (1 + totalLines));

            List<RibbonToolStrip> tools = this.placedToolStrips[tool.LineNumber];
            int index = tools.IndexOf(tool);
            int x = groupBounds.Left + GroupBorder + (index + 1) * GroupPadding + tools.Take(index).Select(t => t.UpdatedWidth).Sum();
            int y = groupBounds.Top + GroupBorder + (tool.LineNumber + 1) * yPadding + tool.LineNumber * RibbonToolStrip.ToolStripHeight;
            int w = tool.UpdatedWidth;
            int h = RibbonToolStrip.ToolStripHeight;
            return new Rectangle(x, y, w, h);
        }
    }

    public class RibbonToolStrip
    {
        public const int ToolStripHeight = 24;
        public const int ToolStripBorder = 2;
        public const int ToolStripItemHeight = ToolStripHeight - 2 * ToolStripBorder;

        public RibbonToolStripGroup Group { get; internal set; }
        public IList<RibbonItem> ToolItems { get; private set; }
        public int NormalLine { get; set; }
        public int CompactLine { get; set; }
        public int UpdatedWidth { get; protected set; }

        private int[] toolItemWidth = null;

        public RibbonToolStrip()
        {
            this.ToolItems = new List<RibbonItem>();
        }

        public int LineNumber
        {
            get
            {
                return this.Group.WidthLevel == RibbonToolStripGroup.NormalWidthLevel ? this.NormalLine : this.CompactLine;
            }
        }

        public void Render(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ToolItems)
            {
                item.Render(g, settings, GetItemBounds(item));
            }
            settings.DrawCarvedBorder(g, settings.LightBorder, settings.DarkBorder, this.Group.GetToolStripBounds(this));
        }

        public void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ToolItems)
            {
                item.ItemContainer = this.Group;
                item.ToolStrip = this;
                item.ItemSize = RibbonItemSize.ToolStrip;
                item.Update(g, settings);
            }
            this.UpdatedWidth = this.ToolItems.Select(i => i.GetWidth(g, settings, i.ItemSize)).Sum() + 2 * ToolStripBorder;
            this.toolItemWidth = this.ToolItems.Select(i => i.GetWidth(g, settings, i.ItemSize)).ToArray();
        }

        public void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ToolItems)
            {
                item.UpdateWithSizeDecided(g, settings);
            }
        }

        public Rectangle GetItemBounds(RibbonItem item)
        {
            Rectangle bounds = this.Group.GetToolStripBounds(this);
            int index = this.ToolItems.IndexOf(item);
            int x = bounds.Left + ToolStripBorder + this.toolItemWidth.Take(index).Sum();
            int y = bounds.Top + ToolStripBorder;
            int w = this.toolItemWidth[index];
            int h = ToolStripItemHeight;
            return new Rectangle(x, y, w, h);
        }
    }
}
