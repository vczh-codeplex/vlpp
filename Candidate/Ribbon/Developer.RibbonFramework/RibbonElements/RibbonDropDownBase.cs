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

            public DropDownControl ParentDropDownControl { get; set; }

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

            Rectangle IRibbonItemContainerServices.GetBounds(RibbonItemContainer container)
            {
                return new Rectangle(0, 0, this.RibbonContainerSize.Width, this.RibbonContainerSize.Height);
            }

            void IRibbonItemContainerServices.ItemExecuted(RibbonItem item)
            {
                if (this.ParentDropDownControl != null)
                {
                    this.ParentDropDownControl.Close();
                }
                this.Close();
            }

            RibbonDropDownHost IRibbonItemContainerServices.Host
            {
                get
                {
                    return this;
                }
            }

            #endregion

            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                this.RibbonPanelControl.BackColor = this.Settings.Panel.Color;
                this.RibbonItems.Services = this;
            }

            protected override void OnOpening(System.ComponentModel.CancelEventArgs e)
            {
                this.OwnerItem = this.ParentDropDownControl != null ? this.ParentDropDownControl.HostingItem : null;
                base.OnOpening(e);
            }

            protected override void OnClosing(System.Windows.Forms.ToolStripDropDownClosingEventArgs e)
            {
                this.OwnerItem = null;
                this.ParentDropDownControl = null;
                base.OnClosing(e);
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
        protected abstract DropDownControl CreateDropDownControl();
        protected abstract RibbonItemContainer RibbonItems { get; }

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
            if (this.dropDownControl == null)
            {
                this.dropDownControl = CreateDropDownControl();
            }
            this.dropDownControl.RibbonItems = this.RibbonItems;
            this.dropDownControl.ParentDropDownControl = item.ItemContainer.Services.Host as DropDownControl;
            this.dropDownControl.Show(p.X + pi.X - pg.X + relativeLocation.X, p.Y + pi.Y - pg.Y + relativeLocation.Y);
        }
    }
}
