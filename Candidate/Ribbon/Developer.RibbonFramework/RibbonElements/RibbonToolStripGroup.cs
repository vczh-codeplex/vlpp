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

        public override int GetWidth(int level)
        {
            int totalLines = level == NormalWidthLevel ? 2 : 3;
            RibbonToolStrip[][] tools = Enumerable.Range(0, totalLines)
                .Select(l => this.ToolStrips.Where(t => (level == NormalWidthLevel ? t.NormalLine : t.CompactLine) == l).ToArray())
                .ToArray();
            return GroupBorder * 2 + tools
                .Select(ts => (ts.Length + 1) * GroupPadding + ts.Select(t => t.SuggestedWidth).Sum())
                .Max();
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return this.ToolStrips.SelectMany(t => t.ToolItems);
            }
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

        public override void Render(Graphics g, Rectangle groupBounds)
        {
            base.Render(g, groupBounds);
            foreach (var tool in this.ToolStrips)
            {
                tool.Render(g);
            }
        }

        public override void Update(System.Drawing.Graphics g)
        {
            base.Update(g);
            foreach (var tool in this.ToolStrips)
            {
                tool.Group = this;
                foreach (var item in tool.ToolItems)
                {
                    item.Group = this;
                    item.ToolStrip = tool;
                    item.ItemSize = RibbonItemSize.ToolStrip;
                }
                tool.Update(g);
            }
            int totalLines = this.WidthLevel == NormalWidthLevel ? 2 : 3;
            this.placedToolStrips = Enumerable.Range(0, totalLines)
                .Select(l => this.ToolStrips.Where(t => t.LineNumber == l).ToList())
                .ToArray();
        }

        public override void UpdateWithSizeDecided(Graphics g)
        {
            Update(g);
        }

        public Rectangle GetToolStripBounds(RibbonToolStrip tool)
        {
            Rectangle groupBounds = this.Tab.GetGroupBounds(this);
            int totalHeight = groupBounds.Height - HeaderHeight - 2 * GroupBorder;
            int totalLines = this.placedToolStrips.Length;
            int yPadding = (totalHeight - RibbonToolStrip.ToolStripHeight * totalLines) / (1 + totalLines);

            List<RibbonToolStrip> tools = this.placedToolStrips[tool.LineNumber];
            int index = tools.IndexOf(tool);
            int x = groupBounds.Left + GroupBorder + (index + 1) * GroupPadding + tools.Take(index).Select(t => t.SuggestedWidth).Sum();
            int y = groupBounds.Top + (tool.LineNumber + 1) * yPadding + tool.LineNumber * RibbonToolStrip.ToolStripHeight;
            int w = tool.SuggestedWidth;
            int h = RibbonToolStrip.ToolStripHeight;
            return new Rectangle(x, y, w, h);
        }
    }

    public class RibbonToolStrip
    {
        public const int ToolStripHeight = 22;
        public const int ToolStripBorder = 1;
        public const int ToolStripItemHeight = ToolStripHeight - 2 * ToolStripBorder;

        public RibbonToolStripGroup Group { get; internal set; }
        public IList<RibbonItem> ToolItems { get; private set; }
        public int NormalLine { get; set; }
        public int CompactLine { get; set; }
        public int SuggestedWidth { get; set; }

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

        public void Render(Graphics g)
        {
            foreach (var item in this.ToolItems)
            {
                item.Render(g, GetItemBounds(item));
            }
        }

        public void Update(Graphics g)
        {
            this.SuggestedWidth = this.ToolItems.Select(i => i.GetSuggestedWidth(g)).Sum() + 2 * ToolStripBorder;
            this.toolItemWidth = this.ToolItems.Select(i => i.GetSuggestedWidth(g)).ToArray();
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
