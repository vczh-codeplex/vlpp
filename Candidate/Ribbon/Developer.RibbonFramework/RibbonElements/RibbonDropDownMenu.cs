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
            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                RibbonMenu menu = (RibbonMenu)this.RibbonItems;
                this.RibbonContainerSize = menu.TotalSize;
            }
        }

        public RibbonMenu Menu { get; set; }

        protected override RibbonDropDownBase.DropDownControl CreateDropDownControl()
        {
            return new MenuDropDownControl();
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
