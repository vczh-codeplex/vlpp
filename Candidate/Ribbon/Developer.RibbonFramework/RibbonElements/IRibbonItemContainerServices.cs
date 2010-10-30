using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public interface IRibbonItemContainerServices
    {
        RibbonItem CapturedItem { get; }
        void Capture(RibbonItem item);
        Point GetLocationInScreen(RibbonItemContainer container);
        Rectangle GetBounds(RibbonItemContainer container);
        void ItemExecuted(RibbonItem item);
        RibbonDropDownHost Host { get; }
        void RefreshItemContainer();
    }
}
