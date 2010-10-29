using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonDropDownBase
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

            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                this.RibbonItems.GlobalServices = this;
            }

            protected override void OnClosed(System.Windows.Forms.ToolStripDropDownClosedEventArgs e)
            {
                base.OnClosed(e);
                this.Dispose();
            }
        }

        protected abstract DropDownControl CreateDropDownControl();
        protected abstract RibbonItemContainer RibbonItems { get; }

        public void Open(RibbonItem item, Point relativeLocation)
        {
        }
    }
}
