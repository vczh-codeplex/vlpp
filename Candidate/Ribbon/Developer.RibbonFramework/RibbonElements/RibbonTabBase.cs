using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonTabBase
    {
        public const int TabPadding = 3;

        public RibbonContainer Container { get; internal set; }
        public string Name { get; set; }
        public RibbonElementState State { get; protected internal set; }

        public int TabWidth { get; protected set; }
        public int TabHeight { get; protected set; }

        public RibbonTabBase()
        {
            this.Name = "";
        }

        public abstract void RenderTab(Graphics g, RibbonThemaSettingsBase settings, Rectangle tabBounds);
        public abstract void Update(Graphics g, RibbonThemaSettingsBase settings);
        public abstract void Executed();
    }
}
