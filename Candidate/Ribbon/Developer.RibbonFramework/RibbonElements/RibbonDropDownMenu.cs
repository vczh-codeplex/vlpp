using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonDropDownMenu : RibbonDropDownBase
    {
        class MenuDropDownControl : DropDownControl
        {
            protected override System.Drawing.Size GetSuggestedSize()
            {
                RibbonMenu menu = (RibbonMenu)this.RibbonItems;
                return menu.TotalSize;
            }
        }

        public RibbonMenu Menu { get; set; }

        public RibbonDropDownMenu(RibbonContainer ribbonContainer)
            : base(ribbonContainer)
        {
        }

        protected override Type GetDropDownControlType()
        {
            return typeof(MenuDropDownControl);
        }

        protected override RibbonItemContainer RibbonItems
        {
            get
            {
                return this.Menu;
            }
        }
    }
}
