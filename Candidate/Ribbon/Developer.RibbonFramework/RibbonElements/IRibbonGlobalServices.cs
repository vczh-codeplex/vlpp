using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public interface IRibbonGlobalServices
    {
        RibbonItem CapturedItem { get; }
        void Capture(RibbonItem item);
        Point GetLocationInScreen(RibbonItemContainer container);
        Rectangle GetBounds(RibbonItemContainer container);
    }
}
