using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonCompactedGroup : RibbonGroup
    {
        private RibbonContainer ribbonContainer;
        private RibbonGroup ribbonGroup;
        private RibbonButton ribbonButton;

        public RibbonCompactedGroup(RibbonContainer container, RibbonGroup group)
        {
            this.ribbonContainer = container;
            this.ribbonGroup = group;

            this.ribbonButton = new RibbonButton();
            this.ribbonButton.Name = this.ribbonGroup.Name;
            this.ribbonButton.ItemSize = RibbonItemSize.Big;
            this.ribbonButton.ButtonStyle = RibbonButtonStyle.DropDownButton;
            this.ribbonButton.DropDown = new RibbonDropDownGroup(this.ribbonContainer)
            {
                Group = this.ribbonGroup
            };

            RibbonItem[] items = this.ribbonGroup.Items.ToArray();
            foreach (var item in items)
            {
                if (item.BigImage != null)
                {
                    this.ribbonButton.BigImage = item.BigImage;
                    return;
                }
            }
            foreach (var item in items)
            {
                if (item.SmallImage != null)
                {
                    this.ribbonButton.BigImage = item.SmallImage;
                    return;
                }
            }
        }

        public override RibbonGroup CompactedGroup
        {
            get
            {
                throw new NotSupportedException();
            }
        }

        public override IEnumerable<RibbonItem> Items
        {
            get
            {
                return new RibbonItem[] { this.ribbonButton };
            }
        }

        public override Rectangle GetItemBounds(RibbonItem item)
        {
            if (this.ribbonButton == item)
            {
                return Rectangle.Inflate(this.Services.GetBounds(this), -GroupBorder, -GroupBorder);
            }
            else
            {
                return Rectangle.Empty;
            }
        }

        public override RibbonItem GetItemFromPoint(Point location)
        {
            if (GetItemBounds(this.ribbonButton).Contains(location))
            {
                return this.ribbonButton;
            }
            else
            {
                return null;
            }
        }

        public override int WidthLevelCount
        {
            get
            {
                return 1;
            }
        }

        public override int GetWidthInternal(int level)
        {
            return this.ribbonButton.UpdatedSize.Width + 2 * GroupBorder;
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            settings.DrawCarvedBorder(g, settings.LightBorder, settings.DarkBorder, itemBounds);
            this.ribbonButton.Render(g, settings, GetItemBounds(this.ribbonButton));
        }

        public override void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.Update(g, settings);
            this.ribbonButton.ItemContainer = this;
            this.ribbonButton.Update(g, settings);
        }

        public override void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
            base.UpdateWithSizeDecided(g, settings);
            this.ribbonButton.UpdateWithSizeDecided(g, settings);
        }
    }
}
