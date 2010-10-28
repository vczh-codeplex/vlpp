using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonButtonGroupItem : RibbonItem
    {
        public abstract int GetBigSize(Graphics g);
        public abstract int GetSmallWidth(Graphics g);
        public abstract int GetSmallCompactWidth(Graphics g);

        public override Size GetSize(Graphics g)
        {
            throw new NotSupportedException();
        }
    }
}
