using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonTab
    {
        public const int TabWidthOffset = 20;
        public const int TabHeightOffset = 10;
        public const int TabPadding = 3;
        public const int TabRoundRadius = 3;

        private List<RibbonGroup> realGroups = new List<RibbonGroup>();
        private RibbonButtonGroup mostCompactGroup = null;

        public RibbonContainer Container { get; internal set; }
        public string Name { get; set; }
        public RibbonElementState State { get; protected internal set; }
        public IList<RibbonGroup> Groups { get; private set; }

        public int TabWidth { get; protected set; }
        public int TabHeight { get; protected set; }

        public IEnumerable<RibbonGroup> RealGroups
        {
            get
            {
                return this.realGroups;
            }
        }

        public RibbonTab()
        {
            this.Name = "";
            this.Groups = new List<RibbonGroup>();
        }

        public virtual void RenderTab(Graphics g, RibbonThemaSettingsBase settings, Rectangle tabBounds)
        {
            switch (this.State)
            {
                case RibbonElementState.Normal:
                    {
                    }
                    break;
                case RibbonElementState.Hot:
                    {
                        using (GraphicsPath path = new GraphicsPath())
                        {
                            path.AddLine(tabBounds.Left, tabBounds.Bottom, tabBounds.Left, tabBounds.Top + TabRoundRadius);
                            path.AddArc(tabBounds.Left, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 180, 90);
                            path.AddLine(tabBounds.Left + TabRoundRadius, tabBounds.Top, tabBounds.Right - TabRoundRadius, tabBounds.Top);
                            path.AddArc(tabBounds.Right - 2 * TabRoundRadius, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 270, 90);
                            path.AddLine(tabBounds.Right, tabBounds.Top + TabRoundRadius, tabBounds.Right, tabBounds.Bottom);
                            g.DrawPath(settings.Border.Pen, path);
                        }
                    }
                    break;
                case RibbonElementState.Pressed:
                case RibbonElementState.Selected:
                    {
                        using (GraphicsPath path = new GraphicsPath())
                        {
                            path.AddArc(tabBounds.Left - 2 * TabRoundRadius, tabBounds.Bottom - 2 * TabRoundRadius, TabRoundRadius * 2, TabRoundRadius * 2, 90, -90);
                            path.AddLine(tabBounds.Left, tabBounds.Bottom - TabRoundRadius, tabBounds.Left, tabBounds.Top + TabRoundRadius);
                            path.AddArc(tabBounds.Left, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 180, 90);
                            path.AddLine(tabBounds.Left + TabRoundRadius, tabBounds.Top, tabBounds.Right - TabRoundRadius, tabBounds.Top);
                            path.AddArc(tabBounds.Right - 2 * TabRoundRadius, tabBounds.Top, TabRoundRadius * 2, TabRoundRadius * 2, 270, 90);
                            path.AddLine(tabBounds.Right, tabBounds.Top + TabRoundRadius, tabBounds.Right, tabBounds.Bottom - TabRoundRadius);
                            path.AddArc(tabBounds.Right, tabBounds.Bottom - 2 * TabRoundRadius, TabRoundRadius * 2, TabRoundRadius * 2, 180, -90);
                            g.DrawLine(settings.Panel1.Pen, tabBounds.Left - TabRoundRadius, tabBounds.Bottom, tabBounds.Right + TabRoundRadius, tabBounds.Bottom);
                            g.FillPath(settings.Panel1.Brush, path);
                            g.DrawPath(settings.Border.Pen, path);
                        }
                    }
                    break;
            }
            SizeF size = g.MeasureString(this.Name, settings.Font);
            int tx = tabBounds.Left + (tabBounds.Width - (int)size.Width) / 2;
            int ty = tabBounds.Bottom - TabHeightOffset / 2 - (int)size.Height;
            g.DrawString(this.Name, settings.Font, settings.TabText.Brush, tx, ty);
        }

        public virtual void RenderPanel(Graphics g, RibbonThemaSettingsBase settings, Rectangle panelBounds)
        {
            RibbonColorItem i1 = settings.Panel1;
            RibbonColorItem i2 = settings.Panel2;
            RibbonColorItem i3 = settings.Panel3;
            RibbonColorItem i4 = settings.Panel4;
            settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, panelBounds, (double)1 / 6);
            foreach (var group in this.realGroups)
            {
                group.Render(g, settings, this.GetGroupBounds(group));
            }
            panelBounds.Width -= 1;
            panelBounds.Height -= 1;
            g.DrawRectangle(settings.Border.Pen, panelBounds);
        }

        protected void UpdateGroup(RibbonGroup group, Graphics g, RibbonThemaSettingsBase settings)
        {
            group.Tab = this;
            group.Services = this.Container;
            group.Update(g, settings);
            group.WidthLevel = group.WidthLevelCount - 1;
        }

        public virtual void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            this.TabWidth = (int)size.Width + TabWidthOffset;
            this.TabHeight = (int)size.Height + TabHeightOffset;
            this.State = RibbonElementState.Normal;
            this.realGroups.Clear();
            this.realGroups.AddRange(this.Groups);

            if (this.realGroups.Count > 0)
            {
                foreach (var group in this.realGroups)
                {
                    UpdateGroup(group, g, settings);
                }
                int maxHeaderHeight = this.realGroups.Select(group => group.HeaderMinHeight).Max();
                foreach (var group in this.realGroups)
                {
                    group.HeaderHeight = maxHeaderHeight;
                }

                int maxWidth = this.Container.GetTabPanelBounds().Width;
                while (true)
                {
                    int currentWidth = (this.realGroups.Count + 1) * RibbonGroup.GroupPadding + this.realGroups.Select(group => group.GetWidth(group.WidthLevel)).Sum();
                    if (currentWidth > maxWidth)
                    {
                        RibbonGroup longeastGroup = null;
                        foreach (var group in this.realGroups)
                        {
                            if (group.WidthLevel > 0)
                            {
                                if (longeastGroup == null || group.GetWidth(group.WidthLevel) >= longeastGroup.GetWidth(longeastGroup.WidthLevel))
                                {
                                    longeastGroup = group;
                                }
                            }
                        }
                        if (longeastGroup != null)
                        {
                            longeastGroup.WidthLevel--;
                            continue;
                        }
                    }
                    break;
                }
                while (true)
                {
                    int currentWidth = (this.realGroups.Count + 1) * RibbonGroup.GroupPadding + this.realGroups.Select(group => group.GetWidth(group.WidthLevel)).Sum();
                    if (currentWidth > maxWidth)
                    {
                        int index = -1;
                        for (int i = this.realGroups.Count - 1; i >= 0; i--)
                        {
                            if (this.realGroups[i] == this.Groups[i])
                            {
                                index = i;
                                break;
                            }
                        }
                        if (index != -1)
                        {
                            RibbonGroup group = this.Groups[index].CompactedGroup;
                            this.realGroups[index] = group;
                            UpdateGroup(group, g, settings);
                            continue;
                        }
                    }
                    break;
                }
                {
                    int currentWidth = (this.realGroups.Count + 1) * RibbonGroup.GroupPadding + this.realGroups.Select(group => group.GetWidth(group.WidthLevel)).Sum();
                    if (currentWidth > maxWidth)
                    {
                        if (this.mostCompactGroup == null)
                        {
                            this.mostCompactGroup = new RibbonButtonGroup();
                            this.mostCompactGroup.Name = this.Name;
                            foreach (var item in this.realGroups.SelectMany(group => group.Items))
                            {
                                this.mostCompactGroup.BigItems.Add(item);
                            }
                            UpdateGroup(this.mostCompactGroup, g, settings);
                        }
                        this.realGroups.Clear();
                        this.realGroups.Add(this.mostCompactGroup.CompactedGroup);
                        UpdateGroup(mostCompactGroup.CompactedGroup, g, settings);
                    }
                }

                foreach (var group in this.realGroups)
                {
                    group.UpdateWithSizeDecided(g, settings);
                }
            }
        }

        public Rectangle GetGroupBounds(RibbonGroup targetGroup)
        {
            Rectangle panelBounds = this.Container.GetTabPanelBounds();
            int x = panelBounds.X + RibbonGroup.GroupPadding + 1;
            int y = panelBounds.Y + RibbonGroup.GroupPadding + 1;
            int h = panelBounds.Height - RibbonGroup.GroupPadding * 2 - 2;
            foreach (RibbonGroup group in this.realGroups)
            {
                int w = group.GetWidth(group.WidthLevel);
                if (group == targetGroup)
                {
                    return new Rectangle(x, y, w, h);
                }
                x += w + RibbonGroup.GroupPadding;
            }
            return Rectangle.Empty;
        }

        public RibbonGroup GetGroupFromPoint(Point location)
        {
            foreach (var group in this.realGroups)
            {
                if (GetGroupBounds(group).Contains(location))
                {
                    return group;
                }
            }
            return null;
        }
    }
}
