using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonItemContainer
    {
        public IRibbonGlobalServices GlobalServices { get; set; }

        public virtual IEnumerable<RibbonItem> Items
        {
            get
            {
                return new RibbonItem[] { };
            }
        }

        public virtual Rectangle GetItemBounds(RibbonItem item)
        {
            return Rectangle.Empty;
        }

        public virtual RibbonItem GetItemFromPoint(Point location)
        {
            return null;
        }

        public virtual void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
        }

        public virtual void Update(Graphics g, RibbonThemaSettingsBase settings)
        {
        }

        public virtual void UpdateWithSizeDecided(Graphics g, RibbonThemaSettingsBase settings)
        {
        }

        public virtual bool OnMouseDown(MouseEventArgs e)
        {
            OnMouseMove(e);
            RibbonItem item = GetItemFromPoint(e.Location);
            if (item != null && item.Enabled)
            {
                return item.OnMouseDown(e);
            }
            return false;
        }

        public virtual bool OnMouseMove(MouseEventArgs e)
        {
            RibbonItem item = GetItemFromPoint(e.Location);
            bool result = false;
            foreach (var ribbonItem in this.Items)
            {
                if (ribbonItem != item && ribbonItem.Enabled)
                {
                    result = ribbonItem.OnMouseLeave(new EventArgs()) || result;
                }
            }
            if (item != null && item.Enabled)
            {
                result = item.OnMouseMove(e) || result;
            }
            return result;
        }

        public virtual bool OnMouseUp(MouseEventArgs e)
        {
            OnMouseMove(e);
            RibbonItem item = GetItemFromPoint(e.Location);
            if (item != null && item.Enabled)
            {
                return item.OnMouseUp(e);
            }
            return false;
        }

        public virtual bool OnMouseLeave(EventArgs e)
        {
            return this.Items.Select(item => item.OnMouseLeave(e)).ToArray().Any();
        }
    }
}
