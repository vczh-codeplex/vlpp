using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonDropDownGroup : RibbonDropDownBase
    {
        class GroupDropDownControl : DropDownControl
        {
            protected override void PrepareToOpen()
            {
                base.PrepareToOpen();
                RibbonGroup group = (RibbonGroup)this.RibbonItems;
                group.WidthLevel = group.WidthLevelCount - 1;
                this.RibbonContainerSize = new Size(group.GetWidth(group.WidthLevel), 102);
            }
        }

        public RibbonGroup Group { get; set; }

        protected override RibbonDropDownBase.DropDownControl CreateDropDownControl()
        {
            return new GroupDropDownControl();
        }

        protected override RibbonItemContainer RibbonItems
        {
            get
            {
                return this.Group;
            }
        }
    }
}
