using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonDropDownBase : IDisposable
    {
        protected class DropDownControl : RibbonDropDownHost, IRibbonGlobalServices
        {
            private RibbonItem capturedItem;

            RibbonItem IRibbonGlobalServices.CapturedItem
            {
                get
                {
                    return this.capturedItem;
                }
            }

            void IRibbonGlobalServices.Capture(RibbonItem item)
            {
                this.capturedItem = item;
                CaptureMouse(item != null);
            }

            Point IRibbonGlobalServices.GetLocationInScreen(RibbonItemContainer container)
            {
                return this.RibbonPanelControl.PointToScreen(new Point(0, 0));
            }

            Rectangle IRibbonGlobalServices.GetBounds(RibbonItemContainer container)
            {
                return new Rectangle(0, 0, this.RibbonContainerSize.Width, this.RibbonContainerSize.Height);
            }

            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                this.RibbonPanelControl.BackColor = this.Settings.Panel.Color;
                this.RibbonItems.GlobalServices = this;
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
            Point p = item.ItemContainer.GlobalServices.GetLocationInScreen(item.ItemContainer);
            Point pg = item.ItemContainer.GlobalServices.GetBounds(item.ItemContainer).Location;
            Point pi = item.ItemContainer.GetItemBounds(item).Location;
            if (this.dropDownControl == null)
            {
                this.dropDownControl = CreateDropDownControl();
            }
            this.dropDownControl.RibbonItems = this.RibbonItems;
            this.dropDownControl.Show(p.X + pi.X - pg.X + relativeLocation.X, p.Y + pi.Y - pg.Y + relativeLocation.Y);
        }
    }
}
