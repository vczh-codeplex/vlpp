using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonButtonGroupItem : RibbonItem
    {
        public abstract int GetBigWidth(Graphics g, RibbonThemaSettingsBase settings);
        public abstract int GetSmallWidth(Graphics g, RibbonThemaSettingsBase settings);
        public abstract int GetSmallCompactWidth(Graphics g, RibbonThemaSettingsBase settings);
    }
}
