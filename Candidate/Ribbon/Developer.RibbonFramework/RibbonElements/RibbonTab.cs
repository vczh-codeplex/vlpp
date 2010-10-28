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

        public RibbonContainer Container { get; internal set; }
        public string Name { get; set; }
        public RibbonElementState State { get; internal set; }
        public IList<RibbonGroup> Groups { get; private set; }

        public int TabWidth { get; protected set; }
        public int TabHeight { get; protected set; }

        public RibbonTab()
        {
            this.Name = "RibbonTab";
            this.Groups = new List<RibbonGroup>();
        }

        public virtual void RenderTab(Graphics g, Rectangle tabBounds)
        {
            var settings = this.Container.Settings;
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
            SizeF size = g.MeasureString(this.Name, this.Container.Font);
            int tx = tabBounds.Left + (tabBounds.Width - (int)size.Width) / 2;
            int ty = tabBounds.Bottom - TabHeightOffset / 2 - (int)size.Height;
            g.DrawString(this.Name, this.Container.Font, settings.TabText.Brush, tx, ty);
        }

        public virtual void RenderPanel(Graphics g, Rectangle panelBounds)
        {
            var settings = this.Container.Settings;
            RibbonColorItem i1 = settings.Panel1;
            RibbonColorItem i2 = settings.Panel2;
            RibbonColorItem i3 = settings.Panel3;
            RibbonColorItem i4 = settings.Panel4;
            settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, panelBounds, (double)1 / 6);
            foreach (var group in this.Groups)
            {
                group.Render(g, this.GetGroupBounds(group));
            }
            panelBounds.Width -= 1;
            panelBounds.Height -= 1;
            g.DrawRectangle(settings.Border.Pen, panelBounds);
        }

        public virtual void Update(Graphics g)
        {
            SizeF size = g.MeasureString(this.Name, this.Container.Font);
            this.TabWidth = (int)size.Width + TabWidthOffset;
            this.TabHeight = (int)size.Height + TabHeightOffset;
            this.State = RibbonElementState.Normal;
            if (this.Groups.Count > 0)
            {
                foreach (var group in this.Groups)
                {
                    group.Tab = this;
                    group.Update(g);
                    group.WidthLevel = group.WidthLevelCount - 1;
                }
                int maxHeaderHeight = this.Groups.Select(group => group.HeaderMinHeight).Max();
                foreach (var group in this.Groups)
                {
                    group.HeaderHeight = maxHeaderHeight;
                }

                int maxWidth = this.Container.GetTabPanelBounds().Width;
                while (true)
                {
                    int currentWidth = (this.Groups.Count + 1) * RibbonGroup.GroupPadding + this.Groups.Select(group => group.GetWidth(group.WidthLevel)).Sum();
                    if (currentWidth > maxWidth)
                    {
                        RibbonGroup longeastGroup = null;
                        foreach (var group in this.Groups)
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

                foreach (var group in this.Groups)
                {
                    group.UpdateWithSizeDecided(g);
                }
            }
        }

        public Rectangle GetGroupBounds(RibbonGroup targetGroup)
        {
            Rectangle panelBounds = this.Container.GetTabPanelBounds();
            int x = panelBounds.X + RibbonGroup.GroupPadding + 1;
            int y = panelBounds.Y + RibbonGroup.GroupPadding + 1;
            int h = panelBounds.Height - RibbonGroup.GroupPadding * 2 - 2;
            foreach (RibbonGroup group in this.Groups)
            {
                int w = Math.Max(group.GetWidth(group.WidthLevel), group.HeaderMinWidth);
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
            foreach (var group in this.Groups)
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
