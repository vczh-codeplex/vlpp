using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonMenuSeparator : RibbonMenuItem
    {
        public override int GetWidth(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            if (itemSize != RibbonItemSize.MenuItem) throw new NotSupportedException();
            return 10;
        }

        public override int GetHeight(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, RibbonItemSize itemSize)
        {
            if (itemSize != RibbonItemSize.MenuItem) throw new NotSupportedException();
            return 3;
        }

        public override void Render(System.Drawing.Graphics g, RibbonThemaSettingsBase settings, System.Drawing.Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            Rectangle r = GetPanelBounds(itemBounds);
            int x1 = r.Left + 5;
            int x2 = r.Right - 5;
            int y = r.Top + 1;
            g.DrawLine(settings.MenuSeparator.Pen, x1, y, x2, y);
        }

        public override bool OnMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            return false;
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            return false;
        }

        public override bool OnMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            return false;
        }

        public override bool OnMouseLeave(EventArgs e)
        {
            return false;
        }
    }
}
