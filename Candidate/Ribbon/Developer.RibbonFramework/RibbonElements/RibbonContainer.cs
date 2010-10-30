using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Developer.RibbonFramework.RibbonElements
{
    public sealed class RibbonContainer : IDisposable, IRibbonItemContainerServices
    {
        private IRibbonInputCallback callback;
        private RibbonItem capturedItem;
        private List<RibbonTabBase> realTabs;
        private bool pressing = false;

        private RibbonButtonTab mostCompactButtonTab;
        private RibbonMenu mostCompactMenu;
        private bool compactTabMode;

        public RibbonResourceManager ResourceManager { get; private set; }
        public IList<RibbonTabBase> Tabs { get; private set; }
        public IList<RibbonTabGroup> TabGroups { get; private set; }

        public RibbonThemaSettingsBase Settings { get; private set; }
        public int TabTotalWidth { get; private set; }
        public int TabTotalHeight { get; private set; }
        public Rectangle RibbonBounds { get; private set; }
        public int RibbonTabOffset { get; private set; }

        public RibbonTabPanel SelectedTabPanel { get; private set; }

        public RibbonContainer(IRibbonInputCallback callback)
        {
            this.callback = callback;
            this.realTabs = new List<RibbonTabBase>();
            this.mostCompactMenu = new RibbonMenu();
            this.mostCompactButtonTab = new RibbonButtonTab();
            this.mostCompactButtonTab.Container = this;
            this.mostCompactButtonTab.DropDown = new RibbonDropDownMenu(this)
            {
                Menu = this.mostCompactMenu
            };

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

        Point IRibbonItemContainerServices.GetLocationInScreen()
        {
            return this.callback.GetLocationInScreen(this);
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
            if (!this.compactTabMode)
            {
                foreach (var group in this.TabGroups)
                {
                    if (group.FirstIndex >= 0 && group.FirstIndex < this.realTabs.Count && group.LastIndex >= 0 && group.LastIndex < this.realTabs.Count)
                    {
                        int x1 = GetTabBounds(this.realTabs[group.FirstIndex]).Left + panelBounds.Left;
                        int x2 = GetTabBounds(this.realTabs[group.LastIndex]).Right + panelBounds.Left;
                        int y1 = 0;
                        int y2 = panelBounds.Top;
                        Rectangle groupBounds = new Rectangle(x1, y1, x2 - x1, y2 - y1);
                        group.Render(g, this.Settings, groupBounds, drawOnAeroFrame);
                    }
                }
            }
        }

        private RibbonTabPanel FindFirstTabPanel()
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

        public void SelectTabPanel(RibbonTabPanel tabPanel)
        {
            this.SelectedTabPanel = tabPanel;
            ClearTabStates(false);
            if (this.compactTabMode)
            {
                if (this.realTabs.Count == 1)
                {
                    this.realTabs.Add(tabPanel.Owner);
                }
                else
                {
                    this.realTabs[1] = tabPanel.Owner;
                }
            }
            tabPanel.Owner.State = RibbonElementState.Selected;
            (this as IRibbonItemContainerServices).RefreshItemContainer();
        }

        public void Update(Graphics g, Rectangle ribbonBounds, int ribbonTabOffset)
        {
            this.compactTabMode = false;
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
            this.TabTotalWidth = this.realTabs.Select(t => t.TabWidth).Sum() + Math.Max(0, this.realTabs.Count - 1) * RibbonTabBase.TabPadding;
            this.TabTotalHeight = this.realTabs.Count > 0 ? this.realTabs.Select(t => t.TabHeight).Max() : 0;
            if (this.SelectedTabPanel == null || !this.realTabs.Contains(this.SelectedTabPanel.Owner))
            {
                SelectTabPanel(FindFirstTabPanel());
            }
            if (this.SelectedTabPanel != null)
            {
                this.SelectedTabPanel.Owner.State = RibbonElementState.Selected;
            }

            if (this.Tabs.Count > 0)
            {
                if (this.TabTotalWidth + ribbonTabOffset >= ribbonBounds.Width)
                {
                    this.compactTabMode = true;
                    this.realTabs.Clear();
                    this.realTabs.Add(this.mostCompactButtonTab);
                    this.mostCompactButtonTab.Name = this.Tabs[0].Name + " ...";
                    this.mostCompactButtonTab.Update(g, this.Settings);

                    if (this.SelectedTabPanel != null)
                    {
                        this.realTabs.Add(this.SelectedTabPanel.Owner);
                    }

                    this.mostCompactMenu.MenuItems.Clear();
                    foreach (var tab in this.Tabs)
                    {
                        this.mostCompactMenu.MenuItems.Add(tab.MenuItem);
                    }
                }
            }
            ClearTabStates(true);
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

        private bool ClearTabStates(bool keepSelect)
        {
            bool result = false;
            foreach (var tab in this.realTabs)
            {
                if (!keepSelect || tab.State != RibbonElementState.Selected)
                {
                    if (tab.State != RibbonElementState.Normal)
                    {
                        tab.State = RibbonElementState.Normal;
                        result = true;
                    }
                }
            }
            return result;
        }

        public bool OnMouseDown(MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
            {
                return false;
            }
            pressing = true;
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseDown(e);
            }
            bool result = ClearTabStates(true);
            RibbonTabBase selectedTab = GetTabFromPoint(e.Location);
            if (selectedTab != null)
            {
                if (selectedTab.State != RibbonElementState.Selected)
                {
                    selectedTab.State = RibbonElementState.Pressed;
                    return true;
                }
            }

            if (this.SelectedTabPanel != null)
            {
                RibbonGroup group = this.SelectedTabPanel.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    if (group.OnMouseDown(e))
                    {
                        return true;
                    }
                }
            }
            return result;
        }

        public bool OnMouseMove(MouseEventArgs e)
        {
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseMove(e);
            }
            bool result = ClearTabStates(true);
            RibbonTabBase selectedTab = GetTabFromPoint(e.Location);
            if (selectedTab != null)
            {
                RibbonElementState expectedState = this.pressing ? RibbonElementState.Pressed : RibbonElementState.Hot;
                if (selectedTab.State != RibbonElementState.Selected && selectedTab.State != expectedState)
                {
                    selectedTab.State = expectedState;
                    return true;
                }
            }

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
            if (e.Button != MouseButtons.Left)
            {
                return false;
            }
            this.pressing = false;
            if (this.capturedItem != null)
            {
                return this.capturedItem.OnMouseUp(e);
            }

            bool result = false;
            RibbonTabBase selectedTab = GetTabFromPoint(e.Location);
            if (selectedTab != null)
            {
                if (selectedTab.Selectable)
                {
                    result = ClearTabStates(false);
                    selectedTab.State = RibbonElementState.Selected;
                }
                else
                {
                    selectedTab.State = RibbonElementState.Normal;
                }
                selectedTab.Executed();
                if (selectedTab.Selectable)
                {
                    return result;
                }
            }
            result = ClearTabStates(true);

            if (this.SelectedTabPanel != null)
            {
                RibbonGroup group = this.SelectedTabPanel.GetGroupFromPoint(e.Location);
                if (group != null)
                {
                    if (group.OnMouseUp(e))
                    {
                        return true;
                    }
                }
            }
            return result;
        }

        public bool OnMouseLeave(EventArgs e)
        {
            bool result = ClearTabStates(true);
            this.pressing = false;
            if (this.SelectedTabPanel == null)
            {
                return result;
            }
            else
            {
                return this.SelectedTabPanel.RealGroups.Select(group => group.OnMouseLeave(e)).ToArray().Any() || result;
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
