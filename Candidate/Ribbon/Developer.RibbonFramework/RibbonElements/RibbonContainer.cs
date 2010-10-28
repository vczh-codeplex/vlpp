using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonContainer : IDisposable
    {
        public IRibbonInputCallback Callback { get; private set; }
        public RibbonItem CapturedItem { get; private set; }

        public RibbonColorSettingsBase Settings { get; set; }
        public IList<RibbonTab> Tabs { get; private set; }
        public IList<RibbonTabGroup> TabGroups { get; private set; }
        public Font Font { get; set; }
        public RibbonTab SelectedTab { get; private set; }
        public RibbonTab HotTab { get; private set; }

        public int TabTotalWidth { get; protected set; }
        public int TabTotalHeight { get; protected set; }
        public Rectangle RibbonBounds { get; protected set; }
        public int RibbonTabOffset { get; protected set; }

        public RibbonContainer(IRibbonInputCallback callback)
        {
            this.Callback = callback;
            this.Settings = new RibbonColorSettings();
            this.Tabs = new List<RibbonTab>();
            this.TabGroups = new List<RibbonTabGroup>();
            this.Font = SystemFonts.CaptionFont;
        }

        public void Dispose()
        {
            this.Settings.Dispose();
        }

        public void RenderTabPanel(Graphics g, Rectangle graphicsBounds)
        {
            g.FillRectangle(this.Settings.Panel.Brush, graphicsBounds);
            int x = this.RibbonBounds.Left + this.RibbonTabOffset + RibbonTab.TabPadding;
            int y = this.RibbonBounds.Top + RibbonTab.TabPadding;
            if (this.SelectedTab != null)
            {
                this.SelectedTab.RenderPanel(g, GetTabPanelBounds());
            }
            foreach (var tab in this.Tabs)
            {
                Rectangle bounds = new Rectangle(x, y, tab.TabWidth, this.TabTotalHeight);
                tab.RenderTab(g, bounds);
                x += tab.TabWidth + RibbonTab.TabPadding;
            }
        }

        public void RenderTabGroups(Graphics g, Rectangle panelBounds, bool drawOnAeroFrame)
        {
            foreach (var group in this.TabGroups)
            {
                int x1 = GetTabBounds(this.Tabs[group.FirstIndex]).Left + panelBounds.Left;
                int x2 = GetTabBounds(this.Tabs[group.LastIndex]).Right + panelBounds.Left;
                int y1 = 0;
                int y2 = panelBounds.Top;
                Rectangle groupBounds = new Rectangle(x1, y1, x2 - x1, y2 - y1);
                group.Render(g, groupBounds, drawOnAeroFrame);
            }
        }

        public void Update(Graphics g, Rectangle ribbonBounds, int ribbonTabOffset)
        {
            this.RibbonBounds = ribbonBounds;
            this.RibbonTabOffset = ribbonTabOffset;
            foreach (var tab in this.Tabs)
            {
                tab.Container = this;
                tab.Update(g);
            }
            foreach (var group in this.TabGroups)
            {
                group.Container = this;
                group.Update(g);
            }
            this.TabTotalWidth = this.Tabs.Select(t => t.TabWidth).Sum();
            this.TabTotalHeight = this.Tabs.Count > 0 ? this.Tabs.Select(t => t.TabHeight).Max() : 0;
            if (!this.Tabs.Contains(this.SelectedTab))
            {
                if (this.Tabs.Count > 0)
                {
                    this.SelectedTab = this.Tabs[0];
                }
                else
                {
                    this.SelectedTab = null;
                }
            }
            if (this.SelectedTab != null)
            {
                this.SelectedTab.State = RibbonElementState.Selected;
            }
            foreach (var tab in this.Tabs)
            {
                if (tab != this.SelectedTab)
                {
                    tab.State = RibbonElementState.Normal;
                }
            }
            this.HotTab = null;
        }

        public Rectangle GetTabBounds(RibbonTab targetTab)
        {
            int x = this.RibbonBounds.Left + this.RibbonTabOffset + RibbonTab.TabPadding;
            int y = this.RibbonBounds.Top + RibbonTab.TabPadding;
            foreach (var tab in this.Tabs)
            {
                if (tab == targetTab)
                {
                    Rectangle bounds = new Rectangle(x, y, tab.TabWidth, this.TabTotalHeight);
                    return bounds;
                }
                x += tab.TabWidth + RibbonTab.TabPadding;
            }
            return Rectangle.Empty;
        }

        public RibbonTab GetTabFromPoint(Point location)
        {
            foreach (var tab in this.Tabs)
            {
                if (GetTabBounds(tab).Contains(location))
                {
                    return tab;
                }
            }
            return null;
        }

        public Rectangle GetTabPanelBounds()
        {
            Rectangle panelBounds = new Rectangle(
                this.RibbonBounds.Left,
                this.RibbonBounds.Top + RibbonTab.TabPadding + this.TabTotalHeight,
                this.RibbonBounds.Width,
                this.RibbonBounds.Height - this.TabTotalHeight - RibbonTab.TabPadding
                );
            return panelBounds;
        }

        public void Capture(RibbonItem item)
        {
            this.CapturedItem = item;
            this.Callback.CaptureMouse(item != null);
        }

        public bool OnMouseDown(MouseEventArgs e)
        {
            if (this.CapturedItem != null)
            {
                return this.CapturedItem.OnMouseDown(e);
            }
            RibbonTab selectedTab = GetTabFromPoint(e.Location);
            if (selectedTab != null)
            {
                if (selectedTab != this.SelectedTab)
                {
                    foreach (var tab in this.Tabs)
                    {
                        tab.State = RibbonElementState.Normal;
                    }
                    selectedTab.State = RibbonElementState.Selected;
                    this.SelectedTab = selectedTab;
                    return true;
                }
            }

            if (this.SelectedTab != null)
            {
                RibbonGroup group = this.SelectedTab.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    return group.OnMouseDown(e);
                }
            }
            return false;
        }

        public bool OnMouseMove(MouseEventArgs e)
        {
            if (this.CapturedItem != null)
            {
                return this.CapturedItem.OnMouseMove(e);
            }
            RibbonTab hotTab = GetTabFromPoint(e.Location);
            if (hotTab == this.SelectedTab)
            {
                hotTab = null;
            }
            if (hotTab != this.HotTab)
            {
                foreach (var tab in this.Tabs)
                {
                    if (tab != this.SelectedTab)
                    {
                        tab.State = RibbonElementState.Normal;
                    }
                }
                if (hotTab != null)
                {
                    hotTab.State = RibbonElementState.Hot;
                }
                this.HotTab = hotTab;
                return true;
            }

            bool result = false;
            if (this.SelectedTab != null)
            {
                RibbonGroup group = this.SelectedTab.GetGroupFromPoint(e.Location);
                foreach (var ribbonGroup in this.SelectedTab.Groups)
                {
                    if (ribbonGroup != group)
                    {
                        result = ribbonGroup.OnMouseLeave(new EventArgs()) || result;
                    }
                }
                if (group != null)
                {
                    result = group.OnMouseMove(e) || result;
                }
            }
            return result;
        }

        public bool OnMouseUp(MouseEventArgs e)
        {
            if (this.CapturedItem != null)
            {
                return this.CapturedItem.OnMouseUp(e);
            }
            if (this.SelectedTab != null)
            {
                RibbonGroup group = this.SelectedTab.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    return group.OnMouseUp(e);
                }
            }
            return false;
        }

        public bool OnMouseLeave(EventArgs e)
        {
            if (this.SelectedTab == null)
            {
                return false;
            }
            else
            {
                return this.SelectedTab.Groups.Select(group => group.OnMouseLeave(e)).ToArray().Any();
            }
        }
    }

    public enum RibbonElementState
    {
        Normal,
        Hot,
        Selected,
        Pressed
    }

    public interface IRibbonInputCallback
    {
        void CaptureMouse(bool capture);
    }
}
