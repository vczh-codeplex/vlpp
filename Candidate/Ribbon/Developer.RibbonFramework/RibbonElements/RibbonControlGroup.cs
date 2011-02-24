using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonControlGroup : RibbonGroup
    {
        public IList<RibbonControlGroupPanel> Panels { get; private set; }

        public RibbonControlGroup()
        {
            this.Panels = new List<RibbonControlGroupPanel>();
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return this.Panels.SelectMany(p => p.ControlItems);
            }
        }

        public override Rectangle GetItemBounds(RibbonItem item)
        {
            return item.ControlGroupPanel.GetItemBounds(item);
        }

        public override RibbonItem GetItemFromPoint(Point location)
        {
            RibbonControlGroupPanel targetPanel = null;
            foreach (var panel in this.Panels)
            {
                if (GetToolStripBounds(panel).Contains(location))
                {
                    targetPanel = panel;
                    break;
                }
            }
            if (targetPanel == null) return null;

            foreach (var item in targetPanel.ControlItems)
            {
                if (targetPanel.GetItemBounds(item).Contains(location))
                {
                    return item;
                }
            }
            return null;
        }

        public override int WidthLevelCount
        {
            get
            {
                return 1 + this.Panels.Select(p => p.EnabledCompactCount).Sum() + 1;
            }
        }

        public override int GetWidthInternal(int level)
        {
            RibbonItemSize[] sizes = GetPanelSizesFromLevel(level);
            return this.Panels.Select((p, i) => p.GetUpdatedWidth(sizes[i])).Sum() + (this.Panels.Count + 1) * GroupPadding + 2 * GroupBorder;
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            foreach (var panel in this.Panels)
            {
                panel.Render(g, settings);
            }
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            foreach (var panel in this.Panels)
            {
                panel.Group = this;
                panel.Update(g, settings);
            }
        }

        public override void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.UpdateWithSizeDecided(g, settings);
            RibbonItemSize[] sizes = GetPanelSizesFromLevel(this.WidthLevel);
            for (int i = 0; i < this.Panels.Count; i++)
            {
                RibbonControlGroupPanel panel = this.Panels[i];
                panel.ItemSize = sizes[i];
                panel.UpdateWithSizeDecided(g, settings);
            }
        }

        public Rectangle GetToolStripBounds(RibbonControlGroupPanel panel)
        {
            int index = this.Panels.IndexOf(panel);
            Rectangle groupBounds = this.Services.GetBounds(this);
            int x = groupBounds.Left + GroupBorder + (index + 1) * GroupPadding + this.Panels.Take(index).Select(p => p.UpdatedWidth).Sum();
            int y = groupBounds.Top + GroupBorder + GroupPadding;
            int w = panel.UpdatedWidth;
            int h = groupBounds.Height - HeaderHeight - GroupBorder - 2 * GroupPadding;
            return new Rectangle(x, y, w, h);
        }

        protected RibbonItemSize[] GetPanelSizesFromLevel(int level)
        {
            RibbonItemSize[] sizes = this.Panels.Select(p =>
                p.IsSizeEnabled(RibbonItemSize.Big) ? RibbonItemSize.Big :
                p.IsSizeEnabled(RibbonItemSize.Small) ? RibbonItemSize.Small :
                RibbonItemSize.Compact
                )
                .ToArray();
            int currentLevel = this.WidthLevelCount - 1;
            while (currentLevel > level)
            {
                for (int i = this.Panels.Count - 1; i >= 0; i--)
                {
                    if (this.Panels[i].CanCompactTo(sizes[i], RibbonItemSize.Small))
                    {
                        sizes[i] = RibbonItemSize.Small;
                        goto FINISH_COMPACTING;
                    }
                }
                for (int i = this.Panels.Count - 1; i >= 0; i--)
                {
                    if (this.Panels[i].CanCompactTo(sizes[i], RibbonItemSize.Compact))
                    {
                        sizes[i] = RibbonItemSize.Compact;
                        goto FINISH_COMPACTING;
                    }
                }
            FINISH_COMPACTING:
                currentLevel--;
            }
            return sizes;
        }
    }

    public class RibbonControlGroupPanel
    {
        public RibbonControlGroup Group { get; internal set; }
        public IList<RibbonItem> ControlItems { get; private set; }
        public RibbonControlSizingPolicy Policy { get; set; }

        public int UpdatedBigSize { get; protected set; }
        public int UpdatedSmallSize { get; protected set; }
        public int UpdatedCompactSize { get; protected set; }
        public int UpdatedWidth { get; protected set; }
        public RibbonItemSize ItemSize { get; internal set; }

        public RibbonControlGroupPanel()
        {
            this.ControlItems = new List<RibbonItem>();
        }

        public int EnabledCompactCount
        {
            get
            {
                switch (this.Policy)
                {
                    case RibbonControlSizingPolicy.Big: return 0;
                    case RibbonControlSizingPolicy.BigSmall: return 1;
                    case RibbonControlSizingPolicy.BigCompact: return 2;
                    case RibbonControlSizingPolicy.Small: return 0;
                    case RibbonControlSizingPolicy.SmallCompact: return 1;
                    case RibbonControlSizingPolicy.Compact: return 0;
                    default: return 0;
                }
            }
        }

        public virtual bool IsSizeEnabled(RibbonItemSize size)
        {
            switch (size)
            {
                case RibbonItemSize.Big:
                    switch (this.Policy)
                    {
                        case RibbonControlSizingPolicy.Big: return true;
                        case RibbonControlSizingPolicy.BigSmall: return true;
                        case RibbonControlSizingPolicy.BigCompact: return true;
                        default: return false;
                    }
                case RibbonItemSize.Small:
                    switch (this.Policy)
                    {
                        case RibbonControlSizingPolicy.BigSmall: return true;
                        case RibbonControlSizingPolicy.BigCompact: return true;
                        case RibbonControlSizingPolicy.Small: return true;
                        case RibbonControlSizingPolicy.SmallCompact: return true;
                        default: return false;
                    }
                case RibbonItemSize.Compact:
                    switch (this.Policy)
                    {
                        case RibbonControlSizingPolicy.BigCompact: return true;
                        case RibbonControlSizingPolicy.SmallCompact: return true;
                        case RibbonControlSizingPolicy.Compact: return true;
                        default: return false;
                    }
                default: return false;
            }
        }

        public bool CanCompactTo(RibbonItemSize oldSize, RibbonItemSize newSize)
        {
            if (IsSizeEnabled(oldSize) && IsSizeEnabled(newSize))
            {
                switch (oldSize)
                {
                    case RibbonItemSize.Big: return newSize != RibbonItemSize.Big;
                    case RibbonItemSize.Small: return newSize == RibbonItemSize.Compact;
                }
            }
            return false;
        }

        public virtual int GetWidth(Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize size)
        {
            switch (size)
            {
                case RibbonItemSize.Big:
                    {
                        return this.ControlItems.Select(i => i.GetWidth(g, settings, size)).Sum() + Math.Max(0, this.ControlItems.Count - 1) * RibbonGroup.GroupPadding;
                    }
                case RibbonItemSize.Small:
                case RibbonItemSize.Compact:
                    {
                        int width = 0;
                        int totalGroups = this.ControlItems.Count / 3 + (this.ControlItems.Count % 3 == 0 ? 0 : 1);
                        for (int i = 0; i < totalGroups; i++)
                        {
                            int start = i * 3;
                            int end = Math.Min(i * 3 + 2, this.ControlItems.Count - 1);
                            width += Enumerable.Range(start, end - start + 1).Select(j => this.ControlItems[j].GetWidth(g, settings, size)).Max();
                        }
                        return width + Math.Max(0, totalGroups - 1) * RibbonGroup.GroupPadding;
                    }
                default:
                    throw new NotSupportedException();
            }
        }

        public int GetUpdatedWidth(RibbonItemSize size)
        {
            switch (size)
            {
                case RibbonItemSize.Big: return this.UpdatedBigSize;
                case RibbonItemSize.Small: return this.UpdatedSmallSize;
                case RibbonItemSize.Compact: return this.UpdatedCompactSize;
                default:
                    throw new NotSupportedException();
            }
        }

        public virtual void Render(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ControlItems)
            {
                item.Render(g, settings, GetItemBounds(item));
            }
        }

        public virtual void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ControlItems)
            {
                item.ItemContainer = this.Group;
                item.ControlGroupPanel = this;
                item.Update(g, settings);
            }
            this.UpdatedBigSize = GetWidth(g, settings, RibbonItemSize.Big);
            this.UpdatedSmallSize = GetWidth(g, settings, RibbonItemSize.Small);
            this.UpdatedCompactSize = GetWidth(g, settings, RibbonItemSize.Compact);
        }

        public virtual void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            foreach (var item in this.ControlItems)
            {
                item.ItemSize = this.ItemSize;
                item.UpdateWithSizeDecided(g, settings);
            }
            this.UpdatedWidth = GetWidth(g, settings, this.ItemSize);
        }

        public Rectangle GetItemBounds(RibbonItem item)
        {
            Rectangle bounds = this.Group.GetToolStripBounds(this);
            switch (this.ItemSize)
            {
                case RibbonItemSize.Big:
                    {
                        int index = this.ControlItems.IndexOf(item);
                        int y = bounds.Top;
                        int x = bounds.Left + this.ControlItems.Take(index).Select(i => i.UpdatedSize.Width).Sum() + Math.Max(0, index - 1) * RibbonGroup.GroupPadding;
                        int w = item.UpdatedSize.Width;
                        int h = bounds.Height;
                        return new Rectangle(x, y, w, h);
                    }
                case RibbonItemSize.Small:
                case RibbonItemSize.Compact:
                    {
                        int x = bounds.Left;
                        int y = bounds.Top;
                        int w = item.UpdatedSize.Width;
                        int h = (bounds.Height - 2 * RibbonGroup.GroupPadding) / 3;

                        int index = this.ControlItems.IndexOf(item);
                        int currentGroup = index / 3;
                        int currentPosition = index % 3;
                        for (int i = 0; i < currentGroup; i++)
                        {
                            int start = i * 3;
                            int end = Math.Min(i * 3 + 2, this.ControlItems.Count - 1);
                            x += Enumerable.Range(start, end - start + 1).Select(j => this.ControlItems[j].UpdatedSize.Width).Max() + RibbonGroup.GroupPadding;
                        }
                        y += currentPosition * (h + RibbonGroup.GroupPadding);
                        return new Rectangle(x, y, w, h);
                    }
                default:
                    return Rectangle.Empty;
            }
        }
    }

    public enum RibbonControlSizingPolicy
    {
        Big,
        BigSmall,
        BigCompact,
        Small,
        SmallCompact,
        Compact,
    }
}
