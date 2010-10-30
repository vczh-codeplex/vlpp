﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonContainer : IDisposable, IRibbonItemContainerServices
    {
        private IRibbonInputCallback callback;
        private RibbonItem capturedItem;
        private List<RibbonTabBase> realTabs;

        public RibbonResourceManager ResourceManager { get; private set; }
        public IList<RibbonTabBase> Tabs { get; private set; }
        public IList<RibbonTabGroup> TabGroups { get; private set; }
        public RibbonTabBase HotTab { get; private set; }

        public RibbonThemaSettingsBase Settings { get; protected set; }
        public int TabTotalWidth { get; protected set; }
        public int TabTotalHeight { get; protected set; }
        public Rectangle RibbonBounds { get; protected set; }
        public int RibbonTabOffset { get; protected set; }

        public RibbonTabPanel SelectedTabPanel { get; set; }

        public RibbonContainer(IRibbonInputCallback callback)
        {
            this.callback = callback;
            this.realTabs = new List<RibbonTabBase>();

            this.Tabs = new List<RibbonTabBase>();
            this.TabGroups = new List<RibbonTabGroup>();

            this.Settings = new RibbonThemaSettings();
            this.Settings.Font = SystemFonts.CaptionFont;
            this.ResourceManager = new RibbonResourceManager();
        }

        #region IRibbonItemContainerServices Members

        RibbonItem IRibbonItemContainerServices.CapturedItem
        {
            get
            {
                return this.capturedItem;
            }
        }

        void IRibbonItemContainerServices.Capture(RibbonItem item)
        {
            this.capturedItem = item;
            this.callback.CaptureMouse(item != null);
        }

        Point IRibbonItemContainerServices.GetLocationInScreen(RibbonItemContainer container)
        {
            Point pc = this.callback.GetLocationInScreen(this);
            RibbonGroup group = (RibbonGroup)container;
            Point pg = group.TabPanel.GetGroupBounds(group).Location;
            return new Point(pc.X + pg.X, pc.Y + pg.Y);
        }

        Rectangle IRibbonItemContainerServices.GetBounds(RibbonItemContainer container)
        {
            RibbonGroup group = (RibbonGroup)container;
            return group.TabPanel.GetGroupBounds(group);
        }

        void IRibbonItemContainerServices.ItemExecuted(RibbonItem item)
        {
        }

        RibbonDropDownHost IRibbonItemContainerServices.Host
        {
            get
            {
                return null;
            }
        }

        void IRibbonItemContainerServices.RefreshItemContainer()
        {
            this.callback.RefreshRibbon();
        }

        #endregion

        public void Dispose()
        {
            this.ResourceManager.Dispose();
            this.Settings.Dispose();
        }

        public void RenderTabPanel(Graphics g, Rectangle graphicsBounds)
        {
            g.FillRectangle(this.Settings.Panel.Brush, graphicsBounds);
            if (this.SelectedTabPanel != null)
            {
                this.SelectedTabPanel.RenderPanel(g, this.Settings, GetTabPanelBounds());
            }
            foreach (var tab in this.realTabs)
            {
                Rectangle bounds = GetTabBounds(tab);
                tab.RenderTab(g, this.Settings, bounds);
            }
        }

        public void RenderTabGroups(Graphics g, Rectangle panelBounds, bool drawOnAeroFrame)
        {
            foreach (var group in this.TabGroups)
            {
                int x1 = GetTabBounds(this.realTabs[group.FirstIndex]).Left + panelBounds.Left;
                int x2 = GetTabBounds(this.realTabs[group.LastIndex]).Right + panelBounds.Left;
                int y1 = 0;
                int y2 = panelBounds.Top;
                Rectangle groupBounds = new Rectangle(x1, y1, x2 - x1, y2 - y1);
                group.Render(g, this.Settings, groupBounds, drawOnAeroFrame);
            }
        }

        protected RibbonTabPanel FindFirstTabPanel()
        {
            foreach (var tab in this.Tabs)
            {
                RibbonTab ribbonTab = tab as RibbonTab;
                if (ribbonTab != null)
                {
                    return ribbonTab.TabPanel;
                }
            }
            return null;
        }

        public void Update(Graphics g, Rectangle ribbonBounds, int ribbonTabOffset)
        {
            this.realTabs.Clear();
            this.realTabs.AddRange(this.Tabs);
            this.RibbonBounds = ribbonBounds;
            this.RibbonTabOffset = ribbonTabOffset;
            foreach (var tab in this.realTabs)
            {
                tab.Container = this;
                tab.Update(g, this.Settings);
            }
            foreach (var group in this.TabGroups)
            {
                group.Container = this;
                group.Update(g);
            }
            this.TabTotalWidth = this.realTabs.Select(t => t.TabWidth).Sum();
            this.TabTotalHeight = this.realTabs.Count > 0 ? this.realTabs.Select(t => t.TabHeight).Max() : 0;
            if (this.SelectedTabPanel == null || !this.realTabs.Contains(this.SelectedTabPanel.Owner))
            {
                this.SelectedTabPanel = FindFirstTabPanel();
            }
            if (this.SelectedTabPanel != null)
            {
                this.SelectedTabPanel.Owner.State = RibbonElementState.Selected;
            }
            foreach (var tab in this.realTabs)
            {
                if (tab != this.SelectedTabPanel.Owner)
                {
                    tab.State = RibbonElementState.Normal;
                }
            }
            this.HotTab = null;
        }

        public Rectangle GetTabBounds(RibbonTabBase targetTab)
        {
            int x = this.RibbonBounds.Left + this.RibbonTabOffset + RibbonTabBase.TabPadding;
            int y = this.RibbonBounds.Top + RibbonTabBase.TabPadding;
            foreach (var tab in this.realTabs)
            {
                if (tab == targetTab)
                {
                    Rectangle bounds = new Rectangle(x, y, tab.TabWidth, this.TabTotalHeight);
                    return bounds;
                }
                x += tab.TabWidth + RibbonTabBase.TabPadding;
            }
            return Rectangle.Empty;
        }

        public RibbonTabBase GetTabFromPoint(Point location)
        {
            foreach (var tab in this.realTabs)
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
                this.RibbonBounds.Top + RibbonTabBase.TabPadding + this.TabTotalHeight,
                this.RibbonBounds.Width,
                this.RibbonBounds.Height - this.TabTotalHeight - RibbonTabBase.TabPadding
                );
            return panelBounds;
        }

        public bool OnMouseDown(MouseEventArgs e)
        {
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseDown(e);
            }
            RibbonTabBase selectedTab = GetTabFromPoint(e.Location);
            if (selectedTab != null)
            {
                if (selectedTab != this.SelectedTabPanel.Owner)
                {
                    if (selectedTab.Selectable)
                    {
                        foreach (var tab in this.realTabs)
                        {
                            tab.State = RibbonElementState.Normal;
                        }
                        selectedTab.State = RibbonElementState.Selected;
                    }
                    selectedTab.Executed();
                    return true;
                }
            }

            if (this.SelectedTabPanel != null)
            {
                RibbonGroup group = this.SelectedTabPanel.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    return group.OnMouseDown(e);
                }
            }
            return false;
        }

        public bool OnMouseMove(MouseEventArgs e)
        {
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseMove(e);
            }
            RibbonTabBase hotTab = GetTabFromPoint(e.Location);
            if (hotTab == this.SelectedTabPanel.Owner)
            {
                hotTab = null;
            }
            if (hotTab != this.HotTab)
            {
                foreach (var tab in this.realTabs)
                {
                    if (tab != this.SelectedTabPanel.Owner)
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
            if (this.SelectedTabPanel != null)
            {
                RibbonGroup group = this.SelectedTabPanel.GetGroupFromPoint(e.Location);
                foreach (var ribbonGroup in this.SelectedTabPanel.RealGroups)
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
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseUp(e);
            }
            if (this.SelectedTabPanel != null)
            {
                RibbonGroup group = this.SelectedTabPanel.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    return group.OnMouseUp(e);
                }
            }
            return false;
        }

        public bool OnMouseLeave(EventArgs e)
        {
            if (this.SelectedTabPanel == null)
            {
                return false;
            }
            else
            {
                return this.SelectedTabPanel.RealGroups.Select(group => group.OnMouseLeave(e)).ToArray().Any();
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
        Point GetLocationInScreen(RibbonContainer ribbonContainer);
        void RefreshRibbon();
    }
}
