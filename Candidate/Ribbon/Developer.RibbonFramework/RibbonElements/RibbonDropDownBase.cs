using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonDropDownBase : IDisposable
    {
        protected class DropDownControl : RibbonDropDownHost, IRibbonItemContainerServices
        {
            private RibbonItem capturedItem;

            public RibbonDropDownBase Owner { get; set; }
            public DropDownControl ParentDropDownControl { get; set; }
            public DropDownControl ChildDropDownControl { get; set; }

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
                CaptureMouse(item != null);
            }

            Point IRibbonItemContainerServices.GetLocationInScreen(RibbonItemContainer container)
            {
                return this.RibbonPanelControl.PointToScreen(new Point(0, 0));
            }

            Point IRibbonItemContainerServices.GetLocationInScreen()
            {
                return this.RibbonPanelControl.PointToScreen(new Point(0, 0));
            }

            Rectangle IRibbonItemContainerServices.GetBounds(RibbonItemContainer container)
            {
                return new Rectangle(0, 0, this.RibbonContainerSize.Width, this.RibbonContainerSize.Height);
            }

            void IRibbonItemContainerServices.ItemExecuted(RibbonItem item)
            {
                CloseToRoot();
            }

            RibbonDropDownHost IRibbonItemContainerServices.Host
            {
                get
                {
                    return this;
                }
            }

            void IRibbonItemContainerServices.RefreshItemContainer()
            {
                Render();
                this.RibbonPanelControl.Refresh();
            }

            #endregion

            protected void CloseToRoot()
            {
                if (this.ParentDropDownControl != null)
                {
                    this.ParentDropDownControl.CloseToRoot();
                }
                this.Close();
            }

            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                this.RibbonPanelControl.BackColor = this.Settings.Panel.Color;
                this.RibbonItems.Services = this;
            }

            protected override void OnOpening(System.ComponentModel.CancelEventArgs e)
            {
                if (this.ParentDropDownControl != null)
                {
                    this.OwnerItem = this.ParentDropDownControl.HostingItem;
                    if (this.ParentDropDownControl.ChildDropDownControl != null)
                    {
                        this.ParentDropDownControl.ChildDropDownControl.Close();
                    }
                    this.ParentDropDownControl.ChildDropDownControl = this;
                }
                base.OnOpening(e);
            }

            protected override void OnClosing(System.Windows.Forms.ToolStripDropDownClosingEventArgs e)
            {
                this.OwnerItem = null;
                if (this.ChildDropDownControl != null)
                {
                    this.ChildDropDownControl.Close();
                }
                if (this.ParentDropDownControl != null)
                {
                    this.ParentDropDownControl.ChildDropDownControl = null;
                    this.ParentDropDownControl = null;
                }
                base.OnClosing(e);
            }

            protected override void OnClosed(System.Windows.Forms.ToolStripDropDownClosedEventArgs e)
            {
                base.OnClosed(e);
                this.Owner.ribbonContainer.ResourceManager.Free(this);
                this.Owner.dropDownControl = null;
                this.Owner = null;
            }
        }

        public void Dispose()
        {
            if (this.dropDownControl != null)
            {
                this.dropDownControl.Dispose();
                this.dropDownControl = null;
            }
        }

        private DropDownControl dropDownControl = null;
        private RibbonContainer ribbonContainer;
        protected abstract Type GetDropDownControlType();
        protected abstract RibbonItemContainer RibbonItems { get; }

        public RibbonDropDownBase(RibbonContainer ribbonContainer)
        {
            this.ribbonContainer = ribbonContainer;
        }

        public RibbonDropDownHost DropDownHost
        {
            get
            {
                return this.dropDownControl;
            }
        }

        public void Open(RibbonItem item, Point relativeLocation)
        {
            Point p = item.ItemContainer.Services.GetLocationInScreen(item.ItemContainer);
            Point pg = item.ItemContainer.Services.GetBounds(item.ItemContainer).Location;
            Point pi = item.ItemContainer.GetItemBounds(item).Location;
            if (this.dropDownControl != null)
            {
                Close();
            }
            this.dropDownControl = this.ribbonContainer.ResourceManager.Allocate<DropDownControl>(GetDropDownControlType());
            this.dropDownControl.Owner = this;
            this.dropDownControl.RibbonItems = this.RibbonItems;
            this.dropDownControl.ParentDropDownControl = item.ItemContainer.Services.Host as DropDownControl;
            this.dropDownControl.Show(p.X + pi.X - pg.X + relativeLocation.X, p.Y + pi.Y - pg.Y + relativeLocation.Y);
        }

        public void Open(RibbonContainer ribbonContainer, Point relativeLocation)
        {
            Point p = (ribbonContainer as IRibbonItemContainerServices).GetLocationInScreen();
            if (this.dropDownControl != null)
            {
                Close();
            }
            this.dropDownControl = this.ribbonContainer.ResourceManager.Allocate<DropDownControl>(GetDropDownControlType());
            this.dropDownControl.Owner = this;
            this.dropDownControl.RibbonItems = this.RibbonItems;
            this.dropDownControl.ParentDropDownControl = null;
            this.dropDownControl.Show(p.X + relativeLocation.X, p.Y + relativeLocation.Y);
        }

        public void Close()
        {
            if (this.dropDownControl != null)
            {
                this.dropDownControl.Close();
                this.dropDownControl = null;
            }
        }
    }
}
