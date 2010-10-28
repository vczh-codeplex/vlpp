using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonButtonGroup : RibbonGroup
    {
        private int[] bigWidth = null;
        private int[] smallWidth = null;
        private int[] smallCompactWidth = null;

        public IList<RibbonButtonGroupItem> BigItems { get; private set; }
        public IList<RibbonButtonGroupItem> SmallItems { get; private set; }

        public RibbonButtonGroup()
        {
            this.BigItems = new List<RibbonButtonGroupItem>();
            this.SmallItems = new List<RibbonButtonGroupItem>();
        }

        public override int WidthLevelCount
        {
            get
            {
                return this.smallWidth.Length + 1;
            }
        }

        public override int GetWidth(int level)
        {
            int totalBigWidth = this.bigWidth.Sum();
            int totalSmallWidth = this.smallWidth.Take(level).Sum();
            int totalSmallCompactWidth = this.smallCompactWidth.Skip(level).Sum();
            int totalPaddings = (this.BigItems.Count + 1 + this.smallWidth.Length) * GroupPadding + 2 * GroupBorder;
            return totalPaddings + totalBigWidth + totalSmallWidth + totalSmallCompactWidth;
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return this.BigItems.Concat(this.SmallItems);
            }
        }

        public override void Render(Graphics g, Rectangle groupBounds)
        {
            base.Render(g, groupBounds);
            foreach (var item in this.BigItems)
            {
                Rectangle bounds = this.GetBigItemBounds(item);
                item.Render(g, bounds);
            }
            foreach (var item in this.SmallItems)
            {
                Rectangle bounds = this.GetSmallItemBounds(item);
                item.Render(g, bounds);
            }
        }

        public override void Update(Graphics g)
        {
            base.Update(g);
            foreach (var item in this.BigItems)
            {
                item.Group = this;
                item.ItemSize = RibbonItemSize.Big;
                item.Update(g);
            }
            foreach (var item in this.SmallItems)
            {
                item.Group = this;
                item.ItemSize = RibbonItemSize.Small;
                item.Update(g);
            }

            this.bigWidth = this.BigItems.Select(item => item.GetBigWidth(g)).ToArray();
            int smallGroups = (this.SmallItems.Count + 2) / 3;
            this.smallWidth = new int[smallGroups];
            this.smallCompactWidth = new int[smallGroups];
            for (int i = 0; i < smallGroups; i++)
            {
                int start = i * 3;
                int end = Math.Min(i * 3 + 2, this.SmallItems.Count - 1);
                this.smallWidth[i] = this.SmallItems.Skip(start).Take(end - start + 1).Select(item => item.GetSmallWidth(g)).Max();
                this.smallCompactWidth[i] = this.SmallItems.Skip(start).Take(end - start + 1).Select(item => item.GetSmallCompactWidth(g)).Max();
            }
        }

        public override void UpdateWithSizeDecided(Graphics g)
        {
            base.UpdateWithSizeDecided(g);
            for (int i = 0; i < this.SmallItems.Count; i++)
            {
                RibbonItem item = this.SmallItems[i];
                int group = i / 3;
                item.ItemSize = group < this.WidthLevel ? RibbonItemSize.Small : RibbonItemSize.SmallCompact;
            }
        }

        public Rectangle GetBigItemBounds(RibbonButtonGroupItem targetItem)
        {
            int index = this.BigItems.IndexOf(targetItem);
            if (index == -1)
            {
                return Rectangle.Empty;
            }
            else
            {
                Rectangle groupBounds = this.Tab.GetGroupBounds(this);
                int x = groupBounds.Left + GroupBorder + (index + 1) * GroupPadding + this.bigWidth.Take(index).Sum();
                int y = groupBounds.Top + GroupBorder + GroupPadding;
                int w = this.bigWidth[index];
                int h = groupBounds.Height - HeaderHeight - GroupBorder - 2 * GroupPadding;
                return new Rectangle(x, y, w, h);
            }
        }

        public Rectangle GetSmallItemBounds(RibbonButtonGroupItem targetItem)
        {
            int index = this.SmallItems.IndexOf(targetItem);
            if (index == -1)
            {
                return Rectangle.Empty;
            }
            else
            {
                int group = index / 3;
                int bigOffsets = GroupBorder + this.BigItems.Count * GroupPadding + this.bigWidth.Sum();
                int smallOffsets = (group + 1) * GroupPadding;
                if (group < this.WidthLevel)
                {
                    smallOffsets += this.smallWidth.Take(group).Sum();
                }
                else
                {
                    int smalls = this.WidthLevel;
                    int compacts = group - this.WidthLevel;
                    smallOffsets += this.smallWidth.Take(smalls).Sum() + this.smallCompactWidth.Skip(smalls).Take(compacts).Sum();
                }

                Rectangle groupBounds = this.Tab.GetGroupBounds(this);
                int w = group < this.WidthLevel ? this.smallWidth[group] : this.smallCompactWidth[group];
                int h = (groupBounds.Height - HeaderHeight - GroupBorder - 4 * GroupPadding) / 3;
                int x = groupBounds.Left + bigOffsets + smallOffsets;
                int y = groupBounds.Top + GroupBorder + (index % 3) * h + (index % 3 + 1) * GroupPadding;
                return new Rectangle(x, y, w, h);
            }
        }

        public override RibbonItem GetItemFromPoint(Point location)
        {
            foreach (var item in this.BigItems)
            {
                if (GetBigItemBounds(item).Contains(location))
                {
                    return item;
                }
            }
            foreach (var item in this.SmallItems)
            {
                if (GetSmallItemBounds(item).Contains(location))
                {
                    return item;
                }
            }
            return null;
        }
    }
}
