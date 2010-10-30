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
            }

            protected override System.Drawing.Size GetSuggestedSize()
            {
                RibbonGroup group = (RibbonGroup)this.RibbonItems;
                return new Size(group.GetWidth(group.WidthLevel), 102);
            }
        }

        public RibbonGroup Group { get; set; }

        public RibbonDropDownGroup(RibbonContainer ribbonContainer)
            : base(ribbonContainer)
        {
        }

        protected override Type GetDropDownControlType()
        {
            return typeof(GroupDropDownControl);
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
