using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonMenuButton : RibbonMenuItem
    {
        public int MenuButtonLeftPadding = 5;

        public bool Hot { get; protected set; }

        public RibbonMenuButton()
        {
            this.Hot = false;
        }

        public override int GetSuggestedHeight(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            return MenuIconAreaHeight;
        }

        public override int GetSuggestedWidth(System.Drawing.Graphics g, RibbonThemaSettingsBase settings)
        {
            SizeF size = g.MeasureString(this.Name, settings.Font);
            return MenuIconAreaWidth + 2 * MenuButtonLeftPadding + (int)size.Width;
        }

        public override void Render(Graphics g, RibbonThemaSettingsBase settings, Rectangle itemBounds)
        {
            base.Render(g, settings, itemBounds);
            if (this.Hot)
            {
                RibbonColorItem i1 = settings.ButtonReleasedBackground1;
                RibbonColorItem i2 = settings.ButtonReleasedBackground2;
                RibbonColorItem i3 = settings.ButtonReleasedBackground3;
                RibbonColorItem i4 = settings.ButtonReleasedBackground4;
                settings.DrawDoubleGradientPanel(g, i1, i2, i3, i4, Rectangle.Inflate(itemBounds, 0, -1), 0.5);

                RibbonColorItem outTop = settings.ButtonReleasedOuterBorderTop;
                RibbonColorItem outBottom = settings.ButtonReleasedOuterBorderBottom;
                RibbonColorItem inTop = settings.ButtonReleasedInnerBorderTop;
                RibbonColorItem inBottom = settings.ButtonReleasedInnerBorderBottom;
                settings.DrawDoubleGradientBorder(g, outTop, outBottom, inTop, inBottom, itemBounds);
            }
            {
                Rectangle r = GetIconBounds(itemBounds);
                int x = r.Left + (r.Width - this.Image.Width) / 2;
                int y = r.Top + (r.Height - this.Image.Height) / 2;
                g.DrawImage(this.Image, x, y);
            }
            {
                Rectangle r = GetPanelBounds(itemBounds);
                SizeF size = g.MeasureString(this.Name, settings.Font);
                int x = r.Left + MenuButtonLeftPadding;
                int y = r.Top + (int)(r.Height - size.Height) / 2;
                g.DrawString(this.Name, settings.Font, settings.TabText.Brush, x, y);
            }
        }

        public override bool OnMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            return false;
        }

        public override bool OnMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            if (!this.Hot)
            {
                this.Hot = true;
                return true;
            }
            else
            {
                return false;
            }
        }

        public override bool OnMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            Executed();
            return false;
        }

        public override bool OnMouseLeave(EventArgs e)
        {
            if (this.Hot)
            {
                this.Hot = false;
                return true;
            }
            else
            {
                return false;
            }
        }

        protected virtual void Executed()
        {
            this.ItemContainer.GlobalServices.ItemExecuted(this);
        }
    }
}
