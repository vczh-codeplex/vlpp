using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonToolStripGroup : RibbonGroup
    {
        public IList<RibbonToolStrip> ToolStrips { get; private set; }

        public RibbonToolStripGroup()
        {
            this.ToolStrips = new List<RibbonToolStrip>();
        }
    }

    public class RibbonToolStrip
    {
        public IList<RibbonItem> ToolItems { get; private set; }
        public int NormalLine { get; set; }
        public int CompactLine { get; set; }

        public RibbonToolStrip()
        {
            this.ToolItems = new List<RibbonItem>();
        }
    }
}
