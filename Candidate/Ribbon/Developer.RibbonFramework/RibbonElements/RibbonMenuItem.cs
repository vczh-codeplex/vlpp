using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.RibbonFramework.RibbonElements
{
    public abstract class RibbonMenuItem : RibbonItem
    {
        public int UpdatedHeight { get; protected set; }

        public abstract int GetSuggestedHeight(System.Drawing.Graphics g, RibbonThemaSettingsBase settings);

        public override void UpdateWithSizeDecided(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            this.UpdatedWidth = GetSuggestedWidth(g, settings);
            this.UpdatedHeight = GetSuggestedHeight(g, settings);
        }
    }
}
